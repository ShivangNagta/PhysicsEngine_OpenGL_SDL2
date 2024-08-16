#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textCoord;

uniform mat4 u_ModelMatrixLight;
uniform mat4 u_PerspectiveLight;
uniform mat4 u_ViewLight;
uniform float u_Seed;

out vec2 v_textCoord;
out vec3 v_normal;
out vec3 fragPos;

float rand(vec2 coord) {
    return fract(sin(dot(coord.xy ,vec2(12.9898,78.233) + u_Seed)) * 43758.5453);
}

float noise(vec2 coord) {
    vec2 i = floor(coord);
    vec2 f = fract(coord);
    f = f*f*(3.0-2.0*f);
    
    return mix(mix(rand(i + vec2(0.0, 0.0)), rand(i + vec2(1.0, 0.0)), f.x),
               mix(rand(i + vec2(0.0, 1.0)), rand(i + vec2(1.0, 1.0)), f.x),
               f.y);
}

// Fractal Brownian Motion (fBm) for more natural-looking noise
float fbm(vec2 coord) {
    float value = 0.0;
    float amplitude = 0.2;
    float frequency = 100.0;
    for (int i = 0; i < 6; i++) {
        value += amplitude * noise(coord * frequency);
        amplitude *= 0.5;
        frequency *= 2.0;
    }
    return value;
}

void main() {
    v_textCoord = textCoord;

    // Apply fBm noise to the y-coordinate of the vertex
    float height = fbm(position.xz * 0.1) * 20.0;
    
    // Add some smaller, sharper details
    height += noise(position.xz * 0.5) * 5.0;
    
    vec3 displacedPosition = vec3(position.x, position.y + height, position.z);

    v_normal = mat3(transpose(inverse(u_ModelMatrixLight))) * normal;
    fragPos = vec3(u_ModelMatrixLight * vec4(displacedPosition, 1.0f));

    vec4 newPosition = u_PerspectiveLight * u_ViewLight * u_ModelMatrixLight * vec4(displacedPosition, 1.0f);
    gl_Position = newPosition;
}