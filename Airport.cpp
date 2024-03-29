#include "Airport.hpp"
#include "AirportEvent.hpp"

#include <sstream>

namespace AirportSim {

Airport::Airport(SST::ComponentId_t id, SST::Params& params)
    : SST::Component(id),
      mt(rd()),
      m_total_passenger_departures(0),
      m_total_aircraft_departures(0),
      m_total_aircraft_arrivals(0),
      m_total_passenger_arrivals(0)
{
  m_name = params.find<String>("name", "Empty Airport");

  m_self_link = configureSelfLink("linkToSelf", new SST::Event::Handler<Airport>(this, &Airport::handle_self_event));

  m_num_connectors = params.find<uint32_t>("num_connectors", 0);
  m_num_planes = params.find<uint32_t>("num_planes", 0);

  m_connector_links.resize(m_num_connectors);
  for (uint32_t i = 0; i < m_num_connectors; ++i) {
    std::stringstream name;
    name << "airport_connector_" << i;
    // 50% of planes that depart will crash
    m_connector_links[i] =
        new RoughAirLink(configureLink(name.str(), new SST::Event::Handler<Airport, uint32_t>(this, &Airport::handle_incoming_plane, i)), .5);
  }

  // TODO: Make configurable
  destination_dist = std::uniform_int_distribution<uint32_t>(0, m_num_connectors - 1);
  delay_dist = std::uniform_int_distribution<uint32_t>(10, 50);
  passenger_count_dist = std::uniform_int_distribution<uint32_t>(50, 200);

  m_clock = new SST::Clock::Handler<Airport>(this, &Airport::clock);
  registerClock("1Hz", m_clock);

  registerAsPrimaryComponent();
  primaryComponentDoNotEndSim();
}
void Airport::handle_incoming_plane(SST::Event* ev, uint32_t idx)
{
  Airplane* ap = (Airplane*)ev;
  std::cout << getCurrentSimTime() << ": ENTERING AIRSPACE : " << m_name << " : " << ap << std::endl;
  ap->set_status(AirportEventType::PLANE_LANDED);
  m_self_link->send(10, ap);
}
void Airport::handle_self_event(SST::Event* ev)
{
  uint32_t delay = delay_dist(mt);
  Airplane* ap = (Airplane*)ev;
  AirportEventType et = ap->get_status();
  switch (et) {
    case AirportEventType::PLANE_LANDED:
      m_total_aircraft_arrivals++;
      m_total_passenger_arrivals += ap->get_passenger_count();
      std::cout << getCurrentSimTime() << ": LANDED : " << m_name << " : " << ap << std::endl;
      ap->set_status(AirportEventType::PLANE_DISEMBARKED);
      m_self_link->send(delay, ap);
      break;
    case AirportEventType::PLANE_DISEMBARKED:
      std::cout << getCurrentSimTime() << ": DISEMBARKED : " << m_name << " : " << ap << std::endl;
      ap->set_status(AirportEventType::PLANE_LOADED);
      m_self_link->send(delay, ap);
      break;
    case AirportEventType::PLANE_LOADED:
      ap->set_passenger_count(passenger_count_dist(mt));
      std::cout << getCurrentSimTime() << ": PLANE LOADED : " << m_name << " : " << ap << std::endl;
      ap->set_status(AirportEventType::PLANE_TAKEN_OFF);
      m_self_link->send(delay, ap);
      break;
    case AirportEventType::PLANE_TAKEN_OFF:
      std::cout << getCurrentSimTime() << ": PLANE TAKEN OFF : " << ap << std::endl;
      m_total_passenger_departures += ap->get_passenger_count();
      m_total_aircraft_departures++;
      ap->set_status(AirportEventType::PLANE_TAKEN_OFF);
      m_connector_links[destination_dist(mt)]->send(delay, ap);
      break;
    default:
      std::cout << "Invalid event type";
      break;
  }
}
void Airport::complete(unsigned int)
{
  /*
  uint32_t m_total_passenger_departures;
  uint32_t m_total_aircraft_departures;
  uint32_t m_total_aircraft_arrivals;
  uint32_t m_total_passenger_arrivals;
*/
  StringStream ss;
  ss << "STATS FOR " << m_name << "\n\tTotal Aircraft Departures: " << m_total_aircraft_departures
     << "\n\tTotal Aircraft Arrivals: " << m_total_aircraft_arrivals
     << "\n\tTotal Passenger Departures: " << m_total_passenger_departures
     << "\n\tTotal Passenger Arrivals: " << m_total_passenger_arrivals;
  std::cout << ss.str() << std::endl;
}

bool Airport::clock(SST::Cycle_t cycle)
{
  // if (cycle == 50000000) {
   if (cycle == 500) {
    primaryComponentOKToEndSim();
    return true;
  }
  return false;
}

void Airport::setup()
{
  std::uniform_int_distribution<uint32_t> destination(0, m_num_connectors - 1);
  std::uniform_int_distribution<uint32_t> delay(5, 50);
  std::uniform_int_distribution<uint32_t> passenger_count(5, 200);
  for (uint32_t i = 0; i < m_num_planes; ++i) {
    std::stringstream ss;
    ss << m_name << "_" << i;
    Airplane* plane = new Airplane(ss.str());
    plane->set_status(AirportEventType::PLANE_LOADED);
    m_self_link->send(delay(mt), plane);
  }
}

void Airport::finish()
{
}
Airport::~Airport()
{
}
}  // namespace AirportSim
