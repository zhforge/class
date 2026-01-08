#pragma once
#include <iostream>
#include <fstream>
#include <queue>
#include <thread>
#include <stop_token>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <format>
#include <string>
#include <chrono>

class OrderPipeline {
private:
    struct Order {
        const int id; // unique identifier number
        const float amount; // order's dollar amount
        const int region_code; // from 0-4 representing West, Central, East, North, or South respectively
    };
    
    std::queue<Order> RawOrderQueue, ValidatedOrderQueue;
    std::mutex ROQmtx, VOQmtx, OUTmtx, LOGmtx;
    std::condition_variable ROQempty, VOQempty, end;
    int gen_threads_done = 0;

    int RegionOrders[5] = {0, 0, 0, 0, 0};
    float RegionAmounts[5] = {0, 0, 0, 0, 0};
    int valid_orders = 0;
    int invalid_orders = 0;


    // thread functions
    void _thread_generator(const int thread_id) { // thread_id = unique auto-increment identifier number starting from 0
        // generates 200 random orders (each), pushes them to raw order queue
        // orders have a random amount between 10 and 500, random region code from 0-4

        for (int i = 1; i <= 200; i++) {
            Order o(i + (200*thread_id), 10 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(500-10))), std::rand() % 5);

            std::unique_lock<std::mutex> ROQlock(ROQmtx);
            RawOrderQueue.emplace(o);
            ROQlock.unlock();

            ROQempty.notify_one();

            // log
            std::lock_guard<std::mutex> LOGlock(LOGmtx);
            try {
                std::ofstream pipeline_log("pipeline.log", std::ios::app);
                if (pipeline_log.is_open()) {
                    pipeline_log << "[" << std::chrono::system_clock::now() << "]" << " Order generated from generator thread " << thread_id << " with ID #" << o.id << std::endl;
                }
            }
            catch (...) {
                std::lock_guard<std::mutex> OUTlock(OUTmtx);
                std::cerr << "Error logging a generated order" << std::endl;
            }
        }

        gen_threads_done++;
        // notify in case the generator threads finish last
        ROQempty.notify_all();
        VOQempty.notify_all();
    }

    void _thread_filter(std::stop_token st) {
        // takes an order from the raw order queue
        // verifies order's amount >= 50 and ID is positive
        // discards invalid orders, pushes valid orders to the validated order queue
        std::stop_callback stopCallback(st, [this] { ROQempty.notify_all(); });

        while (true) {
            std::unique_lock<std::mutex> ROQlock(ROQmtx);
            ROQempty.wait(ROQlock, [this, st] { return st.stop_requested() || !RawOrderQueue.empty(); });

            if (RawOrderQueue.empty() && st.stop_requested()) {
                return;
            }

            Order o = RawOrderQueue.front();
            RawOrderQueue.pop();

            ROQlock.unlock();
            ROQempty.notify_one();
            VOQempty.notify_all();

            if (o.amount >= 50 && o.id >= 1) {
                std::unique_lock<std::mutex> VOQlock(VOQmtx);
                ValidatedOrderQueue.emplace(std::move(o));
                VOQlock.unlock();
                valid_orders++;

                // log
                std::lock_guard<std::mutex> LOGlock(LOGmtx);
                try {
                    std::ofstream pipeline_log("pipeline.log", std::ios::app);
                    if (pipeline_log.is_open()) {
                        pipeline_log << "[" << std::chrono::system_clock::now() << "]" << "A filter thread validated order with ID #" << o.id << std::endl;
                    }
                }
                catch (...) {
                    std::lock_guard<std::mutex> OUTlock(OUTmtx);
                    std::cerr << "Error logging a filtered order" << std::endl;
                }
            } else {
                invalid_orders++;
            }
            end.notify_all();
        }
    }

    void _thread_router(std::stop_token st, const int thread_id) { // thread_id = unique region identifier number from 0-4
        // monitors the front of the validated order queue
        // takes an order only if its region code matches the thread's assigned region
        // writes matching order to respective regional warehouse file
        std::stop_callback stopCallback(st, [this] { VOQempty.notify_all(); });

        while (true) {
            std::unique_lock<std::mutex> VOQlock(VOQmtx);
            VOQempty.wait(VOQlock, [this, st, thread_id]{ return (ValidatedOrderQueue.empty() && st.stop_requested()) || (!ValidatedOrderQueue.empty() && ValidatedOrderQueue.front().region_code == thread_id); });

            if (ValidatedOrderQueue.empty() && st.stop_requested()) {
                return;
            }

            Order o = ValidatedOrderQueue.front();
            ValidatedOrderQueue.pop();

            VOQlock.unlock();
            VOQempty.notify_all();

            try {
                std::ofstream region_manifest(std::format("region_{}.txt", thread_id), std::ios::app);
                if (region_manifest.is_open()) {
                    region_manifest << o.id << "," << o.amount << std::endl;
                    region_manifest.close();
                }
                RegionOrders[thread_id]++;
                RegionAmounts[thread_id] += o.amount;

                // log
                std::lock_guard<std::mutex> LOGlock(LOGmtx);
                try {
                    std::ofstream pipeline_log("pipeline.log", std::ios::app);
                    if (pipeline_log.is_open()) {
                        pipeline_log << "[" << std::chrono::system_clock::now() << "]" << " Order routed from router thread " << thread_id << " with ID #" << o.id << std::endl;
                    }
                }
                catch (...) {
                    std::lock_guard<std::mutex> OUTlock(OUTmtx);
                    std::cerr << "Error logging a routed order" << std::endl;
                }
            }
            catch (...) {
                std::unique_lock<std::mutex> CERRlock(OUTmtx);
                std::cerr << "Error occurred writing to region " << thread_id << " manifest file" << std::endl;
                CERRlock.unlock();
                std::lock_guard<std::mutex> VOQlock(VOQmtx);
                ValidatedOrderQueue.emplace(std::move(o));
            }
            end.notify_all();
        }

    }

public:
    void StartOrderPipeline() {
        std::vector<std::jthread> GeneratorThreads, FilterThreads, RouterThreads;

        for (int i = 0; i < 5; i++) {
            std::jthread t(&OrderPipeline::_thread_generator, this, i);
            GeneratorThreads.emplace_back(std::move(t));
        }
        for (int i = 0; i < 4; i++) {
            std::jthread t(std::bind_front(&OrderPipeline::_thread_filter, this));
            FilterThreads.emplace_back(std::move(t));
            // bind_front is necessary because jthreads don't like initializing with a class member function with a stop_token param
        }
        for (int i = 0; i < 5; i++) {
            std::jthread t(std::bind_front(&OrderPipeline::_thread_router, this), i);
            RouterThreads.emplace_back(std::move(t));
        }

        std::unique_lock<std::mutex> COUTlock(OUTmtx);
        end.wait(COUTlock, [this]{ return gen_threads_done == 5 && RawOrderQueue.empty() && ValidatedOrderQueue.empty(); });

        std::cout << "Order Processing Pipeline Results" << std::endl
                  << "==================================" << std::endl
                  << std::format("Region 0 (West): {} orders, Total Revenue: ${:.2f}", RegionOrders[0], RegionAmounts[0]) << std::endl
                  << std::format("Region 1 (Central): {} orders, Total Revenue: ${:.2f}", RegionOrders[1], RegionAmounts[1]) << std::endl
                  << std::format("Region 2 (East): {} orders, Total Revenue: ${:.2f}", RegionOrders[2], RegionAmounts[2]) << std::endl
                  << std::format("Region 3 (North): {} orders, Total Revenue: ${:.2f}", RegionOrders[3], RegionAmounts[3]) << std::endl
                  << std::format("Region 4 (South): {} orders, Total Revenue: ${:.2f}", RegionOrders[4], RegionAmounts[4]) << std::endl
                  << std::endl
                  << "Total Valid Orders: " << valid_orders << std::endl
                  << "Total Invalid Orders: " << invalid_orders << std::endl
                  << "Pipeline processing completed successfully." << std::endl;
        COUTlock.unlock();

        // log
        std::lock_guard<std::mutex> LOGlock(LOGmtx);
        try {
            std::ofstream pipeline_log("pipeline.log", std::ios::app);
            if (pipeline_log.is_open()) {
                pipeline_log << "[" << std::chrono::system_clock::now() << "]" << " Order pipeline successfully completed" << std::endl;
            }
        }
        catch (...) {
            std::lock_guard<std::mutex> OUTlock(OUTmtx);
            std::cerr << "Error logging pipeline completion" << std::endl;
        }
    }

};