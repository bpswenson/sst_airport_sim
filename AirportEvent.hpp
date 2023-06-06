#pragma once

#include "Common.hpp"

#include <sst/core/event.h>


namespace AirportSim {
enum class AirportEventType {
  PLANE_ARRIVED,
  PLANE_LANDED,
  PLANE_DISEMBARKED,
  PLANE_LOADED,
  PLANE_TAKEN_OFF,
};

class Airplane : public SST::Event {
public:
  Airplane(String name) : m_name(name) {}
  void set_passenger_count(uint32_t t) {
    m_count = t;
  }
  uint32_t get_passenger_count() { return m_count; }


  void set_status(AirportEventType aet) {
    m_status = aet;
  }

  AirportEventType get_status() const { 
    return m_status;
  }

  void serialize_order(SST::Core::Serialization::serializer &ser) override {
    Event::serialize_order(ser);
    ser &m_name;
    ser &m_count;
    ser &m_status;
  }

  Airplane *clone() override
  {
    return new Airplane(*this);
  }

  String to_string() const {
    StringStream ss;
    ss << "Flight " << m_name << " with " << m_count << " passengers";
    return ss.str();
  }

  ImplementSerializable(Airplane);

private:
  Airplane() {}
  String m_name;
  AirportEventType m_status;
  uint32_t m_count;
};

// For debugging
inline std::ostream &operator<<(std::ostream &os, const Airplane *const pse)
{
  os << pse->to_string();
  return os;
}

}  // namespace PaintShop
