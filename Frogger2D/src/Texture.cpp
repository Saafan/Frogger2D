#include "Texture.h"
#include <iostream>

#include <stb_image/stb_image.h>

Texture::Texture(const std::string& path)
    : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	if (path == "")
		empty = true;


	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (!m_LocalBuffer)
	{
		empty = true;
	}

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
	glEnable(GL_TEXTURE_2D);
}

void Texture::LoadTexture()
{

	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
	glEnable(GL_TEXTURE_2D);
}

void Texture::LoadTexture(const std::string& path)
{

	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
	glEnable(GL_TEXTURE_2D);
}


Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const
{
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}




unsigned char* Texture::GetLocalBuffer()
{
    return m_LocalBuffer;
}

void Texture::SetLocalBuffer(unsigned char* c)
{
    m_LocalBuffer = c;
}
