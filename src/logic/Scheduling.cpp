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
            const auto earliest = std::ranges::min_element(pending, {}, &models::Album::releaseDate);
            current_time = std::max(current_time, earliest->releaseDate);
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

    std::vector<models::ExecutionSegment> simulate_hrrn(const std::vector<models::Album>& albums)
    {
        std::vector<models::Album> pending = albums;
        std::vector<models::ExecutionSegment> result;
        int current_time = 0;
        while (!pending.empty())
        {
            const auto earliest = std::ranges::min_element(pending, {}, &models::Album::releaseDate);
            current_time = std::max(current_time, earliest->releaseDate);
            const auto next = std::ranges::min_element(
                pending,
                [current_time](const models::Album& a, const models::Album& b){
                    const int a_wait = current_time - a.releaseDate;
                    const int b_wait = current_time - b.releaseDate;

                    const int a_score = (a_wait + a.duration) * b.duration;
                    const int b_score = (b_wait + b.duration) * a.duration;

                    if (a_score != b_score)
                        return a_score > b_score;
                    if (a.releaseDate != b.releaseDate)
                        return a.releaseDate < b.releaseDate;
                    return a.id < b.id;
            });

            result.push_back(models::ExecutionSegment { .album_id = next->id, .start = current_time, .end = current_time + next->duration });
            current_time = current_time + next->duration;
            pending.erase(next);
        }
        return result;
    }

    std::vector<models::ExecutionSegment> simulate_srt(const std::vector<models::Album>&)
    {
        return {};   // TODO: implementar SRT
    }
}