/*
 * =====================================================================================
 *
 *       Filename:  Texture.cpp
 *
 *    Description:
 *
 *        Created:  20/12/2014 01:15:43
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "gk/gl/GLCheck.hpp"
#include "gk/gl/Texture.hpp"
#include "gk/core/Exception.hpp"

namespace gk {

Texture::Texture(const std::string &filename) {
	loadFromFile(filename);
}

Texture::Texture(SDL_Surface *surface) {
	loadFromSurface(surface);
}

Texture::Texture(Texture &&texture) {
	m_filename = texture.m_filename;

	m_size = texture.m_size;

	m_texture = texture.m_texture;
	texture.m_texture = 0;
}

Texture::~Texture() noexcept {
	glCheck(glDeleteTextures(1, &m_texture));
}

Texture &Texture::operator=(Texture &&texture) {
	m_filename = texture.m_filename;

	m_size = texture.m_size;

	m_texture = texture.m_texture;
	texture.m_texture = 0;

	return *this;
}

void Texture::loadFromFile(const std::string &filename) {
	m_filename = filename;

	SDL_Surface *surface = IMG_Load(m_filename.c_str());
	if(!surface) {
		throw EXCEPTION("Failed to load texture:", filename);
	}

	loadFromSurface(surface);

	SDL_FreeSurface(surface);
}

void Texture::loadFromSurface(SDL_Surface *surface) {
	m_size.x = surface->w;
	m_size.y = surface->h;

	if (m_texture == 0)
		glCheck(glGenTextures(1, &m_texture));

	bind(this);

	// GLenum format = (surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	// glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

	GLenum format;
	if (surface->format->BytesPerPixel == 4) {
		format = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
	} else {
		format = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
	}

	glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, format, GL_UNSIGNED_BYTE, surface->pixels));

	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	bind(nullptr);
}

void Texture::bind(const Texture *texture) {
	if(texture) {
		glCheck(glBindTexture(GL_TEXTURE_2D, texture->m_texture));
	} else {
		glCheck(glBindTexture(GL_TEXTURE_2D, 0));
	}
}

} // namespace gk

