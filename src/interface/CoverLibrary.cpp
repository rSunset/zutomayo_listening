//
// Created by miniroque on 24/11/26.
//
#include "interface/CoverLibrary.hpp"

#include <cstddef>
#include <iostream>
#include <utility>

namespace zutomayo::ui
{
    CoverLibrary::CoverLibrary(const std::filesystem::path& directory,
                               const std::vector<std::string>& file_names)
    {
        textures_.reserve(file_names.size());
        for (const std::string& file_name : file_names)
        {
            const std::filesystem::path path = directory / file_name;
            auto texture = std::make_unique<sf::Texture>();
            if (texture->loadFromFile(path.string()))
            {
                texture->setSmooth(true);
                textures_.push_back(std::move(texture));
            }
            else
            {
                std::cerr << "No se pudo cargar la portada: " << path << '\n';
                textures_.push_back(nullptr);
            }
        }
    }

    const sf::Texture* CoverLibrary::find(const int album_id) const
    {
        const auto index = static_cast<std::size_t>(album_id);
        if (index >= textures_.size())
            return nullptr;
        return textures_[index].get();
    }
}