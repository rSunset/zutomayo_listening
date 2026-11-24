//
// Created by miniroque on 24/11/26.
//
#pragma once
#include <string>

namespace zutomayo::ui
{
    inline std::string album_label(const int album_id)
    {
        return std::string(1, static_cast<char>('A' + album_id));
    }
}