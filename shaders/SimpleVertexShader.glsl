#version 330 core

// 입력 정점 데이터, 이 셰이더가 실행할 때마다 달라집니다. (각 정점마다 셰이더가 한번씩 실행되요.)
layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 1) in vec2 vertexUV;

// 출력 데이터 ; 각 픽셀마다 알아서 보간될거에요.
out vec2 UV;

// 이 변수는 '매쉬당' 상수에요.
// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main() {
    // 정점의 출력 위치 = MVP(Model View Projection) * position;
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

    // 정점의 UV. 특별한 건 없음.
    UV = vertexUV;
}