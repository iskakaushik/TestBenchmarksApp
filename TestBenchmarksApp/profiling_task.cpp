//
//  profiling_task.cpp
//  TestBenchmarksApp
//
//  Created by Kaushik Iska on 4/9/20.
//  Copyright © 2020 Kaushik Iska. All rights reserved.
//

#include "profiling_task.hpp"

#include <iostream>

#include "profiling.h"

ProfilingTask::ProfilingTask() {
    log_handle_ = os_log_create("is.iska.profiling_queue", OS_LOG_CATEGORY_POINTS_OF_INTEREST);
    queue_ = dispatch_queue_create("is.iska.profiling_queue", DISPATCH_QUEUE_SERIAL);
    if (!queue_) {
        std::cerr << "Unable to create a dispatch queue. Profiler will not start." << std::endl;
    }
}

void ProfilingTask::Start(uint64_t sampling_frequency_micros, uint64_t leeway_micros) {
    if (!queue_) {
        return;
    }
    timer_ = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue_);
    if (!timer_) {
        std::cerr << "Unable to create timer. Profiler will not start." << std::endl;
        return;
    }
    const auto start_time = dispatch_walltime(NULL, 0);
    dispatch_source_set_timer(timer_, start_time, sampling_frequency_micros * NSEC_PER_USEC, leeway_micros * NSEC_PER_USEC);
    dispatch_source_set_event_handler(timer_, ^{
        const os_signpost_id_t identifier = os_signpost_id_generate(log_handle_);
        os_signpost_interval_begin(log_handle_, identifier, "CpuProfile");
        cpu_usage();
        os_signpost_interval_end(log_handle_, identifier, "CpuProfile");
    });
    dispatch_resume(timer_);
}
