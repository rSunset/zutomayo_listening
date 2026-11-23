//
// Created by miniroque on 22/11/26.
//

#include "logic/AlbumGenerator.hpp"
#include <random>

namespace
{
    constexpr int LOWER_ARRIVE_LIMIT = 0;
    constexpr int LOWER_DURATION_LIMIT = 1;
    constexpr int UPPER_ARRIVE_LIMIT = 15;
    constexpr int UPPER_DURATION_LIMIT = 10;
}

namespace zutomayo::logic
{
    std::vector<models::Album> generate_albums(const std::vector<std::string>& names, const std::uint32_t seed)
    {
        std::mt19937 generator(seed);
        std::uniform_int_distribution<int> arrivingGenerator(LOWER_ARRIVE_LIMIT, UPPER_ARRIVE_LIMIT);
        std::uniform_int_distribution<int> durationGenerator(LOWER_DURATION_LIMIT, UPPER_DURATION_LIMIT);
        std::vector<models::Album> albums;

        for (std::size_t id = 0; id < names.size(); ++id)
        {
            albums.emplace_back(models::Album {
                .id = static_cast<int>(id),
                .name = names[id],
                .releaseDate = arrivingGenerator(generator),
                .duration = durationGenerator(generator)
            });
        }
        if (!albums.empty())
            albums[0].releaseDate = 0;

        return albums;
    }
}
