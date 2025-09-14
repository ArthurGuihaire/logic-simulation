#shader vertex
#version 330 core

layout(location=0) in vec3 position;

uniform mat4 projectionView;

void main() {
    gl_Position = projectionView * vec4(position, 1.0f);
    //gl_Position = vec4(position, 1.0f);
}

#shader fragment
#version 330 core

uniform vec4 color;

out vec4 fragColor;

void main() {
    fragColor = color;
}
