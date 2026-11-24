//
// Created by miniroque on 24/11/26.
//
#include "interface/Drawing.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <string>

#include "interface/AlbumLabel.hpp"

namespace zutomayo::ui
{
    namespace
    {
        constexpr unsigned int TEXT_SIZE = 16;
        constexpr unsigned int SMALL_TEXT_SIZE = 12;
        constexpr float ROW_HEIGHT = 24.F;

        constexpr float COLUMN_LABEL = 0.F;
        constexpr float COLUMN_TITLE = 50.F;
        constexpr float COLUMN_RELEASE = 330.F;
        constexpr float COLUMN_DURATION = 450.F;

        constexpr float GANTT_LABEL_WIDTH = 30.F;
        constexpr float GANTT_PLOT_WIDTH = 600.F;
        constexpr float GANTT_TITLE_HEIGHT = 26.F;
        constexpr float GANTT_ROW_HEIGHT = 22.F;
        constexpr int AXIS_LABEL_EVERY = 5;

        const sf::Color HEADER_COLOR(150, 200, 255);
        const sf::Color AXIS_COLOR(160, 160, 170);
        const sf::Color ROW_COLOR(38, 38, 52);
        const sf::Color BUTTON_COLOR(70, 90, 160);

        sf::Color album_color(const int album_id)
        {
            const std::array<sf::Color, 5> palette{
                sf::Color(236, 95, 103), sf::Color(250, 200, 90), sf::Color(153, 199, 148),
                sf::Color(102, 153, 204), sf::Color(198, 148, 198)};
            return palette[static_cast<std::size_t>(album_id) % palette.size()];
        }

        void draw_text(sf::RenderTarget& target, const sf::Font& font, const std::string& content,
                       const sf::Vector2f position, const sf::Color color = sf::Color::White,
                       const unsigned int size = TEXT_SIZE)
        {
            sf::Text text(content, font, size);
            text.setFillColor(color);
            text.setPosition(position);
            target.draw(text);
        }

        void draw_rect(sf::RenderTarget& target, const sf::FloatRect& area, const sf::Color color)
        {
            sf::RectangleShape shape(sf::Vector2f(area.width, area.height));
            shape.setPosition(area.left, area.top);
            shape.setFillColor(color);
            target.draw(shape);
        }
    }

    void draw_button(sf::RenderTarget& target, const sf::Font& font, const Button& button)
    {
        draw_rect(target, button.bounds, BUTTON_COLOR);
        draw_text(target, font, button.label, {button.bounds.left + 16.F, button.bounds.top + 7.F});
    }

    void draw_album_table(sf::RenderTarget& target, const sf::Font& font,
                          const std::vector<models::Album>& albums, const sf::Vector2f origin)
    {
        draw_text(target, font, "Album", {origin.x + COLUMN_LABEL, origin.y}, HEADER_COLOR);
        draw_text(target, font, "Titulo", {origin.x + COLUMN_TITLE, origin.y}, HEADER_COLOR);
        draw_text(target, font, "Lanzamiento", {origin.x + COLUMN_RELEASE, origin.y}, HEADER_COLOR);
        draw_text(target, font, "Duracion", {origin.x + COLUMN_DURATION, origin.y}, HEADER_COLOR);

        for (std::size_t row = 0; row < albums.size(); ++row)
        {
            const models::Album& album = albums[row];
            const float y = origin.y + ROW_HEIGHT * static_cast<float>(row + 1);

            draw_text(target, font, album_label(album.id), {origin.x + COLUMN_LABEL, y}, album_color(album.id));
            draw_text(target, font, album.name, {origin.x + COLUMN_TITLE, y});
            draw_text(target, font, std::to_string(album.releaseDate), {origin.x + COLUMN_RELEASE, y});
            draw_text(target, font, std::to_string(album.duration), {origin.x + COLUMN_DURATION, y});
        }
    }

    void draw_gantt(sf::RenderTarget& target, const sf::Font& font, const GanttChart& chart,
                    const std::vector<models::Album>& albums, const sf::Vector2f origin, const int total_time)
    {
        draw_text(target, font, chart.title, origin, HEADER_COLOR);

        const float plot_left = origin.x + GANTT_LABEL_WIDTH;
        const float top = origin.y + GANTT_TITLE_HEIGHT;
        const float unit = GANTT_PLOT_WIDTH / static_cast<float>(std::max(total_time, 1));

        for (std::size_t row = 0; row < albums.size(); ++row)
        {
            const float y = top + GANTT_ROW_HEIGHT * static_cast<float>(row);
            draw_rect(target, {plot_left, y, GANTT_PLOT_WIDTH, GANTT_ROW_HEIGHT - 2.F}, ROW_COLOR);
            draw_text(target, font, album_label(albums[row].id), {origin.x, y + 3.F}, sf::Color::White,
                      SMALL_TEXT_SIZE);
        }

        for (const models::ExecutionSegment& segment : chart.segments)
        {
            const auto row = static_cast<std::size_t>(segment.album_id);
            if (row >= albums.size())
                continue;

            const float x = plot_left + unit * static_cast<float>(segment.start);
            const float y = top + GANTT_ROW_HEIGHT * static_cast<float>(row);
            const float width = unit * static_cast<float>(segment.end - segment.start);
            draw_rect(target, {x, y, width, GANTT_ROW_HEIGHT - 2.F}, album_color(segment.album_id));
        }

        const float axis_y = top + GANTT_ROW_HEIGHT * static_cast<float>(albums.size());
        for (int t = 0; t <= total_time; ++t)
        {
            const bool labeled = t % AXIS_LABEL_EVERY == 0;
            const float x = plot_left + unit * static_cast<float>(t);
            draw_rect(target, {x, axis_y, 1.F, labeled ? 6.F : 3.F}, AXIS_COLOR);
            if (labeled)
                draw_text(target, font, std::to_string(t), {x - 4.F, axis_y + 6.F}, AXIS_COLOR, SMALL_TEXT_SIZE);
        }
    }
}