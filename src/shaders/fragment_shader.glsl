#version 330 core

out vec4 FragColor;
uniform vec2 iResolution;
uniform float iTime;

float randomRange(float min, float max, float seed) {
    float randValue = fract(sin(seed) * 43758.5453);
    return min + randValue * (max - min);
}

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution; 

    float randValueX = randomRange(0.0, 1.0, uv.x + iTime);
    float randValueY = randomRange(0.0, 1.0, uv.y + iTime);

    vec3 col = vec3(randValueX + cos(iTime + uv.x), randValueY + sin(iTime + uv.y), randValueX + randValueY);

    col = col *2.0*sin((iTime + uv.x*cos(iTime/12.0) + uv.y*cos(iTime/11.0))*100.0*sin(iTime/6.0));

    FragColor = vec4(col, 1.0);         
}