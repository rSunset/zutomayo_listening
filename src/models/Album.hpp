//
// Created by miniroque on 22/11/26.
//
#pragma once
#include <string>

namespace zutomayo::models
{
    struct Album
    {
        int id{0};
        std::string name;
        int releaseDate{0};
        int duration{0};
        bool operator==(const Album&) const = default;
    };
}

