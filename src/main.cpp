#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

int main()
{
    sf::Font font;
    const std::string font_path = std::string {ZUTOMAYO_ASSET_DIR} + "/assets/regularsans.ttf";
    if (!font.loadFromFile(font_path))
    {
        std::cout << "No se pudo cargar la fuente: " << font_path << "\n";
        return 1;
    }

    sf::Text title("ZUTOMAYO listening", font, 28);
    title.setPosition(20.F, 20.F);

    sf::RenderWindow window(sf::VideoMode(900, 600), "ZUTOMAYO listening");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(20, 20, 20));
        window.draw(title);
        window.display();
    }
    return 0;
}