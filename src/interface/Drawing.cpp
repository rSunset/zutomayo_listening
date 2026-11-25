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
        constexpr float TABLE_HEADER_HEIGHT = 30.F;
        constexpr float TABLE_ROW_HEIGHT = 64.F;
        constexpr float TEXT_OFFSET_Y = 20.F;
        constexpr float COVER_SIZE = 56.F;
        constexpr float COVER_BORDER = 3.F;

        constexpr float COLUMN_COVER = 0.F;
        constexpr float COLUMN_LABEL = 72.F;
        constexpr float COLUMN_TITLE = 100.F;
        constexpr float COLUMN_RELEASE = 340.F;
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
                sf::Color(235, 185, 60),    // 0 Hisohiso Banashi: dorado
                sf::Color(70, 130, 240),    // 1 Gusare: azul
                sf::Color(100, 205, 175),   // 2 Jinkougaku: turquesa
                sf::Color(225, 70, 80),     // 3 Meiko wa Gunaruga Gotoshi: rojo
                sf::Color(250, 213, 170)};  // 4 Keisoudo: crema
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

        void draw_cover(sf::RenderTarget& target, const sf::Font& font, const sf::Texture* texture,
                const models::Album& album, const sf::Vector2f position)
        {
            draw_rect(target,
                      {position.x - COVER_BORDER, position.y - COVER_BORDER,
                       COVER_SIZE + 2.F * COVER_BORDER, COVER_SIZE + 2.F * COVER_BORDER},
                      album_color(album.id));

            if (texture == nullptr)
            {
                draw_rect(target, {position.x, position.y, COVER_SIZE, COVER_SIZE}, ROW_COLOR);
                draw_text(target, font, album_label(album.id), {position.x + 20.F, position.y + 16.F});
                return;
            }

            sf::Sprite sprite(*texture);
            const sf::Vector2u texture_size = texture->getSize();
            const float scale = COVER_SIZE / static_cast<float>(std::max(texture_size.x, texture_size.y));
            sprite.setScale(scale, scale);
            sprite.setPosition(position);
            target.draw(sprite);
        }
    }

    void draw_button(sf::RenderTarget& target, const sf::Font& font, const Button& button)
    {
        draw_rect(target, button.bounds, BUTTON_COLOR);
        draw_text(target, font, button.label, {button.bounds.left + 16.F, button.bounds.top + 7.F});
    }

    void draw_album_table(sf::RenderTarget& target, const sf::Font& font,
                      const std::vector<models::Album>& albums, const CoverLibrary& covers,
                      const sf::Vector2f origin)
    {
        draw_text(target, font, "ID", {origin.x + COLUMN_LABEL, origin.y}, HEADER_COLOR);
        draw_text(target, font, "Titulo", {origin.x + COLUMN_TITLE, origin.y}, HEADER_COLOR);
        draw_text(target, font, "Lanzamiento", {origin.x + COLUMN_RELEASE, origin.y}, HEADER_COLOR);
        draw_text(target, font, "Duracion", {origin.x + COLUMN_DURATION, origin.y}, HEADER_COLOR);

        for (std::size_t row = 0; row < albums.size(); ++row)
        {
            const models::Album& album = albums[row];
            const float top = origin.y + TABLE_HEADER_HEIGHT + TABLE_ROW_HEIGHT * static_cast<float>(row);
            const float text_y = top + TEXT_OFFSET_Y;

            draw_cover(target, font, covers.find(album.id), album, {origin.x + COLUMN_COVER, top});
            draw_text(target, font, album_label(album.id), {origin.x + COLUMN_LABEL, text_y}, album_color(album.id));
            draw_text(target, font, album.name, {origin.x + COLUMN_TITLE, text_y});
            draw_text(target, font, std::to_string(album.releaseDate), {origin.x + COLUMN_RELEASE, text_y});
            draw_text(target, font, std::to_string(album.duration), {origin.x + COLUMN_DURATION, text_y});
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