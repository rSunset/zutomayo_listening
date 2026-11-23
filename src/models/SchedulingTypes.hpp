//
// Created by miniroque on 22/11/26.
//
#pragma once

namespace zutomayo::models
{
    enum class SchedulingAlgorithm
    {
        FCFS,
        SPN,
        SRT,
        HRRN
    };

    struct ExecutionSegment
    {
        int album_id {0};
        int start {0};
        int end {0};

        bool operator == (const ExecutionSegment&) const = default;
    };
}

