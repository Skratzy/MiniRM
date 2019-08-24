#include "TextureResource.h"

TextureResource::TextureResource(unsigned int width, unsigned int height, unsigned char* image, const long GUID)
	: Resource(GUID)
{
	m_width = width;
	m_height = height;
	m_image = image;
}

TextureResource::~TextureResource() {
	RM_FREE(m_image);
}
