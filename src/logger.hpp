#pragma once

#include <condition_variable>
#include <fstream>
#include <memory>
#include <queue>
#include <thread>

#include "metric/metric.h"
#include "request.h"

class Logger {
    std::queue<Request> queue_;
    std::condition_variable cv_;
    std::mutex mtx_;
    std::fstream file_;
    std::jthread main_thread_;

    void log(Request task);

public:
    Logger(std::string output_file);

    void log(std::vector<std::shared_ptr<IMetric>> metric);

    ~Logger();
};
