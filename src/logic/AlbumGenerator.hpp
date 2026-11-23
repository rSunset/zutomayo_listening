//
// Created by miniroque on 22/11/26.
//
#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "models/Album.hpp"

namespace zutomayo::logic
{
    std::vector<models::Album> generate_albums(const std::vector<std::string>& names, std::uint32_t seed);
}

