//
// Created by miniroque on 24/11/26.
//
#pragma once
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "models/Album.hpp"
#include "models/SchedulingTypes.hpp"
#include "interface/CoverLibrary.hpp"

namespace zutomayo::ui
{
    constexpr float GANTT_BLOCK_HEIGHT = 170.F;

    struct Button
    {
        sf::FloatRect bounds;
        std::string label;
    };

    struct GanttChart
    {
        std::string title;
        std::vector<models::ExecutionSegment> segments;
    };

    void draw_button(sf::RenderTarget& target, const sf::Font& font, const Button& button);

    void draw_album_table(sf::RenderTarget& target, const sf::Font& font,
                          const std::vector<models::Album>& albums, sf::Vector2f origin);

    void draw_gantt(sf::RenderTarget& target, const sf::Font& font, const GanttChart& chart,
                    const std::vector<models::Album>& albums, sf::Vector2f origin, int total_time);

    void draw_album_table(sf::RenderTarget& target, const sf::Font& font,
                      const std::vector<models::Album>& albums, const CoverLibrary& covers,
                      sf::Vector2f origin);
}
