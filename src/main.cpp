#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <vector>

#include "logger.h"
#include "metric/all_metrics.h"

void worker(Logger& logger) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution dist(1, 100);

    for (int i = 0; i < 100; i++) {
        auto http = std::make_shared<HTTPMetric>("HTTP RPS", dist(mt));
        auto cpu = std::make_shared<CPUMetric>("CPU", dist(mt), 8);

        logger.log({cpu, http});
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void work(Logger& logger) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution dist(1, 100);

    auto http = std::make_shared<HTTPMetric>("HTTP RPS", dist(mt));
    auto cpu = std::make_shared<CPUMetric>("CPU", 1.266, dist(mt));
    std::vector<std::shared_ptr<IMetric>> metrics = {http, cpu};
    logger.log(std::move(metrics));
}

int main() {
    std::string output = "log.txt";
    Logger logger(output);
    std::thread t(worker, std::ref(logger));

    // Guarantee that logger remains alive when threads try to log anything
    t.join();

    return 0;
}
