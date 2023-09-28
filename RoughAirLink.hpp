#include <sst/core/link.h>
#include <random>
#include "AirportEvent.hpp"

// The not so friendly skies

namespace AirportSim {
class RoughAirLink {
public:
    RoughAirLink(SST::Link* l, double pCrash = 0.0) : m_l(l), m_pcrash(pCrash), m_gen(m_rd()) {
        dist = std::uniform_real_distribution<double>(0.0, 1.0);
    }
    void send(SST::SimTime_t delay, SST::Event* event) {
        if(dist(m_gen) < m_pcrash) {
            std::cout << "**BOOM**: " << (Airplane*)event << " has crashed :( " << std::endl;
            delete event;   // sadly the plane won't make the destination
        }
        else {
            m_l->send(delay, event);  // we live to fly again, send the airplane event
        }
    }

private:
    SST::Link* m_l;
    double m_pcrash;
    std::random_device m_rd;  //seed source for rng
    std::mt19937 m_gen; //mt engine seeded with m_rd
    std::uniform_real_distribution<> dist;
};
}
