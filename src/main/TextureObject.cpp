#include <general/TextureObject.h>

#include <stb/stb_image.h>
#include <glad/glad.h>

#include <iostream>

TextureObject::TextureObject(const std::string& filePath, bool flipVertically) 
    : textureID(0), width(0), height(0), channels(0), loaded(false) 
{
    loadFromFile(filePath, flipVertically);
}

TextureObject::~TextureObject() 
{
    if (textureID != 0) 
    {
        glDeleteTextures(1, &textureID);
    }
}

void TextureObject::bind(unsigned int textureUnit) const 
{
    if (loaded) 
    {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
}

void TextureObject::unbind() 
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureObject::loadFromFile(const std::string& filePath, bool flipVertically)
{
    stbi_set_flip_vertically_on_load(flipVertically);
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    
    if (!data) 
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        loaded = false;
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    unsigned int format = GL_RGB;
    if (channels == 1)
        format = GL_RED;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    setupTextureParameters();

    stbi_image_free(data);

    loaded = true;

    unbind();
}

void TextureObject::setupTextureParameters() 
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
}