#include "Airport.hpp"
#include "AirportEvent.hpp"

#include <sstream>

namespace AirportSim {

Airport::Airport(SST::ComponentId_t id, SST::Params &params) : SST::Component(id), mt(rd())
{
  m_name = params.find<String>("name", "Empty Airport");

  m_self_link = configureSelfLink(
      "linkToSelf", new SST::Event::Handler<Airport>(this, &Airport::handle_self_event));

  m_num_connectors = params.find<uint32_t>("num_connectors", 0);
  m_num_planes = params.find<uint32_t>("num_planes", 0);

  m_connector_links.resize(m_num_connectors);
  for(uint32_t i = 0; i < m_num_connectors; ++i) {
    std::stringstream name;
    name << "airport_connector_" << i;
    m_connector_links[i] = configureLink(name.str(), new SST::Event::Handler<Airport, uint32_t>(this, &Airport::handle_incoming_plane, i));
  }

  m_clock = new SST::Clock::Handler<Airport>(this, &Airport::clock);
  registerClock("1Hz", m_clock);

  registerAsPrimaryComponent();
  primaryComponentDoNotEndSim();
}
void Airport::handle_incoming_plane(SST::Event* ev, uint32_t idx) {
  Airplane* ap = (Airplane*)ev;
  std::cout << getCurrentSimTime() << ": ARRIVED IN " << m_name << " AIRSPACE " << ap << std::endl;
  ap->set_status(AirportEventType::PLANE_LANDED);
  m_self_link->send(10, ap);
}
void Airport::handle_self_event(SST::Event *ev) {
  std::uniform_int_distribution<uint32_t> dist(1, 50);
  uint32_t delay = dist(mt);
  Airplane* ap = (Airplane*)ev;
  AirportEventType et = ap->get_status();
  switch(et) {
    case AirportEventType::PLANE_LANDED:
      std::cout << getCurrentSimTime() << ": LANDED " << ap << std::endl;
      ap->set_status(AirportEventType::PLANE_DISEMBARKED);
      m_self_link->send(delay, ap);
      break;
    case AirportEventType::PLANE_DISEMBARKED:
      std::cout << getCurrentSimTime() << ": DISEMBARKED " << ap << std::endl;
      ap->set_status(AirportEventType::PLANE_LOADED);
      m_self_link->send(delay, ap);
      break;
    case AirportEventType::PLANE_LOADED:
      std::cout << getCurrentSimTime() << ": PLANE LOADED " << ap << std::endl;
      ap->set_status(AirportEventType::PLANE_TAKEN_OFF);
      m_self_link->send(delay, ap);
      break;
    case AirportEventType::PLANE_TAKEN_OFF:
      std::cout << getCurrentSimTime() << ": PLANE TAKEN OFF " << ap << std::endl;
      ap->set_status(AirportEventType::PLANE_TAKEN_OFF);
      m_connector_links[0]->send(delay, ap);
      break;
    default:
      std::cout << "Invalid event type";
      break;
  }

}
void Airport::complete(unsigned int) {
}

bool Airport::clock(SST::Cycle_t cycle) {
  if(cycle == 100) {
    primaryComponentOKToEndSim();
    return true;
  }
  return false;
}

void Airport::setup() {
  std::uniform_int_distribution<uint32_t> destination(0, m_num_connectors - 1);
  std::uniform_int_distribution<uint32_t> delay(5, 50);
  std::uniform_int_distribution<uint32_t> passenger_count(1, 200);
  for(uint32_t i = 0; i < m_num_planes; ++i) {
    std::stringstream ss;
    ss << m_name << "_" << i;
    Airplane* plane = new Airplane(ss.str());
    plane->set_passenger_count(passenger_count(mt));
    m_connector_links[destination(mt)]->send(delay(mt), plane); 
  }
}

void Airport::finish()
{

}
Airport::~Airport()
{
  
}
}  // namespace PaintShop
