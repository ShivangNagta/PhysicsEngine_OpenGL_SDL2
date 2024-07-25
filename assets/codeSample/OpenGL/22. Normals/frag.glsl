#version 410 core

in vec2 v_textCoord;
out vec4 color;

uniform sampler2D myTexture1;

void main()
{
    color = texture(myTexture1, v_textCoord);
}
