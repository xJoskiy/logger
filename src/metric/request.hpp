#pragma once

#include <chrono>
#include <memory>

#include "metric/metric.hpp"

struct Request {
    using time_stamp = std::chrono::system_clock::time_point;
    std::vector<std::shared_ptr<IMetric>> metrics_;
    time_stamp time_;

    Request(std::vector<std::shared_ptr<IMetric>> metrics, time_stamp time)
        : metrics_(std::move(metrics)), time_(time) {};

    Request(const Request&) = delete;
    Request(Request&& rhs) {
        metrics_ = std::move(rhs.metrics_);
        time_ = std::move(rhs.time_);
    }
};
