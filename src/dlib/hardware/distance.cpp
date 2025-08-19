#include "dlib/hardware/distance.hpp"
#include "au/au.hpp"

// distance.cpp

namespace dlib {

DistanceConfig::DistanceConfig(
    int8_t port
) : 
    port(port) {

}

Distance::Distance(DistanceConfig config) : raw(config.port) {}

au::Quantity<au::Milli<au::Meters>,double> Distance::get_distance(){
    return au::milli(au::meters)(this->raw.get_distance());
}

}