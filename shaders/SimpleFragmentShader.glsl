#version 330 core

// 정점 셰이더에서 넘겨준 보간 값.
in vec2 UV;

// 출력 데이터
out vec3 color;

// 한 메쉬를 그리는 동안 일정하게 유지되는 값.
uniform sampler2D myTextureSampler;

void main() {
    // Output color = 지정된 UV에서 텍스처의 색.
    color = texture(myTextureSampler, UV).rgb;
}