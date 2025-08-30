#shader vertex
#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 texturePosition;

uniform mat4 model;
uniform mat4 projectionView;

out vec2 texCoords;

void main() {
    gl_Position = projectionView * model * vec4(position, 1.0f);;
    texCoords = texturePosition;
}

#shader fragment
#version 330 core

in vec2 texCoords;
uniform sampler2D inputTexture;

out vec4 fragColor;

void main() {
    fragColor = texture(inputTexture, texCoords);
}