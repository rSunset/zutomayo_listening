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
        int arrival_time{0};
        int duration{0};
    };
}

