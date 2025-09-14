#shader vertex
#version 330 core

layout(location=0) in vec3 position;

void main() {
    gl_Position = vec4(position, 1.0f);
}

#shader fragment
#version 330 core

out vec4 fragColor;

void main() {
    fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
