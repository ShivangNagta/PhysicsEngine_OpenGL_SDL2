#version 410 core

in vec2 v_textCoord;
in vec3 fragPos;


out vec4 FragColor;

float factor = 200.0;

uniform vec3 u_Color; // Uniform color for the terrain

void main()
{
    // Use a uniform color or perform any additional coloring here
    // If you want to use a single color, set it directly or modify it
    FragColor = vec4(0.5 + (sin(fragPos.y )) / 2.0, 0.5 + (sin(fragPos.y)) / 2.0, 0.5 + (sin(fragPos.y)) / 2.0, 1.0); // Set the color with full opacity
    // FragColor = vec4(fragPos.y / factor, fragPos.y / factor, fragPos.y / factor , 1.0); // Set the color with full opacity
}
