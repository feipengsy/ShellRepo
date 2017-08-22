#include "RootIOUtil/RootFileReader.h"
#include "RootIOUtil/NavTreeList.h"
#include "FileMetaData.h"
#include "RootFileInter.h"
#include "InputElementKeeper.h"
#include "SniperKernel/SniperLog.h"
#include <boost/foreach.hpp>
#include <algorithm>
#include "TFile.h"

RootFileReader::RootFileReader(const StringVector& fileList, bool withNav)
    : m_fileList(fileList)
    , m_withNav(withNav)
    , m_navTree(0)
    , m_objName("RootFileReader")
{
}

RootFileReader::~RootFileReader()
{
}

bool RootFileReader::checkAndExecute()
{
    // Remove reduplicated files
    StringVector::iterator it, ret, beg = m_fileList.begin();
    for (it = beg + 1; it != m_fileList.end(); ) {
        ret = std::find(beg, it, *it);
        if (ret != it) {
            LogWarn << "Found reduplicated input file: " << *it
                    << ", skipped." << std::endl;
            it = m_fileList.erase(it);
        }
        else { 
            ++it;
        }
    }

    int highestPriority = -100;
    std::map<int, std::string> fid2fname;
    typedef std::map<int, std::string>::value_type fid2fname_pair;
    // Check reasonableness and register file
    BOOST_FOREACH(const std::string& filename, m_fileList) {
        TFile* file = RootFileInter::OpenFile(filename);
        if (0 == file) {
            LogError << "Failed to open file: " << filename
                     << std::endl;
            return false;
        }
        // Check if file is recovered


        // Get FileMetaData
        JM::FileMetaData* fmd = RootFileInter::GetFileMetaData(file);
        if (0 == fmd) {
            LogError << "Failed to retrieve FileMetaData of file: " << filename
                     << std::endl;
            return false;
        }
        // Register file
        int fileid = InputElementKeeper::GetInputElementKeeper()->RegisterFile(filename, fmd);
        if (-1 == fileid) {
            LogError << "Failed to register input file: " << filename
                     << std::endl;
            return false;
        }

        // Collect FileMetaDatas with the highest priority of the file list
        int priority = fmd->GetNavPriority();
        if (m_withNav && priority >= highestPriority) {
            int priority = fmd->GetNavPriority();
            if (priority > highestPriority) {
                // We've got new list, purge old one
                fid2fname.clear();
                highestPriority = priority;
                m_navPath = fmd->GetNavPath();
            }
            fid2fname.insert(std::make_pair(fileid, filename));
        }

        delete fmd;
        file->Close();
    }

    // Build NavTreeList
    if (m_withNav) {
        m_navTree = new NavTreeList;
        BOOST_FOREACH(const fid2fname_pair& item, fid2fname) {
            m_navTree->push_back(new NavTreeHandle(item.first, item.second));
        }
    }

    return true;
}
