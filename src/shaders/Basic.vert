#version 330 core

layout(location = 0) in vec3 attrPos;
layout(location = 1) in vec3 attrNormal;
layout(location = 2) in vec2 attrTexCoord;

out vec3 fragmentPosition;
out vec3 normal;
out vec2 textureCoordinates;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main() 
{
    fragmentPosition = vec3(ModelMatrix * vec4(attrPos, 1.0));

    normal = mat3(transpose(inverse(ModelMatrix))) * attrNormal;

    textureCoordinates = attrTexCoord;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(attrPos, 1.0);
}