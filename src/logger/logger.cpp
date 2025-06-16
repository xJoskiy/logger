#include "logger.hpp"

#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <thread>

#include "metric/metric.hpp"
#include "metric/request.hpp"

Logger::Logger(std::string output_file) : file_(output_file, std::ios_base::app) {
    auto main_f = [this](std::stop_token stop_token) {
        auto f = [this, stop_token]() { return !queue_.empty() || stop_token.stop_requested(); };

        while (!stop_token.stop_requested()) {
            std::unique_lock lock(mtx_);
            cv_.wait(lock, f);
            if (!queue_.empty()) {
                auto task = std::move(queue_.front());
                log(std::move(task));
                queue_.pop();
            }
        }
    };

    main_thread_ = std::jthread(main_f);
};

void Logger::log(std::vector<std::shared_ptr<IMetric>> metrics) {
    auto log_f = [this](Request request) {
        std::lock_guard lock(mtx_);
        queue_.emplace(std::move(request));
        cv_.notify_one();
    };

    // Run queuing in a separate thread in order to not stop the initial logging thread
    Request request{std::move(metrics), std::chrono::system_clock::now()};

    std::thread log_t = std::thread(log_f, std::move(request));
    log_t.detach();
}

Logger::~Logger() {
    main_thread_.request_stop();
    cv_.notify_all();
}

void Logger::log(Request task) {
    file_ << task.time_ << " ";
    for (const auto& metric : task.metrics_) file_ << metric->ToString() << " ";
    file_ << std::endl;
}