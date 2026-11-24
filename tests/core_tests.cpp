#undef NDEBUG
#include <cassert>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "logic/AlbumGenerator.hpp"
#include "logic/Scheduling.hpp"
#include "models/SchedulingTypes.hpp"

namespace
{
    std::vector<std::string> make_names()
    {
        return {"A", "B", "C", "D", "E"};
    }

    void same_seed_gives_same_albums()
    {
        const auto names = make_names();
        const auto first = zutomayo::logic::generate_albums(names, 42);
        const auto second = zutomayo::logic::generate_albums(names, 42);

        assert(first == second);
    }

    void empty_names_gives_empty_albums()
    {
        const auto albums = zutomayo::logic::generate_albums({}, 1);

        assert(albums.empty());
    }

    void values_stay_in_range_and_reach_the_extremes()
    {
        const auto names = make_names();

        int min_arrival = std::numeric_limits<int>::max();
        int max_arrival = std::numeric_limits<int>::min();
        int min_duration = std::numeric_limits<int>::max();
        int max_duration = std::numeric_limits<int>::min();

        for (std::uint32_t seed = 0; seed < 1000; ++seed)
        {
            const auto albums = zutomayo::logic::generate_albums(names, seed);

            assert(albums.size() == names.size());
            assert(albums[0].releaseDate == 0);

            for (std::size_t i = 0; i < albums.size(); ++i)
            {
                assert(albums[i].id == static_cast<int>(i));
                assert(albums[i].name == names[i]);
                assert(albums[i].releaseDate >= 0 && albums[i].releaseDate <= 15);
                assert(albums[i].duration >= 1 && albums[i].duration <= 10);

                if (i > 0)
                {
                    min_arrival = std::min(min_arrival, albums[i].releaseDate);
                    max_arrival = std::max(max_arrival, albums[i].releaseDate);
                }
                min_duration = std::min(min_duration, albums[i].duration);
                max_duration = std::max(max_duration, albums[i].duration);
            }
        }

        assert(min_arrival == 0);
        assert(max_arrival == 15);
        assert(min_duration == 1);
        assert(max_duration == 10);
    }

    using zutomayo::models::Album;
    using zutomayo::models::ExecutionSegment;

    void fcfs_runs_in_arrival_order_and_leaves_idle_gaps()
    {
        const std::vector<Album> albums{
            {.id = 0, .name = "A", .releaseDate = 0,  .duration = 3},
            {.id = 1, .name = "B", .releaseDate = 1,  .duration = 2},
            {.id = 2, .name = "C", .releaseDate = 10, .duration = 1},
        };
        const std::vector<ExecutionSegment> expected{
            {.album_id = 0, .start = 0,  .end = 3},
            {.album_id = 1, .start = 3,  .end = 5},
            {.album_id = 2, .start = 10, .end = 11},
        };

        assert(zutomayo::logic::simulate_fcfs(albums) == expected);
    }

    void fcfs_ignores_input_order_and_breaks_ties_by_id()
    {
        const std::vector<Album> albums{
            {.id = 0, .name = "A", .releaseDate = 0, .duration = 2},
            {.id = 2, .name = "C", .releaseDate = 4, .duration = 1},
            {.id = 1, .name = "B", .releaseDate = 4, .duration = 3},
        };
        const std::vector<ExecutionSegment> expected{
            {.album_id = 0, .start = 0, .end = 2},
            {.album_id = 1, .start = 4, .end = 7},
            {.album_id = 2, .start = 7, .end = 8},
        };

        assert(zutomayo::logic::simulate_fcfs(albums) == expected);
    }

    void fcfs_with_no_albums_gives_no_segments()
    {
        assert(zutomayo::logic::simulate_fcfs({}).empty());
    }

    void spn_does_not_preempt_and_picks_shortest_ready_album()
    {
        const std::vector<Album> albums{
            {.id = 0, .name = "A", .releaseDate = 0, .duration = 7},
            {.id = 1, .name = "B", .releaseDate = 2, .duration = 4},
            {.id = 2, .name = "C", .releaseDate = 4, .duration = 1},
            {.id = 3, .name = "D", .releaseDate = 5, .duration = 4},
        };
        const std::vector<ExecutionSegment> expected{
            {.album_id = 0, .start = 0,  .end = 7},
            {.album_id = 2, .start = 7,  .end = 8},
            {.album_id = 1, .start = 8,  .end = 12},
            {.album_id = 3, .start = 12, .end = 16},
        };

        assert(zutomayo::logic::simulate_spn(albums) == expected);
    }

    void spn_waits_for_arrivals_and_breaks_ties_by_id()
    {
        const std::vector<Album> albums{
            {.id = 0, .name = "A", .releaseDate = 0, .duration = 1},
            {.id = 2, .name = "C", .releaseDate = 3, .duration = 2},
            {.id = 1, .name = "B", .releaseDate = 3, .duration = 2},
        };
        const std::vector<ExecutionSegment> expected{
            {.album_id = 0, .start = 0, .end = 1},
            {.album_id = 1, .start = 3, .end = 5},
            {.album_id = 2, .start = 5, .end = 7},
        };

        assert(zutomayo::logic::simulate_spn(albums) == expected);
    }

    void spn_with_no_albums_gives_no_segments()
    {
        assert(zutomayo::logic::simulate_spn({}).empty());
    }
}

int main()
{
    same_seed_gives_same_albums();
    empty_names_gives_empty_albums();
    values_stay_in_range_and_reach_the_extremes();
    fcfs_runs_in_arrival_order_and_leaves_idle_gaps();
    fcfs_ignores_input_order_and_breaks_ties_by_id();
    fcfs_with_no_albums_gives_no_segments();
    spn_does_not_preempt_and_picks_shortest_ready_album();
    spn_waits_for_arrivals_and_breaks_ties_by_id();
    spn_with_no_albums_gives_no_segments();

    std::cout << "All tests passed\n";
    return 0;
}