#include <iostream>
#include "models/Album.hpp"
#include "models/SchedulingTypes.hpp"

int main()
{
    zutomayo::models::Album album = zutomayo::models::Album();
    album.name = "Jinkougaku";

    zutomayo::models::SchedulingAlgorithm scheduling = zutomayo::models::SchedulingAlgorithm::FCFS;

    std::cout << album.name << std::endl;
    std::cout << static_cast<int>(scheduling) << std::endl;

    return 0;
}