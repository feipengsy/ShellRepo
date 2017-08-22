#include "RootIOUtil/NavTreeList.h"
#include "InputElementKeeper.h"
#include "SniperKernel/SniperLog.h"

#include "TTree.h"
#include "TBranch.h"

NavTreeHandle::NavTreeHandle(int fileid, const std::string& filename)
    : m_tree(0), m_fileID(fileid)
    , m_entry(-1), m_entries(-1)
    , m_addr(0), m_filename(filename)
    , m_name("NavTreeHandle"), m_opened(false)
{
  InputElementKeeper* keeper = InputElementKeeper::GetInputElementKeeper();
  m_filename = keeper->GetFileName(fileid);
}

NavTreeHandle::~NavTreeHandle()
{
}

bool NavTreeHandle::open()
{
    if (m_opened) return true;
    InputElementKeeper* keeper = InputElementKeeper::GetInputElementKeeper();
    LogDebug << "Getting nav tree from " << m_filename
             << std::endl;
    bool ok = keeper->GetNavTree(m_fileID, m_tree);
    keeper->AddTreeRef(m_fileID);
    if (!ok) {
        LogError << "Failed to open file for nav tree"
                 << std::endl;
        return false;
    }
    m_opened = true;
    if (!m_tree) {
        LogError << "Failed to get nav tree"
                 << std::endl;
        return false;
    }
    if (m_entries < 0) {
        m_entries = m_tree->GetEntries();
    }
    return true;
}

bool NavTreeHandle::leave()
{
    LogDebug << "Leaving " << m_filename << " for nav tree"
             << std::endl;

    InputElementKeeper* keeper = InputElementKeeper::GetInputElementKeeper();
    // dec reference to the file owning this nav tree by one
    keeper->DecTreeRef(m_fileID);
    m_tree = 0;
    m_addr = 0;
    m_entry = -1;
    m_opened = false;

    return true;
}

int NavTreeHandle::entry()
{
    return m_entry;
}

bool NavTreeHandle::setEntry(int entry)
{
    if (!m_opened) return false;
    if (entry >= m_entries) {
        LogError << "Entry too large >= " << m_entries
                 << std::endl;
        return false;
    }
    m_entry = entry;
    return true;
}

bool NavTreeHandle::setAddr(void*& addr)
{
    //if (addr == m_addr) return true;
    if (!m_tree) return false;
    TBranch* b = m_tree->GetBranch("EvtNavigator");
    if (!b) {
        LogError << "Invalid nav tree: can't find nav branch"
                 << std::endl;
        return false;
    }

    LogDebug << "setAddr("<<(void*)addr<<") for nav in " << m_filename
             << std::endl;

    b->SetAddress(&addr);
    m_addr = addr;
    return true;
}

bool NavTreeHandle::read(void*& addr)
{
    if (m_entry < 0 || m_entry >= m_entries) {
        LogError << "Bad entry: " << m_entry
                 << std::endl;
        return false;
    }

    if (!this->setAddr(addr)) return false;

    int nbytes = m_tree->GetEntry(m_entry);
    return nbytes > 0;
}

int NavTreeHandle::entries()
{
    if (m_entries < 0) {        // not yet opened 
        if (!this->open()) return 0;
        this->leave();          // may not want to leave open long term, leave it as found it
    }
    return m_entries;
}

bool NavTreeHandle::next(int steps)
{
    return this->setEntry(m_entry+steps);
}

bool NavTreeHandle::beginning()
{
    if (!this->open()) return false;
    m_entry = 0;
    return true;
}

bool NavTreeHandle::ending()
{
    if (!this->open()) return false;
    m_entry = m_entries-1;
    return true;
}


bool NavTreeHandle::prev(int steps)
{
    return this->setEntry(m_entry-steps);
}

bool NavTreeHandle::opened()
{
    return m_opened;
}

// ------ NavTreeList ------

NavTreeList::NavTreeList()
 : std::vector<NavTreeHandle*>(), m_index(-1), m_name("NavTreeList")
{
}

NavTreeList::~NavTreeList()
{
    NavTreeList::iterator it, itend = this->end();
    for (it = this->begin(); it != itend; ++it) {
        if ((*it)->opened()) (*it)->leave();
        delete *it;
    }
}

int NavTreeList::index()
{
    return m_index;
}

NavTreeHandle* NavTreeList::current()
{
    if (m_index < 0 || m_index >= (int)size()) return 0;
    return (*this)[m_index];
}

bool NavTreeList::next()
{
    if (m_index + 1 >= (int)size()) {
        LogDebug << "Already at last file" 
                 << std::endl;
        return false;
    }
    NavTreeHandle* handle = current();
    if (handle) handle->leave();
    ++m_index;
    handle = current();
    if (!handle) {
        LogError << "Failed to get next nav tree"
                 << std::endl;
        return false;
    }
    handle->open();
    return true;
}

bool NavTreeList::last()
{
    if (m_index+1 == (int)size()) return true;
    NavTreeHandle* handle = current();
    if (handle) handle->leave();
    m_index = (int)size() - 1;
    handle = current();
    if (!handle) {
        LogError << "Falied to get last nav tree"
                 << std::endl;
        return false;
    }
    handle->open();
    return true;
}

bool NavTreeList::prev()
{
    if (m_index <= 0) {
        LogError << "Already at first nav tree"
                 << std::endl;
        return false;
    }
    NavTreeHandle* handle = current();
    if (handle) handle->leave();
    --m_index;
    handle = current();
    if (!handle) {
        LogError << "Failed to get prev nav tree"
                 << std::endl;
        return false;
    }
    handle->open();
    return true;
}

bool NavTreeList::first()
{
    if (!m_index) return true;
    NavTreeHandle* handle = current();
    if (handle) handle->leave();
    m_index = 0;
    handle = current();
    if (!handle) {
        LogError << "Failed to get first nav tree"
                 << std::endl;
        return false;
    }
    handle->open();
    return true;
}

bool NavTreeList::jump(int index)
{
    if (m_index == index) return true;
    NavTreeHandle* handle = current();
    if (handle) handle->leave();
    m_index = index;
    handle = current();
    if (!handle) {
        LogError << "Failed to get "<<index<<"th nav tree"
                 << std::endl;
        return false;
    }
    handle->open();
    return true;
}

int NavTreeList::entriesBefore(int index)
{
    if (index < 0 || index >= (int)size()) return -1;
    // cound up entries up to but not including current index
    int totEntries = 0;
    for (int ind = 0; ind < index; ++ind) {
        int entries = (*this)[ind]->entries();
        if (entries < 0) return -1;
        totEntries += entries;
    }
    return totEntries;
}
