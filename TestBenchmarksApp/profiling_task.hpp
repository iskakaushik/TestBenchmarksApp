//
//  profiling_task.hpp
//  TestBenchmarksApp
//
//  Created by Kaushik Iska on 4/9/20.
//  Copyright © 2020 Kaushik Iska. All rights reserved.
//

#ifndef profiling_task_hpp
#define profiling_task_hpp

#include <dispatch/dispatch.h>
#include <os/signpost.h>
#include <stdio.h>
#include <thread>

// Starts profiling tasks.
class ProfilingTask {
public:
    ProfilingTask();
    ~ProfilingTask() = default;
    void Start(uint64_t sampling_frequency_micros, uint64_t leeway_micros);
    
private:
    os_log_t log_handle_;
    dispatch_queue_t queue_;
    dispatch_source_t timer_;
};

#endif /* profiling_task_hpp */
