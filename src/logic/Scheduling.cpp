//
// Created by miniroque on 23/11/26.
//
#include <algorithm>
#include <utility>

#include "logic/Scheduling.hpp"

namespace zutomayo::logic
{
    std::vector<models::ExecutionSegment> simulate_fcfs(const std::vector<models::Album>& albums)
    {
        std::vector<models::Album> ordered = albums;
        std::ranges::sort(ordered, std::ranges::less{}, [](const models::Album& a)
        {
            return std::pair{a.releaseDate, a.id};
        });

        std::vector<models::ExecutionSegment> result;
        int current_time = 0;

        for (const models::Album& album : ordered)
        {
            const int start = std::max(current_time, album.releaseDate);
            const int end = start + album.duration;
            result.push_back(models::ExecutionSegment { .album_id = album.id, .start = start, .end = end});
            current_time = end;
        }

        return result;
    }
}