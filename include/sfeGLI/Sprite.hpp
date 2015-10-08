/*
*  Sprite.hpp
*  sfeGLI project
*
*  Copyright (C) 2015 Stephan Vedder
*  stephan.vedder@gmail.com
*
*  This program is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this program; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*
*/
#ifndef SFEGLI_SPRITE_HPP
#define SFEGLI_SPRITE_HPP

#include <sfeGLI/Visibility.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace sfe
{
class Texture;

class SFE_API Sprite : public sf::Drawable, public sf::Transformable
{
public:
    Sprite();

    explicit Sprite(const Texture& texture);

    Sprite(const Texture& texture, const sf::IntRect& rectangle);

    void setTexture(const Texture& texture, bool resetRect = false);

    void setTextureRect(const sf::IntRect& rectangle);

    void setColor(const sf::Color& color);

    const Texture* getTexture() const;

    const sf::IntRect& getTextureRect() const;

    const sf::Color& getColor() const;

    sf::FloatRect getLocalBounds() const;

    sf::FloatRect getGlobalBounds() const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void updatePositions();

    void updateTexCoords();

	void applyTransform(const sf::Transform& transform);

    sf::Vertex      m_vertices[4]; 
    const Texture*	m_texture;     
    sf::IntRect     m_textureRect;
};

}

#endif 
