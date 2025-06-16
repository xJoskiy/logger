#include <iomanip>
#include <sstream>
#include <string>

#include "metric/metric.hpp"

struct CPUMetric : public Metric<double> {
    size_t precision_;
    size_t cores_count_;

    CPUMetric(const std::string& name, double val, size_t cores_count, size_t precision = 3)
        : Metric(val, name), cores_count_(cores_count), precision_(precision) {}

    std::string ToString() const override {
        std::ostringstream os;
        os << "\"" << name_ << std::setprecision(precision_) << "\" " << val_;
        return os.str();
    }
};
