#version 330 core

layout (location = 0) in vec3 aPos;
uniform float iTime;

void main() { 
    vec3 animatedPosition = aPos;
    vec2 amplitude = vec2(0.7, 0.9);
    animatedPosition.x += sin(iTime + aPos.y) * amplitude.x;
    animatedPosition.y += cos(iTime + aPos.x) * amplitude.y;

    gl_Position = vec4(animatedPosition, 1.0);
}