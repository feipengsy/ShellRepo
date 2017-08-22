#ifndef ROOTIOUTIL_ROOTFILEWRITER_H
#define ROOTIOUTIL_ROOTFILEWRITER_H

#include "TObject.h"

#include <string>
#include <vector>
#include <map>

class RootOutputFileHandle;
class TTree;
class TDirectory;

namespace JM {
    class TreeMetaData;
    class EvtNavigator;
}

class OutputTreeHandle {

    public:
        // Lazy-loading data types
        typedef std::vector<std::string>                  StringVector;
        typedef std::vector<std::vector<Short_t> >        BIDVector;
        typedef std::vector<std::vector<Int_t> >          UIDVector;

        OutputTreeHandle(const std::string& path, const std::string& objName, TDirectory* dir);
        ~OutputTreeHandle();

        void setAddress(void* addr) { m_addr = addr; }
        void resetAddress() { m_addr = 0; }
        Long64_t entries() const { return m_entries; }
        const std::string& objectName() const { return m_objName; }
        const std::string& fullTreeName() const { return m_fullTreeName; }
        JM::TreeMetaData* getTreeMetaData() { return m_metaData; }
        void initialize();
        bool fill(int& nbytes);
        void write();
        void writeUID(RootOutputFileHandle* file);

    private:
        void fillUID(int bid = -1);

        std::string        m_path;
        std::string        m_objName;
        std::string        m_fullTreeName;
        TTree*             m_tree;
        void*              m_addr;
        TDirectory*        m_dir;
        JM::TreeMetaData*  m_metaData;
        Long64_t           m_entries;

        // For lazy-loading data
        StringVector       m_guid;
        BIDVector          m_bid;
        UIDVector          m_uid;
        
};

class RootFileWriter {

    public:
        typedef std::map<std::string, OutputTreeHandle*>  String2TreeHandle;
        typedef std::vector<JM::TreeMetaData*>            TMDVector;
        
        RootFileWriter(const std::string& treepath, const std::string& headerName);
        ~RootFileWriter();
        // Get current file handle
        RootOutputFileHandle* getFile();
        // Write current state of addr to disk.
        bool write(JM::EvtNavigator* nav);
        // Write tree to file and close file.
        bool close();

        void revise();
        // Start a new output file
        void newFile(RootOutputFileHandle* file);
        // Return the number of entries in the output stream
        int fileEntries();
        // Set the name of event header
        void setHeaderName(const std::string& name);

    private:
        // Initialize the writer
        void initialize();
        // Write header data to tree
        bool writeHeader();
        // Write event data to tree
        bool writeEvent();
        // Write Evtnavigators to tree
        bool writeNav();
        // Check if this path is the last path of its output file
        void checkFilePath();
        // Set Address of branches to be filled
        void setAddress(JM::EvtNavigator* nav);
        /// Reset the addresses to 0
        void resetAddress();

    private:
        RootOutputFileHandle*   m_file;
        OutputTreeHandle*       m_headerTree;
        String2TreeHandle       m_eventTrees;
        TTree*                  m_navTree;
        TMDVector               m_treeMetaDatas;
        TDirectory*             m_dir;
        std::string             m_path;
        std::string             m_headerName;
        bool                    m_withNav;
        bool                    m_initialized;
        int                     m_fileEntries;
        JM::EvtNavigator*       m_navAddr;  
};


#endif
