#version 410 core

in vec2 v_textCoord;
in vec3 v_normal;
in vec3 v_fragPos;

out vec4 color;

uniform sampler2D myTexture1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
    float ambientFactor = 0.1f;
    vec3 ambient = lightColor * ambientFactor;

    // lightCol * n.l
    vec3 normal = normalize(v_normal);
    vec3 lightDir = normalize(lightPos - v_fragPos);
    float NDotL = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = NDotL * lightColor;

    float shininess = 100.0f;
    float specularFactor = 1.0f;
    vec3 cameraDir = normalize(cameraPos - v_fragPos);
    vec3 half = normalize(cameraDir + lightDir);
    float NDotH = max(dot(normal, half), 0.0f);
    vec3 specular = pow(NDotH, shininess) * lightColor * specularFactor;
    vec4 texel = texture(myTexture1, v_textCoord);

    color = vec4(ambient + diffuse + specular, 1.0f) * texel;
}
