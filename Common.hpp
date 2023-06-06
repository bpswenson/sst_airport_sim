#pragma once
#include <sst/core/event.h>
#include <sst/core/output.h>
#include <sst/core/params.h>
#include <sst/core/sst_types.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


#ifndef NDEBUG
#define ASSERT(condition, message)                                                                                \
  do {                                                                                                            \
    if (!(condition)) {                                                                                           \
      std::cerr << "Assertion `" #condition "` failed in " << __FILE__ << " line " << __LINE__ << ": " << message \
                << std::endl;                                                                                     \
      std::terminate();                                                                                           \
    }                                                                                                             \
  } while (false)
#else
#define ASSERT(condition, message) \
  do {                             \
  } while (false)
#endif

namespace AirportSim {

template <typename X, typename Y>
using Pair = std::pair<X, Y>;

template <typename T>
using Vector = std::vector<T>;

using String = std::string;

using StringStream = std::stringstream;

template <typename KEY, typename VALUE>
using Map = std::map<KEY, VALUE>;

template <typename KEY, typename VALUE>
using HashMap = std::unordered_map<KEY, VALUE>;

template <typename T>
using Sptr = std::shared_ptr<T>;

}  // namespace AirportSim
