#version 410 core

in vec2 v_textCoord;


out vec4 FragColor;

uniform vec3 u_Color; // Uniform color for the terrain

void main()
{
    // Use a uniform color or perform any additional coloring here
    // If you want to use a single color, set it directly or modify it
    FragColor = vec4(u_Color, 1.0); // Set the color with full opacity
}
