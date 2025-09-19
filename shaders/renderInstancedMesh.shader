#shader vertex
#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;
//model matrix: 4 columns
layout(location=2) in vec4 modelColumn1;
layout(location=3) in vec4 modelColumn2;
layout(location=4) in vec4 modelColumn3;
layout(location=5) in vec4 modelColumn4;

uniform mat4 projectionView;
flat out vec4 vertexColor;

void main() {
    mat4 model = mat4(modelColumn1, modelColumn2, modelColumn3, modelColumn4);
    gl_Position = projectionView * model * vec4(position, 1.0f);
    vertexColor = color;
}

#shader fragment
#version 330 core

flat in vec4 vertexColor;
out vec4 fragColor;

void main() {
    fragColor = vertexColor;
}
