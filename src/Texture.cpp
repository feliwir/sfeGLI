/*
*  Texture.cpp
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

#include <SFML/System.hpp>
#include <SFML/Config.hpp>
#include <sfeGLI/Texture.hpp>
#include <gli/gli.hpp>
#include "GL.hpp"


namespace sfe
{
	Texture::Texture() : m_texture(NULL)
	{
		flextInit();
		glGenTextures(1, &m_texture);
	}

	Texture::~Texture()
	{
		if (m_texture)
			glDeleteTextures(1, &m_texture);
	}

	bool Texture::loadFromFile(const std::string & filename)
	{
		sf::FileInputStream fin;
		if (!fin.open(filename))
			return false;

		return loadFromStream(fin);
	}


	bool Texture::loadFromMemory(const void * data, std::size_t size)
	{
		gli::texture Texture = gli::load((const char*)data,size);
		if (Texture.empty())
			return 0;

		gli::gl GL;
		gli::gl::format const Format = GL.translate(Texture.format());
		m_target = GL.translate(Texture.target());
		
		glCheck(glBindTexture(m_target, m_texture));
		glCheck(glTexParameteri(m_target, GL_TEXTURE_BASE_LEVEL, 0));
		glCheck(glTexParameteri(m_target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1)));
		glCheck(glTexParameteri(m_target, GL_TEXTURE_SWIZZLE_R, Format.Swizzles[0]));
		glCheck(glTexParameteri(m_target, GL_TEXTURE_SWIZZLE_G, Format.Swizzles[1]));
		glCheck(glTexParameteri(m_target, GL_TEXTURE_SWIZZLE_B, Format.Swizzles[2]));
		glCheck(glTexParameteri(m_target, GL_TEXTURE_SWIZZLE_A, Format.Swizzles[3]));
		
		glm::tvec3<GLsizei> const Dimensions(Texture.dimensions());
		GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());
		m_mipLevels = Texture.levels();
		m_size = Vector3u(	Dimensions.x, 
							Dimensions.y, 
							Dimensions.z);

		
		switch (Texture.target())
		{
		case gli::TARGET_1D:
			glCheck(glTexStorage1D(
				m_target, static_cast<GLint>(Texture.levels()), Format.Internal, Dimensions.x));
			break;
		case gli::TARGET_1D_ARRAY:
		case gli::TARGET_2D:
		case gli::TARGET_CUBE:
			glCheck(glTexStorage2D(
				m_target, static_cast<GLint>(Texture.levels()), Format.Internal,
				Dimensions.x, Texture.target() == gli::TARGET_2D ? Dimensions.y : FaceTotal));
			break;
		case gli::TARGET_2D_ARRAY:
		case gli::TARGET_3D:
		case gli::TARGET_CUBE_ARRAY:
			glCheck(glTexStorage3D(
				m_target, static_cast<GLint>(Texture.levels()), Format.Internal,
				Dimensions.x, Dimensions.y,
				Texture.target() == gli::TARGET_3D ? Dimensions.z : FaceTotal));
			break;
		default:
			assert(0);
			break;
		}

		for (std::size_t Layer = 0; Layer < Texture.layers(); ++Layer)
			for (std::size_t Face = 0; Face < Texture.faces(); ++Face)
				for (std::size_t Level = 0; Level < Texture.levels(); ++Level)
				{
					GLsizei const LayerGL = static_cast<GLsizei>(Layer);
					glm::tvec3<GLsizei> Dimensions(Texture.dimensions(Level));
					m_target = gli::is_target_cube(Texture.target()) 
						? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face): m_target;

					switch (Texture.target())
					{
					case gli::TARGET_1D:
						if (gli::is_compressed(Texture.format()))
							glCompressedTexSubImage1D(
								m_target, static_cast<GLint>(Level), 0, Dimensions.x,
								Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
								Texture.data(Layer, Face, Level));
						else
							glTexSubImage1D(
								m_target, static_cast<GLint>(Level), 0, Dimensions.x,
								Format.External, Format.Type,
								Texture.data(Layer, Face, Level));
						break;
					case gli::TARGET_1D_ARRAY:
					case gli::TARGET_2D:
					case gli::TARGET_CUBE:
						if (gli::is_compressed(Texture.format()))
							glCompressedTexSubImage2D(
								m_target, static_cast<GLint>(Level),
								0, 0,Dimensions.x,
								Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Dimensions.y,
								Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
								Texture.data(Layer, Face, Level));
						else
							glTexSubImage2D(m_target, static_cast<GLint>(Level),
								0, 0, Dimensions.x,
								Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Dimensions.y,
								Format.External, Format.Type,
								Texture.data(Layer, Face, Level));
						break;
					case gli::TARGET_2D_ARRAY:
					case gli::TARGET_3D:
					case gli::TARGET_CUBE_ARRAY:
						if (gli::is_compressed(Texture.format()))
							glCompressedTexSubImage3D(
								m_target, static_cast<GLint>(Level),
								0, 0, 0, Dimensions.x, Dimensions.y,
								Texture.target() == gli::TARGET_3D ? Dimensions.z : LayerGL,
								Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
								Texture.data(Layer, Face, Level));
						else
							glTexSubImage3D(
								m_target, static_cast<GLint>(Level),
								0, 0, 0, Dimensions.x, Dimensions.y,
								Texture.target() == gli::TARGET_3D ? Dimensions.z : LayerGL,
								Format.External, Format.Type,
								Texture.data(Layer, Face, Level));
						break;
					default: assert(0); break;
					}
				}
		return true;
	}

	bool Texture::loadFromStream(sf::InputStream & stream)
	{
		sf::Int64 size = stream.getSize();
		if (size == -1)
			return false;

		sf::Uint8* data = new sf::Uint8[size];
		stream.read(data, size);
		return loadFromMemory(data, size);
	}

	Vector3u Texture::getSize() const
	{
		return m_size;
	}

	unsigned int Texture::getDimension() const
	{
		return m_dimension;
	}

	unsigned int Texture::getLevels() const
	{
		return m_mipLevels;
	}

	void Texture::Bind() const
	{
		glBindTexture(m_target, m_texture);
	}

	namespace priv
	{
		////////////////////////////////////////////////////////////
		void glCheckError(const char* file, unsigned int line, const char* expression)
		{
			// Get the last error
			GLenum errorCode = glGetError();

			if (errorCode != GL_NO_ERROR)
			{
				std::string fileString = file;
				std::string error = "Unknown error";
				std::string description = "No description";

				// Decode the error code
				switch (errorCode)
				{
				case GL_INVALID_ENUM:
				{
					error = "GL_INVALID_ENUM";
					description = "An unacceptable value has been specified for an enumerated argument.";
					break;
				}

				case GL_INVALID_VALUE:
				{
					error = "GL_INVALID_VALUE";
					description = "A numeric argument is out of range.";
					break;
				}

				case GL_INVALID_OPERATION:
				{
					error = "GL_INVALID_OPERATION";
					description = "The specified operation is not allowed in the current state.";
					break;
				}

				case GL_OUT_OF_MEMORY:
				{
					error = "GL_OUT_OF_MEMORY";
					description = "There is not enough memory left to execute the command.";
					break;
				}
				}

				// Log the error
				sf::err() << "An internal OpenGL call failed in "
					<< fileString.substr(fileString.find_last_of("\\/") + 1) << "(" << line << ")."
					<< "\nExpression:\n   " << expression
					<< "\nError description:\n   " << error << "\n   " << description << "\n"
					<< std::endl;
			}
		}
	}
}