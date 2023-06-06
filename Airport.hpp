
#pragma once

#include <sst/core/component.h>
#include <sst/core/link.h>
#include <sst/core/output.h>
#include <sst/core/params.h>
#include <sst/core/sst_types.h>
#include <random>
#include "Common.hpp"


namespace AirportSim {
class Airport : public SST::Component {
public:
  SST_ELI_REGISTER_COMPONENT(
      Airport, "AirportSim", "Airport", SST_ELI_ELEMENT_VERSION(1, 0, 1), "AirportSim", COMPONENT_CATEGORY_NETWORK)

  SST_ELI_DOCUMENT_PARAMS(
      {"name", "Airport Name", "Empty_Airport"},
      {"num_connectors", "Number of flight links to other airports", "0"},
      {"num_planes", "Number of initial flights out of airport", "0"},
      )


  SST_ELI_DOCUMENT_PORTS(
      {"airport_connector_%(num_connectors)d", "Airport Connection Flights", {""}})

  Airport(SST::ComponentId_t id, SST::Params &params);

  ~Airport() override;

protected:
  void finish() override;
  void complete(unsigned int) override;
  void handle_incoming_plane(SST::Event* ev, uint32_t idx);
  void handle_self_event(SST::Event *ev);
  bool clock(SST::Cycle_t cycle);
  void setup() override;
  SST::Clock::Handler<Airport> *m_clock;

private:
  uint32_t m_num_planes;
  std::random_device rd;
  std::mt19937 mt; 
  uint64_t m_clock_count;
  String m_name;
  uint32_t m_num_connectors;
  SST::Link *m_self_link;      //!< SST Link for sending delayed events to myself
  Vector<SST::Link*> m_connector_links;

};
}  // namespace AirportSim
