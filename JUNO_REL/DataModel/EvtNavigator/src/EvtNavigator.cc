#include "EvtNavigator/EvtNavigator.h"
#include "EDMManager.h"
#include "EvtNavigator/Elec2DetRelation.h"
#include <algorithm>

ClassImp(JM::EvtNavigator);

JM::EvtNavigator::EvtNavigator()
  : m_elec2det(0)
  , m_writeRelFlag(false)
{
    this->Class()->IgnoreTObjectStreamer();
}

JM::EvtNavigator::~EvtNavigator()
{
    std::vector<JM::SmartRef*>::iterator it, end = m_refs.end();
    for (it = m_refs.begin(); it != end; ++it) {
        delete *it;
    }
    if (m_elec2det)  delete m_elec2det;
}

JM::EvtNavigator::EvtNavigator(const JM::EvtNavigator& nav)
    : TObject(nav)
{
    this->init(nav);
}

JM::EvtNavigator& JM::EvtNavigator::operator=(const JM::EvtNavigator& nav)
{
    if (this != &nav) {
        TObject::operator=(nav);
        this->init(nav);
    }
    return *this;
}

void JM::EvtNavigator::init(const JM::EvtNavigator& nav)
{
    m_paths = nav.m_paths;
    m_writeFlag = nav.m_writeFlag;
    m_TimeStamp = nav.m_TimeStamp;
    // Clear previous SmartRefs
    std::vector<SmartRef*>::iterator it, end = m_refs.end();
    for (it = m_refs.begin(); it != end; ++it) {
        delete *it;
    }
    // Copy new SmartRefs
    std::vector<SmartRef*>::const_iterator it2, end2 = nav.m_refs.end();
    for (it2 = nav.m_refs.begin(); it2 != end2; ++it2) {
        SmartRef* ref = new SmartRef(**it2);
        m_refs.push_back(ref);
    }
}

JM::HeaderObject* JM::EvtNavigator::getHeader(const std::string& path)
{
    JM::SmartRef* ref = this->getSmartRef(path);
    if (!ref) {
        return 0;
    }
    return static_cast<JM::HeaderObject*>(ref->GetObject());
}

std::vector<std::string>& JM::EvtNavigator::getPath()
{
    return m_paths;
}

const std::vector<std::string>& JM::EvtNavigator::getPath() const
{
    return m_paths;
}

std::vector<JM::SmartRef*>& JM::EvtNavigator::getRef()
{
    return m_refs;
}

const std::vector<JM::SmartRef*>& JM::EvtNavigator::getRef() const
{   
    return m_refs;
}

void JM::EvtNavigator::setHeaderEntry(const std::string& path, int entry)
{
    JM::SmartRef* ref = this->getSmartRef(path);
    if (!ref) {
        return;
    }
    ref->setEntry(entry);
}

void JM::EvtNavigator::addHeader(const std::string& path, JM::HeaderObject* header)
{
    m_paths.push_back(path);
    SmartRef* ref = new SmartRef();
    m_refs.push_back(ref);
    m_refs.back()->SetObject(header);
    m_writeFlag.push_back(true);
}

void JM::EvtNavigator::addHeader(JM::HeaderObject* header) 
{
    m_paths.push_back(EDMManager::get()->getPathWithHeader(header->ClassName()));
    SmartRef* ref = new SmartRef();
    m_refs.push_back(ref);
    m_refs.back()->SetObject(header);
    m_writeFlag.push_back(true);
}

bool JM::EvtNavigator::copyHeader(JM::EvtNavigator* another, const std::string& oldPath, const std::string& newPath)
{
    // Get old Header information
    JM::SmartRef* oref = another->getSmartRef(oldPath);
    if (0 == oref) return false;
    JM::HeaderObject* header = another->getHeader(oldPath);
    if (0 == header) return false;
    std::string path = newPath.size() ? newPath : oldPath;
    // Add Header
    m_paths.push_back(path);
    SmartRef* ref = new SmartRef();
    ref->setEntry(oref->entry());
    m_refs.push_back(ref);
    m_refs.back()->SetObject(header);
    m_writeFlag.push_back(true);
    return true;
}

void JM::EvtNavigator::adjustPath(const std::vector<std::string>& path)
{
    for (size_t i = 0; i < path.size(); ++i ) {
        std::vector<std::string>::iterator pos = std::find(m_paths.begin(), m_paths.end(),path[i]);
        if (pos == m_paths.end()) {
            // One new path, insert it just for occupying position
            m_paths.insert(m_paths.begin() + i, path[i]);
            m_refs.insert(m_refs.begin() + i, new SmartRef);
            m_writeFlag.insert(m_writeFlag.begin() + i, false);
        }
        else {
            size_t ps = pos - m_paths.begin();
            if (ps != i) {
                // We find the path, but it's on the wrong position
                std::swap(*pos, m_paths[i]);
                std::swap(m_refs[pos - m_paths.begin()], m_refs[i]);
                bool temp = m_writeFlag[pos - m_paths.begin()];
                m_writeFlag[pos - m_paths.begin()] = m_writeFlag[i];
                m_writeFlag[i] = temp;
            }
        }
    }
}

void JM::EvtNavigator::setPath(const std::vector<std::string>& paths)
{
    m_paths = paths;
}

bool JM::EvtNavigator::writePath(const std::string& path)
{
    std::vector<std::string>::iterator pos = find(m_paths.begin(), m_paths.end(), path);
    if (m_paths.end() == pos) {
        return false;
    }
    return m_writeFlag[pos - m_paths.begin()];
}

void JM::EvtNavigator::setWriteFlag(const std::string& path, bool write)
{
    std::vector<std::string>::iterator pos = find(m_paths.begin(), m_paths.end(), path);
    if (m_paths.end() == pos) return;
    m_writeFlag[pos - m_paths.begin()] = write;
}

void JM::EvtNavigator::resetWriteFlag()
{
    if (m_writeFlag.size()) m_writeFlag.clear();
    int npath = m_paths.size();
    for (int i = 0; i < npath; ++i) {
        m_writeFlag.push_back(true);
    }
}

const TTimeStamp& JM::EvtNavigator::TimeStamp() const
{
    return m_TimeStamp;
}

TTimeStamp& JM::EvtNavigator::TimeStamp()
{
    return m_TimeStamp;
}

void JM::EvtNavigator::setTimeStamp(const TTimeStamp& value)
{
    m_TimeStamp = value;
}

JM::SmartRef* JM::EvtNavigator::getSmartRef(const std::string& path)
{
    std::vector<std::string>::iterator pos = std::find(m_paths.begin(), m_paths.end(), path);
    if (m_paths.end() == pos) {
        return 0;
    }
    size_t ps = pos - m_paths.begin();
    if (ps >= m_refs.size()) {
        return 0;
    } 
    return m_refs[ps];
}

void JM::EvtNavigator::setElec2DetRelation(Elec2DetRelation* relation)
{
    if (!m_elec2det) m_elec2det = new SmartRef;
    m_elec2det->SetObject(relation);
    // Write out the relation first time when it's created
    m_writeRelFlag = true;
}

Elec2DetRelation* JM::EvtNavigator::getElec2DetRelation()
{
    return m_elec2det ? dynamic_cast<Elec2DetRelation*>(m_elec2det->GetObject()) : 0;
}

void JM::EvtNavigator::setRelEntry(Long64_t value)
{
    if (m_elec2det) m_elec2det->setEntry(value);
}
