#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    if (abs(gl_FragCoord.x - 0.5) < 0.01 || abs(gl_FragCoord.y - 0.5) < 0.01) 
    {
        FragColor = vec4(0.8, 0.2, 0.2, 1.0);
    }
}