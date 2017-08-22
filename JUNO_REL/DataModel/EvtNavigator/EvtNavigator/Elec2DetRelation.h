#ifndef ELEC2DETRELATION_H
#define ELEC2DETRELATION_H

#include "Event/EventObject.h"
#include "EDMUtil/SmartRef.h"
#include <map>
#include <vector>

namespace JM {

    class SimHeader;

}

class Elec2DetRelation : public JM::EventObject {
    // Derive from EventObject, making use of SmartRef
public:
    Elec2DetRelation();
    ~Elec2DetRelation();
    int addSimHeader(JM::SimHeader* header);
    // In one ElecEvent, each triggered PMT has one ElecFeeChannel readout, which comes from multiple PMT hits from multiple SimEvents
    void addHit(int PMTID, int eventID, int hitID, int type);
    std::vector<JM::SmartRef*> getAllHeaders() { return m_simheaders; }
    std::map<int, std::vector<Long64_t> > getAllHits() { return m_hitids; }

private:
    // Source of this ElecEvent
    std::vector<JM::SmartRef*>  m_simheaders;
    /*
        KEY: Triggered PMT IDs of ElecEvent
        VALUE: ID table of PMT hits source
            0-31 bit: hit id, 32-47 bit: event id
            48-51 bit: hit type
            52-63 bit: reserved
    */
    std::map<int, std::vector<Long64_t> > m_hitids;

    ClassDef(Elec2DetRelation,1);
};

#endif
