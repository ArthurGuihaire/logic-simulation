#shader vertex
#version 330 core

layout(location=0) in vec3 position;

uniform mat4 model;
uniform mat4 projectionView;

out vec2 texCoords;

void main() {
    gl_Position = projectionView * model * vec4(position, 1.0f);
}

#shader fragment
#version 330 core

uniform vec4 color;

out vec4 fragColor;

void main() {
    fragColor = outputColor;
}