#include <general/Object3D.h>
#include <glad/glad.h>
#include <sys/stat.h>

Object3D::Object3D() : VertexArrayObject(0), VertexBufferObject(0), ElementBufferObject(0), indicesCount(0)
{
    glGenVertexArrays(1, &VertexArrayObject);

    glGenBuffers(1, &VertexBufferObject);
    glGenBuffers(1, &ElementBufferObject);

    diffuseMap = nullptr;
    specularMap = nullptr;
}

Object3D::~Object3D()
{
    glDeleteVertexArrays(1, &VertexArrayObject);

    glDeleteBuffers(1 , &VertexBufferObject);
    glDeleteBuffers(1, &ElementBufferObject);

    if (diffuseMap != nullptr)
        delete diffuseMap;
    
    if (specularMap != nullptr)
        delete specularMap;
}

bool Object3D::loadFromFile(const std::string& fileName)
{
    tinyobj::attrib_t attrib;

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn, err;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
        fileName.c_str(),
        nullptr,
        true
    );

    if (!warn.empty()) std::cerr << "Warning: " << warn << std::endl;
    if (!err.empty()) std::cerr << "Error: " << err << std::endl;

    if (!success) 
    {
        std::cerr << "Couldn't load .obj file..." << std::endl;
        return false;
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (const auto& shape: shapes)
    {
        for (const auto& index: shape.mesh.indices)
        {
            Vertex vertex;

            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (index.normal_index >= 0)
            {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if (index.texcoord_index >= 0) 
            {
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            vertices.push_back(vertex);
            indices.push_back(static_cast<unsigned int>(indices.size()));
        }
    }

    indicesCount = indices.size();

    glBindVertexArray(VertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data());

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoord)));

    glBindVertexArray(0);

    return true;
}

bool Object3D::loadTextures(const std::string& textureDirectory)
{
    std::string texturesPath = textureDirectory + "/textures/";

    const std::vector<std::string> availableExtensions = { ".jpg", ".jpeg", ".png" };

    auto tryLoadTexture = [&](const std::string& baseName) -> TextureObject* 
    {
        for (const auto& extension : availableExtensions)
        {
            std::string fullPath = texturesPath + baseName + extension;
            struct stat buffer;
            
            if (stat(fullPath.c_str(), &buffer) == 0)
            {
                return new TextureObject(fullPath);
            }
        }

        return nullptr;
    };

    diffuseMap = tryLoadTexture("diffuse");
    if (!diffuseMap) 
    {
        std::cerr << "Warning: Could not find diffuse texture in " << texturesPath << std::endl;
    }

    specularMap = tryLoadTexture("specular");
    if (!specularMap)
    {
        std::cerr << "Warning: Could not find specular texture in " << texturesPath << std::endl;
    }

    return diffuseMap != nullptr;

}

void Object3D::render() const 
{
    if (diffuseMap) 
        diffuseMap->bind(GL_TEXTURE0);

    if (specularMap)
        specularMap->bind(GL_TEXTURE1);

    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, static_cast<int>(indicesCount), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    TextureObject::unbind();
}