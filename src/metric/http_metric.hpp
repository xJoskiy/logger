#pragma once

#include <iomanip>
#include <sstream>

#include "metric.h"

struct HTTPMetric : public Metric<size_t> {
    HTTPMetric(const std::string& name, size_t val) : Metric(val, name) {};
};
