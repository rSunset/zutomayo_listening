#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "interface/Drawing.hpp"
#include "logic/AlbumGenerator.hpp"
#include "logic/Scheduling.hpp"
#include "models/Album.hpp"

namespace
{
    constexpr unsigned int WINDOW_WIDTH = 1280;
    constexpr unsigned int WINDOW_HEIGHT = 800;
    constexpr float GANTT_ORIGIN_X = 600.F;
    constexpr float GANTT_ORIGIN_Y = 20.F;

    struct Simulation
    {
        std::vector<zutomayo::models::Album> albums;
        std::vector<zutomayo::ui::GanttChart> charts;
        int total_time{0};
    };

    Simulation run_simulation(const std::vector<std::string>& titles, const std::uint32_t seed)
    {
        Simulation simulation;
        simulation.albums = zutomayo::logic::generate_albums(titles, seed);
        simulation.charts = {
            {"FCFS: primero en llegar, primero en servirse", zutomayo::logic::simulate_fcfs(simulation.albums)},
            {"SPN: primero el mas corto", zutomayo::logic::simulate_spn(simulation.albums)},
            {"SRT: menor tiempo restante", zutomayo::logic::simulate_srt(simulation.albums)},
            {"HRRN: mayor tasa de respuesta", zutomayo::logic::simulate_hrrn(simulation.albums)},
        };

        for (const zutomayo::ui::GanttChart& chart : simulation.charts)
            for (const zutomayo::models::ExecutionSegment& segment : chart.segments)
                simulation.total_time = std::max(simulation.total_time, segment.end);

        return simulation;
    }
}

int main()
{
    sf::Font font;
    const std::filesystem::path font_path =
        std::filesystem::path{ZUTOMAYO_ASSET_DIR} / "assets" / "regularsans.ttf";
    if (!font.loadFromFile(font_path.string()))
    {
        std::cerr << "No se pudo cargar la fuente: " << font_path << '\n';
        return 1;
    }

    const std::vector<std::string> titles{
        "Hisohiso Banashi", "Gusare", "Jinkogaku", "Meiko wa Gunaruga Gotoshi", "Keisodo"};

    std::random_device entropy;
    Simulation simulation = run_simulation(titles, static_cast<std::uint32_t>(entropy()));

    const zutomayo::ui::Button generate_button{sf::FloatRect(20.F, 70.F, 200.F, 36.F), "Generar"};

    sf::Text heading("ZUTOMAYO listening", font, 28);
    heading.setPosition(20.F, 20.F);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "zutomayo_listening");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                const sf::Vector2f click(static_cast<float>(event.mouseButton.x),
                                         static_cast<float>(event.mouseButton.y));
                if (generate_button.bounds.contains(click))
                    simulation = run_simulation(titles, static_cast<std::uint32_t>(entropy()));
            }
        }

        window.clear(sf::Color(20, 20, 30));
        window.draw(heading);
        zutomayo::ui::draw_button(window, font, generate_button);
        zutomayo::ui::draw_album_table(window, font, simulation.albums, {20.F, 130.F});

        for (std::size_t i = 0; i < simulation.charts.size(); ++i)
        {
            const float y = GANTT_ORIGIN_Y + zutomayo::ui::GANTT_BLOCK_HEIGHT * static_cast<float>(i);
            zutomayo::ui::draw_gantt(window, font, simulation.charts[i], simulation.albums,
                                     {GANTT_ORIGIN_X, y}, simulation.total_time);
        }

        window.display();
    }

    return 0;
}