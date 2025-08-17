#ifndef TEXTURE_OBJECT_H
#define TEXTURE_OBJECT_H

#include <string>
#include <glad/glad.h>

class TextureObject 
{
private:
    unsigned int textureID;
    int width, height, channels;
    bool loaded;

    void loadFromFile(const std::string& filePath, bool flipVertically);
    void setupTextureParameters();

public:
    TextureObject(const std::string& filePath, bool flipVertically = true);
    ~TextureObject();

    bool isLoaded() const { return loaded; }
    unsigned int getTextureID() const { return textureID; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }

    void bind(unsigned int textureUnit = GL_TEXTURE0) const;
    static void unbind();
};

#endif