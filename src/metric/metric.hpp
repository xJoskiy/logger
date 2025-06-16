#pragma once

#include <sstream>
#include <string>

template <typename T>
concept Streamable = requires(T val, std::ostream os) { os << val; };

struct IMetric {
    std::string name_;
    virtual std::string ToString() const = 0;
    virtual ~IMetric() = default;
    virtual IMetric* Clone() const = 0;

    IMetric(std::string name) : name_(name) {}
};

template <Streamable T>
struct Metric : IMetric {
    T val_;

    Metric(T val, const std::string& name) : IMetric(name), val_(val) {}

    virtual std::string ToString() const override {
        std::ostringstream os;
        os << "\"" << name_ << "\" " << val_;
        return os.str();
    }

    virtual IMetric* Clone() const {
        return new Metric<T>(val_, name_);
    }
};
