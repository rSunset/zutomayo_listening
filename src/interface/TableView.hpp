//
// Created by miniroque on 24/11/26.
//
#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "models/Album.hpp"

namespace zutomayo::ui
{
    void draw_album_table(sf::RenderWindow& window, const sf::Font& font, const std::vector<models::Album>& albums, sf::Vector2f origin);
}
