#version 410 core

in vec2 v_textCoord;
out vec4 color;

uniform sampler2D myTexture;

void main()
{
    color = texture(myTexture, v_textCoord);
}
