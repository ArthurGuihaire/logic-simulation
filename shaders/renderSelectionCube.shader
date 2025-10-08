#shader vertex
#version 330 core

layout(location=0) in vec3 position;

uniform mat4 model;
uniform mat4 projectionView;
out vec3 vertexColor;

void main() {
    gl_Position = projectionView * vec4(position, 1.0f);
    vertexColor = (position + vec3(0.5f, 0.5f, 0.5f));
}

#shader fragment
#version 330 core

uniform vec4 color;

in vec3 vertexColor;
out vec4 fragColor;

void main() {
    fragColor = vec4(vertexColor, 0.5f);
}
