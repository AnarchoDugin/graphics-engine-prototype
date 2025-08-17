#version 330 core
layout (location = 0) in vec3 attrPos;

out vec3 TexCoords;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main() {
    TexCoords = attrPos;
    vec4 pos = ProjectionMatrix * ViewMatrix * vec4(attrPos, 1.0);
    gl_Position = pos.xyww;
}