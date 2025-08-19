#pragma once

#include "pros/distance.hpp"
#include "au/au.hpp"

// distance.hpp

// this is for MCL when we get to it

namespace dlib {

struct DistanceConfig {
    int8_t port;

    DistanceConfig(int8_t port);
};

class Distance {
public:
    void initialize();
    
    au::Quantity<au::Milli<au::Meters>, double> get_distance();

    Distance(DistanceConfig config);

    pros::Distance raw;
};

}