#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_Perspective;
uniform mat4 u_View;
uniform float u_Seed;

out vec2 v_textCoord;
out vec3 v_normal;
out vec3 fragPos;

// // Function to generate a random gradient vector based on grid coordinates and seed
// vec2 randomGradient(int ix, int iy) {
//     const uint w = 8 * uint(32); // 32-bit unsigned int
//     const uint s = w / 2;
//     uint a = uint(ix) + uint(u_Seed * 10000.0); // Incorporate u_Seed into the random number generation
//     uint b = uint(iy) + uint(u_Seed * 10000.0); // Incorporate u_Seed into the random number generation
//     a *= 3284157443u;

//     b ^= a << s | a >> (w - s);
//     b *= 1911520717u;

//     a ^= b << s | b >> (w - s);
//     a *= 2048419325u;

//     float random = (float(a)) * (3.14159265 / float(0xFFFFFFFFu)); // Convert to [0, 2*Pi]

//     // Create the vector from the angle
//     return vec2(sin(random), cos(random));
// }


vec2 randomGradient(int ix, int iy) {
    float random = fract(sin(dot(vec2(ix, iy) * 0.1, vec2(12.9898, 78.233))) * 43758.5453 * u_Seed);
    float angle = random * 2.0 * 3.14159265; // Convert to angle between 0 and 2*Pi
    return vec2(sin(angle), cos(angle));
}

// // Function to generate a random gradient vector based on grid coordinates and seed
// vec2 randomGradient(int ix, int iy) {

//     uint a = uint(ix) + uint(u_Seed * 10000.0); // Incorporate u_Seed into the random number generation
//     uint b = uint(iy) + uint(u_Seed * 10000.0); // Incorporate u_Seed into the random number generation

//     float random = (float((a * b)) * (3.14159265 / float(0xFFFFFFFFu)); // Convert to [0, 2*Pi]

//     // Create the vector from the angle
//     return vec2(sin(random), cos(random));
// }

// Computes the dot product of the distance and gradient vectors
float dotGridGradient(int ix, int iy, float x, float y) {
    vec2 gradient = randomGradient(ix, iy);

    // Compute the distance vector
    float dx = x - float(ix);
    float dy = y - float(iy);

    // Compute the dot-product
    return (dx * gradient.x + dy * gradient.y);
}

// Interpolation function ( Hermite interpolation)
float interpolate(float a0, float a1, float w) {
    return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

// Sample Perlin noise at coordinates (x, y)
float perlin(float x, float y) {
    // Determine grid cell corner coordinates
    int x0 = int(floor(x));
    int y0 = int(floor(y));
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // Compute interpolation weights
    float sx = x - float(x0);
    float sy = y - float(y0);

    // Interpolate top two corners
    float n0 = dotGridGradient(x0, y0, x, y);
    float n1 = dotGridGradient(x1, y0, x, y);
    float ix0 = interpolate(n0, n1, sx);

    // Interpolate bottom two corners
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    float ix1 = interpolate(n0, n1, sx);

    // Final interpolation
    return interpolate(ix0, ix1, sy);
}

void main() {
    v_textCoord = textCoord;

    float height = 0.0;

    float freq = 2.0;
    float amp = 300.5;

    // Generate terrain height with multiple octaves of Perlin noise
    for (int i = 0; i < 12; i++) {
        height += perlin(position.x * freq, position.z * freq) * amp;
        freq *= 2.0;
        amp *= 0.5;
    }

    // Apply the computed height to the vertex position
    vec3 displacedPosition = vec3(position.x, position.y + height, position.z);

    // Transform the normal and position
    v_normal = mat3(transpose(inverse(u_ModelMatrix))) * normal;
    fragPos = vec3(u_ModelMatrix * vec4(displacedPosition, 1.0f));

    // Apply the model, view, and perspective matrices to get the final vertex position
    vec4 newPosition = u_Perspective * u_View * u_ModelMatrix * vec4(displacedPosition, 1.0f);
    gl_Position = newPosition;
}

