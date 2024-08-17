#version 410 core

in vec2 v_textCoord;
in vec3 fragPos;

out vec4 FragColor;

float factor = 2000.0;

vec3 getColor(float height)
{
    if (height < 0.2)
        return vec3(0.0, 0.0, 0.5); // Deep water
    else if (height < 0.4)
        return vec3(0.0, 0.5, 0.0); // Shallow water
    else if (height < 0.6)
        return vec3(0.5, 0.5, 0.0); // Sand
    else if (height < 0.8)
        return vec3(0.0, 0.5, 0.0); // Grass
    else
        return vec3(1.0, 1.0, 1.0); // Snow
}

void main()
{
    // Normalize height between 0 and 1
    float heightFactor = (fragPos.y + factor) / (2.0 * factor);
    heightFactor = clamp(heightFactor, 0.0, 1.0);
    
    vec3 color = getColor(heightFactor);
    
    FragColor = vec4(color, 1.0); // Set the color with full opacity
}










// #version 410 core

// in vec2 v_textCoord;
// in vec3 fragPos;


// out vec4 FragColor;

// float factor = 2000.0;
// float innerFactor = 40.0;

// uniform vec3 u_Color; // Uniform color for the terrain

// void main()
// {
//     // Use a uniform color or perform any additional coloring here
//     // If you want to use a single color, set it directly or modify it
//     // FragColor = vec4(0.5 + (sin(fragPos.y / innerFactor )) / 2.0, 0.5 + (sin(fragPos.y / innerFactor)) / 2.0, 0.5 + (sin(fragPos.y / innerFactor)) / 2.0, 1.0); // Set the color with full opacity
//     // FragColor = vec4(fragPos.y / factor, fragPos.y / factor, fragPos.y / factor , 1.0); // Set the color with full opacity
//     FragColor = vec4(fragPos.y / factor, 0.3, 0.0 , 1.0); // Set the color with full opacity
// }


// #version 410 core

// in vec2 v_textCoord;
// in vec3 fragPos;

// out vec4 FragColor;


// float factor = 2000.0;

// void main()
// {
//     vec3 u_ColorLow = vec3(0.0, 0.0, 0.0);
//     vec3 u_ColorHigh = vec3(1.0, 1.0, 1.0);
//     // Normalize height between 0 and 1
//     float heightFactor = (fragPos.y + factor) / (2.0 * factor);
//     heightFactor = clamp(heightFactor, 0.0, 1.0);
    
//     // Linear interpolation between two colors
//     vec3 color = mix(u_ColorLow, u_ColorHigh, heightFactor);
    
//     FragColor = vec4(color, 1.0); // Set the color with full opacity
// }
