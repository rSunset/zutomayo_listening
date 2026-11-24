//
// Created by miniroque on 23/11/26.
//
#pragma once
#include <vector>

#include "models/SchedulingTypes.hpp"
#include "models/Album.hpp"

namespace zutomayo::logic
{
    std::vector<models::ExecutionSegment> simulate_fcfs(const std::vector<models::Album>& albums);
    std::vector<models::ExecutionSegment> simulate_spn(const std::vector<models::Album>& albums);
    std::vector<models::ExecutionSegment> simulate_hrrn(const std::vector<models::Album>& albums);
    std::vector<models::ExecutionSegment> simulate_srt(const std::vector<models::Album>& albums);
}
