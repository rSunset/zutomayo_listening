//
// Created by miniroque on 23/11/26.
//
#include <algorithm>
#include <utility>
#include <tuple>

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
            result.push_back(models::ExecutionSegment { .album_id = album.id, .start = start, .end = end });
            current_time = end;
        }

        return result;
    }

    std::vector<models::ExecutionSegment> simulate_spn(const std::vector<models::Album>& albums)
    {
        std::vector<models::Album> pending = albums;
        std::vector<models::ExecutionSegment> result;
        int current_time = 0;
        while (!pending.empty())
        {
            current_time = std::max(current_time, std::ranges::min_element(pending, {}, &models::Album::releaseDate)->releaseDate);
            const auto next = std::ranges::min_element(
                pending,std::ranges::less {},
                [current_time](const models::Album& a){
                    const bool not_ready = a.releaseDate > current_time;
                    return std::tuple{not_ready, a.duration, a.releaseDate, a.id};
            });

            result.push_back(models::ExecutionSegment { .album_id = next->id, .start = current_time, .end = current_time + next->duration });
            current_time = current_time + next->duration;
            pending.erase(next);
        }
        return result;
    }
}