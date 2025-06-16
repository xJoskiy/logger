#pragma once

#include <iomanip>
#include <sstream>

#include "metric/metric.hpp"

struct HTTPMetric : public Metric<size_t> {
    HTTPMetric(const std::string& name, size_t val) : Metric(val, name) {};
};
