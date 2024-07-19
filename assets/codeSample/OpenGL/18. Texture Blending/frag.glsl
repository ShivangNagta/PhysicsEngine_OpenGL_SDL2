#version 410 core

in vec2 v_textCoord;
out vec4 color;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

void main()
{
    color = mix(texture(myTexture1, v_textCoord), texture(myTexture2, v_textCoord), 0.5);
}
