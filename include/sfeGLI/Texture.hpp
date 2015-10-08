/*
 *  Texture.hpp
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
 #ifndef SFEGLI_IMAGE_HPP
 #define SFEGLI_IMAGE_HPP

 ////////////////////////////////////////////////////////////
 // Headers
 ////////////////////////////////////////////////////////////
#include <SFML/Window/GlResource.hpp>
#include <SFML/Config.hpp>
#include <sfeGLI/Visibility.hpp>
#include <memory>

namespace sfe
{
		namespace priv
		{
			#ifdef SFML_DEBUG
			#define glCheck(expr) do { expr; sfe::priv::glCheckError(__FILE__, __LINE__, #expr); } while (false)
			#else
			#define glCheck(expr) (expr)
			#endif
			void glCheckError(const char* file, unsigned int line, const char* expression);
		}

	typedef sf::Vector3<unsigned int> Vector3u;

	class SFE_API Texture : sf::GlResource
	{
	public:
		Texture();
		Texture(const Texture&) = delete;
		~Texture();

		bool loadFromFile(const std::string& filename);
		bool loadFromMemory(const void* data, std::size_t size);
		bool loadFromStream(sf::InputStream& stream);

		Vector3u getSize() const;
		unsigned int getDimension() const;
		unsigned int getLevels() const;
		
		void Bind() const;

	private:
		Vector3u		m_size;
		unsigned int	m_dimension;
		unsigned int	m_texture;
		unsigned int	m_mipLevels;
		unsigned int	m_target;
	};
}


 #endif
