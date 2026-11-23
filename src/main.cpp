#include <iostream>
#include "models/Album.hpp"
#include "models/SchedulingTypes.hpp"
#include "logic/AlbumGenerator.hpp"

int main()
{
    zutomayo::models::Album albumTest{.id = 0, .name= "Jinkougaku", .releaseDate = 0, .duration = 3};
    zutomayo::models::SchedulingAlgorithm scheduling = zutomayo::models::SchedulingAlgorithm::FCFS;

    std::vector<zutomayo::models::Album> albums = zutomayo::logic::generate_albums(
        std::vector<std::string> {
            "Hisohiso Banashi",
            "Gusare",
            "Jinkougaku",
            "Meiko wa Gunaruga Gotoshi",
            "Keisoudo"
        },
        42
    );

    std::printf("%-5s%-40s%-15s%s\n", "ID", "Name", "ReleaseDate", "Duration");
    for (const auto& album : albums)
    {
        std::printf("%-5d%-40s%-15d%-5d\n", album.id, album.name.c_str(), album.releaseDate, album.duration);
    }

    std::cout << albumTest.name << '\n';
    std::cout << static_cast<int>(scheduling) << '\n';

    albums = zutomayo::logic::generate_albums(
        std::vector<std::string> {
            "Hisohiso Banashi",
            "Gusare",
            "Jinkougaku",
            "Meiko wa Gunaruga Gotoshi",
            "Keisoudo"
        },
        42
    );

    std::printf("%-5s%-40s%-15s%s\n", "ID", "Name", "ReleaseDate", "Duration");
    for (const auto& album : albums)
    {
        std::printf("%-5d%-40s%-15d%-5d\n", album.id, album.name.c_str(), album.releaseDate, album.duration);
    }

    return 0;
}