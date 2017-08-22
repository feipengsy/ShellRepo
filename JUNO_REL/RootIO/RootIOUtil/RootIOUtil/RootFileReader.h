#ifndef ROOT_FILE_READER_H
#define ROOT_FILE_READER_H 0

#include <string>
#include <map>
#include <vector>

class NavTreeList;

class RootFileReader {

    typedef std::vector<std::string> StringVector;

    public:
        RootFileReader(const StringVector& filelist, bool withNav);
        ~RootFileReader();

        // Analyze input files, construct NavTreeList and InputElementKeeper
        bool checkAndExecute();
        const StringVector& getNavPath() { return m_navPath; }
        NavTreeList* getNavTree() { return m_navTree; }
        // For SniperLog
        const std::string& objName() { return m_objName; }

    private:
        StringVector m_fileList;
        bool m_withNav;
        NavTreeList* m_navTree;
        StringVector m_navPath;
        std::string  m_objName;
};

#endif
