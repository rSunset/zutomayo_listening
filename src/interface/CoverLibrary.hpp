//
// Created by miniroque on 24/11/26.
//
#pragma once
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics/Texture.hpp>

namespace zutomayo::ui
{
    class CoverLibrary
    {
    public:
        CoverLibrary(const std::filesystem::path& directory, const std::vector<std::string>& file_names);

        // nullptr si esa portada no se pudo cargar o el id no existe
        const sf::Texture* find(int album_id) const;

    private:
        std::vector<std::unique_ptr<sf::Texture>> textures_;
    };
}