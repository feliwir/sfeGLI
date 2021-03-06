#include <sfeGLI/Sprite.hpp>
#include <sfeGLI/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/OpenGL.hpp>
#include <cstdlib>

namespace sfe
{
	////////////////////////////////////////////////////////////
	Sprite::Sprite() :
		m_texture(NULL),
		m_textureRect()
	{

	}


	////////////////////////////////////////////////////////////
	Sprite::Sprite(const Texture& texture) :
		m_texture(NULL),
		m_textureRect()
	{
		setTexture(texture);
	}


	////////////////////////////////////////////////////////////
	Sprite::Sprite(const Texture& texture, const sf::IntRect& rectangle) :
		m_texture(NULL),
		m_textureRect()
	{
		setTexture(texture);
		setTextureRect(rectangle);
	}


	////////////////////////////////////////////////////////////
	void Sprite::setTexture(const Texture& texture, bool resetRect)
	{
		// Recompute the texture area if requested, or if there was no valid texture & rect before
		if (resetRect || (!m_texture && (m_textureRect == sf::IntRect())))
			setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));

		// Assign the new texture
		m_texture = &texture;
	}


	////////////////////////////////////////////////////////////
	void Sprite::setTextureRect(const sf::IntRect& rectangle)
	{
		if (rectangle != m_textureRect)
		{
			m_textureRect = rectangle;
			updatePositions();
			updateTexCoords();
		}
	}


	////////////////////////////////////////////////////////////
	void Sprite::setColor(const sf::Color& color)
	{
		// Update the vertices' color
		m_vertices[0].color = color;
		m_vertices[1].color = color;
		m_vertices[2].color = color;
		m_vertices[3].color = color;
	}


	////////////////////////////////////////////////////////////
	const Texture* Sprite::getTexture() const
	{
		return m_texture;
	}


	////////////////////////////////////////////////////////////
	const sf::IntRect& Sprite::getTextureRect() const
	{
		return m_textureRect;
	}


	////////////////////////////////////////////////////////////
	const sf::Color& Sprite::getColor() const
	{
		return m_vertices[0].color;
	}


	////////////////////////////////////////////////////////////
	sf::FloatRect Sprite::getLocalBounds() const
	{
		float width = static_cast<float>(std::abs(m_textureRect.width));
		float height = static_cast<float>(std::abs(m_textureRect.height));

		return sf::FloatRect(0.f, 0.f, width, height);
	}


	////////////////////////////////////////////////////////////
	sf::FloatRect Sprite::getGlobalBounds() const
	{
		return getTransform().transformRect(getLocalBounds());
	}

	void Sprite::applyTransform(const sf::Transform& transform)
	{
		glLoadMatrixf(transform.getMatrix());
	}

	////////////////////////////////////////////////////////////
	void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_texture)
		{
			states.transform *= getTransform();
			int vertexCount = 6;
			
			glLoadMatrixf(states.transform.getMatrix());
			sf::IntRect viewport = target.getViewport(target.getView());
			int top = target.getSize().y - (viewport.top + viewport.height);
			glViewport(viewport.left, top, viewport.width, viewport.height);

			// Set the projection matrix
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(target.getView().getTransform().getMatrix());

			// Go back to model-view mode
			glMatrixMode(GL_MODELVIEW);
			m_texture->Bind();

			// Apply the shader
			sf::Shader::bind(states.shader);

			if (m_vertices)
			{
				const char* data = reinterpret_cast<const char*>(m_vertices);
				glVertexPointer(2, GL_FLOAT, sizeof(sf::Vertex), data + 0);
				glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(sf::Vertex), data + 8);
				glTexCoordPointer(2, GL_FLOAT, sizeof(sf::Vertex), data + 12);
			}

			glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
		}
	}


	////////////////////////////////////////////////////////////
	void Sprite::updatePositions()
	{
		sf::FloatRect bounds = getLocalBounds();

		m_vertices[0].position = sf::Vector2f(0, 0);
		m_vertices[1].position = sf::Vector2f(0, bounds.height);
		m_vertices[2].position = sf::Vector2f(bounds.width, 0);
		m_vertices[3].position = sf::Vector2f(bounds.width, bounds.height);
	}


	////////////////////////////////////////////////////////////
	void Sprite::updateTexCoords()
	{
		float left = static_cast<float>(m_textureRect.left);
		float right = left + m_textureRect.width;
		float top = static_cast<float>(m_textureRect.top);
		float bottom = top + m_textureRect.height;

		m_vertices[0].texCoords = sf::Vector2f(left, top);
		m_vertices[1].texCoords = sf::Vector2f(left, bottom);
		m_vertices[2].texCoords = sf::Vector2f(right, top);
		m_vertices[3].texCoords = sf::Vector2f(right, bottom);
	}
}