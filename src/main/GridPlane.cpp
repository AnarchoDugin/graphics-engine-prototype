#include <general/GridPlane.h>
#include <vector>

GridPlane::GridPlane(float size, float step) 
    : VAO(0), VBO(0), gridShader(nullptr), size(size), step(step)
{
    setupGridMesh();
}

GridPlane::~GridPlane() 
{
    if (VAO != 0) 
        glDeleteVertexArrays(1, &VAO);

    if (VBO != 0) 
        glDeleteBuffers(1, &VBO);
}

void GridPlane::setupGridMesh() 
{
    std::vector<float> vertices;
    
    for (float i = -size; i <= size; i += step) 
    { 
        vertices.push_back(-size); vertices.push_back(0.0f); vertices.push_back(i);
        vertices.push_back(size);  vertices.push_back(0.0f); vertices.push_back(i);
        
        vertices.push_back(i); vertices.push_back(0.0f); vertices.push_back(-size);
        vertices.push_back(i); vertices.push_back(0.0f); vertices.push_back(size);
    }
    
    vertexCount = vertices.size() / 3;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glBindVertexArray(0);
}

void GridPlane::draw(const glm::mat4& view, const glm::mat4& projection)
{
    if (!gridShader) return;
    
    gridShader->use();

    gridShader->setMat4("ViewMatrix", view);
    gridShader->setMat4("ProjectionMatrix", projection);
    gridShader->setMat4("ModelMatrix", glm::mat4(1.0f));
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, vertexCount);
    glBindVertexArray(0);
}