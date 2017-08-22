#ifndef EVTNAVIGATOR_H
#define EVTNAVIGATOR_H

#include "TObject.h"
#include "TClass.h"
#include "TTimeStamp.h"
#include "EDMUtil/SmartRef.h"
#include "Event/HeaderObject.h"
#include <vector>
#include <string>

namespace JM {

class EvtNavigator : public TObject {

    public:
        // Default constructor
        EvtNavigator();
        // Destructor
        ~EvtNavigator();
        // Copy constructor
        EvtNavigator(const EvtNavigator& nav);
        // Assignment operator
        EvtNavigator& operator=(const EvtNavigator& nav);
        // Add an event header to EvtNavigator
        void addHeader(const std::string& path, JM::HeaderObject* header);
        // If path is not given, use default one
        void addHeader(JM::HeaderObject* header);
        // Copy an event header from another EvtNavigator, and save it using the newPath(if not given, use the oldPath)
        bool copyHeader(JM::EvtNavigator* another, const std::string& oldPath, const std::string& newPath="");
        // Addjust the paths of EvtNavigator(called by Output service)
        void adjustPath(const std::vector<std::string>& paths);
        // Get the event header of a certain path
        JM::HeaderObject* getHeader(const std::string& path);
        // Get the list of the paths holded by EvtNavigator
        std::vector<std::string>& getPath();
        const std::vector<std::string>& getPath() const;
        // Get the list of SmartRefs
        std::vector<JM::SmartRef*>& getRef();
        const std::vector<JM::SmartRef*>& getRef() const;
        // Set the entry of an event header, called by output system
        void setHeaderEntry(const std::string& path, int entry);
        // Set m_paths after EvtNavigator is loaded, called by input system
        void setPath(const std::vector<std::string>& paths);
        // Set whether a path will be written out
        void setWriteFlag(const std::string& path, bool write = false);
        // Set the write flag of a path to true
        bool writePath(const std::string& path);
        // Set the write flags of all paths to true
        void resetWriteFlag();
        // Get time stamp
        const TTimeStamp& TimeStamp() const;
        TTimeStamp& TimeStamp();
        // Set time stamp
        void setTimeStamp(const TTimeStamp& value);
    
    private:
        // paths and write flags won't be saved
        std::vector<std::string>    m_paths;     //!
        std::vector<bool>           m_writeFlag; //!
        std::vector<JM::SmartRef*>  m_refs;
        TTimeStamp                  m_TimeStamp; // Time stamp of event

        // Initialize self, using another EvtNavigator
        void init(const EvtNavigator& nav);
        JM::SmartRef* getSmartRef(const std::string& path);

    ClassDef(EvtNavigator,2)

};

} // namespace JM

#endif
