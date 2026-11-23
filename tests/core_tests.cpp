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
}

int main()
{
    same_seed_gives_same_albums();
    empty_names_gives_empty_albums();
    values_stay_in_range_and_reach_the_extremes();

    std::cout << "All tests passed\n";
    return 0;
}