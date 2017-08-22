#include "EvtNavigator/Elec2DetRelation.h"
#include "Event/SimHeader.h"

Elec2DetRelation::Elec2DetRelation()
{
}

Elec2DetRelation::~Elec2DetRelation()
{
    for (size_t i = 0; i < m_simheaders.size(); ++i)  delete m_simheaders[i]; 
}

int Elec2DetRelation::addSimHeader(JM::SimHeader* header)
{
    // Considering performance, won't check the repetitiveness of the headers
    // Create a new SmartRef to cache the header, so it won't be destructed by the DataBuffer
    JM::SmartRef* ref = new JM::SmartRef;
    ref->SetObject(header);
    m_simheaders.push_back(ref);
    return m_simheaders.size() - 1;
}

void Elec2DetRelation::addHit(int PMTID, int eventid, int hitid, int type) 
{
    Long64_t value = eventid + ((Long64_t)hitid << 32) + ((Long64_t)type << 48);
    m_hitids[PMTID].push_back(value);
}
