#version 330 core
layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 1) in vec3 vertexColor;
out vec3 fragmentColor;
// 이 값은 한 매쉬동안은 상수적입니다.
uniform mat4 MVP;
void main() {
    // 정점의 출력 좌표, clip space에선 : MVP * position
    // Output position of the vertex, in clip space : MVP * position
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

    // The color of each vertex will be interpolated
    // to produce the color of each fragment
    fragmentColor = vertexColor;
}