#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_Perspective;
uniform mat4 u_View;

out vec2 v_textCoord;
out vec3 v_normal;
out vec3 fragPos;

void main() {
    v_textCoord = textCoord;

    // Basic sine wave displacement for testing
    float height = sin(position.x * 0.1) * 5.0;
    vec3 displacedPosition = vec3(position.x, position.y + height, position.z);

    v_normal = mat3(transpose(inverse(u_ModelMatrix))) * normal;
    fragPos = vec3(u_ModelMatrix * vec4(displacedPosition, 1.0f));

    vec4 newPosition = u_Perspective * u_View * u_ModelMatrix * vec4(displacedPosition, 1.0f);
    gl_Position = newPosition;
}
