#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_Perspective;
uniform mat4 u_View;
uniform float time;  // Uniform for time

out vec2 v_textCoord;
out vec3 v_normal;
out vec3 fragPos;

void main()
{
    v_textCoord = textCoord;
    v_normal = normal;

    // Parameters for the first wave
    float waveHeight1 = 0.65;
    float waveFrequency1 = 2.0;
    float waveSpeed1 = 4.0;

    // Parameters for the second wave
    float waveHeight2 = 0.67;
    float waveFrequency2 = 5.5;
    float waveSpeed2 = 3.5;

    // Parameters for the third wave
    float waveHeight3 = 0.8;
    float waveFrequency3 = 3.5;
    float waveSpeed3 = 2.8;

    // Combine multiple sine waves
    vec3 modifiedPosition = position;
    modifiedPosition.y += waveHeight1 * sin(waveFrequency1 * (position.x + position.z) + waveSpeed1 * time);
    modifiedPosition.y += waveHeight2 * sin(waveFrequency2 * (position.x - position.z) + waveSpeed2 * time + 1.0); // Phase shift
    modifiedPosition.y += waveHeight3 * sin(waveFrequency3 * (position.x + position.z * 0.5) + waveSpeed3 * time - 0.5); // Different orientation

    fragPos = vec3(u_ModelMatrix * vec4(modifiedPosition, 1.0f));
    vec4 newPosition = u_Perspective * u_View * u_ModelMatrix * vec4(modifiedPosition, 1.0f);
    gl_Position = newPosition;
}
