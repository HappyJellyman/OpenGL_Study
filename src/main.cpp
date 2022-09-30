#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

extern GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path);

static constexpr int WIDTH = 1600;
static constexpr int HEIGHT = 900;

GLFWwindow *init()
{
    if (!glfwInit())
    {
        cout << "GLFW 초기화 실패!" << endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window
    GLFWwindow *window;
    window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Sample Project", NULL, NULL);
    if (window == NULL)
    {
        cout << "GLFW 윈도우 생성 실패!" << endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;

    if (glewInit() != GLEW_OK)
    {
        cout << "GLFW 윈도우 생성 실패!" << endl;
        glfwTerminate();
        return nullptr;
    }

    cout << glGetString(GL_VERSION) << endl;
    return window;
}

int main()
{
    GLFWwindow *window = init();
    if (window == nullptr)
    {
        return -1;
    }

    // VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Shader
    GLuint programID = LoadShaders("shaders\\SimpleVertexShader.glsl", "shaders\\SimpleFragmentShader.glsl");

    // 우리의 "MVP" 행렬에 참조를 얻습니다.
    // 아. 초기화때만 하셔야 해요.
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // 깊이 버퍼
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // 우리 Vertex들입니다. 3개의 연속 된 float 숫자가 하나의 3차원 Vertex입니다.
    // 3개의 연속 된 Vertex들이 하나의 삼각형을 정의합니다.
    // 하나의 육면체는 2개의 삼각형으로 이루어진 면 6개를 가지고 있고,
    // 그래서 총 6*2=12개의 삼각형, 12*3 개의 Vertex를 가지고 있습니다.
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, -1.0f, // triangle 1 : begin
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f, -1.0f, // triangle 2 : begin
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f, // triangle 2 : end
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

    // 이것이 우리의 버텍스 버퍼를 가리킵니다.
    GLuint vertexbuffer;
    // 버퍼를 하나 생성합니다. vertexbuffer 에 결과 식별자를 넣습니다
    glGenBuffers(1, &vertexbuffer);
    // 아래의 명령어들은 우리의 "vertexbuffer" 버퍼에 대해서 다룰겁니다
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // 우리의 버텍스들을 OpenGL로 넘겨줍니다
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // One color for each vertex. They were generated randomly.
    static const GLfloat g_color_buffer_data[] = {
        0.583f, 0.771f, 0.014f,
        0.609f, 0.115f, 0.436f,
        0.327f, 0.483f, 0.844f,
        0.822f, 0.569f, 0.201f,
        0.435f, 0.602f, 0.223f,
        0.310f, 0.747f, 0.185f,
        0.597f, 0.770f, 0.761f,
        0.559f, 0.436f, 0.730f,
        0.359f, 0.583f, 0.152f,
        0.483f, 0.596f, 0.789f,
        0.559f, 0.861f, 0.639f,
        0.195f, 0.548f, 0.859f,
        0.014f, 0.184f, 0.576f,
        0.771f, 0.328f, 0.970f,
        0.406f, 0.615f, 0.116f,
        0.676f, 0.977f, 0.133f,
        0.971f, 0.572f, 0.833f,
        0.140f, 0.616f, 0.489f,
        0.997f, 0.513f, 0.064f,
        0.945f, 0.719f, 0.592f,
        0.543f, 0.021f, 0.978f,
        0.279f, 0.317f, 0.505f,
        0.167f, 0.620f, 0.077f,
        0.347f, 0.857f, 0.137f,
        0.055f, 0.953f, 0.042f,
        0.714f, 0.505f, 0.345f,
        0.783f, 0.290f, 0.734f,
        0.722f, 0.645f, 0.174f,
        0.302f, 0.455f, 0.848f,
        0.225f, 0.587f, 0.040f,
        0.517f, 0.713f, 0.338f,
        0.053f, 0.959f, 0.120f,
        0.393f, 0.621f, 0.362f,
        0.673f, 0.211f, 0.457f,
        0.820f, 0.883f, 0.371f,
        0.982f, 0.099f, 0.879f};

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    // 프로젝션 매트릭스 : 45도 시야각, 4:3 비율, 시야 범위 : 0.1 유닛 <--> 100 유닛
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    // 혹은 ortho(직교) 카메라에선 :
    // glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // 월드 좌표로 표현

    // 카메라 매트릭스
    glm::mat4 View = glm::lookAt(
        glm::vec3(4, 3, 3), // 카메라는 (4,3,3) 에 있다. 월드 좌표에서
        glm::vec3(0, 0, 0), // 그리고 카메라가 원점을 본다
        glm::vec3(0, 1, 0)  // 머리가 위쪽이다 (0,-1,0 으로 해보면, 뒤집어 볼것이다)
    );

    // 모델 매트릭스 : 단위 매트릭스 (모델은 원점에 배치된다)
    glm::mat4 Model = glm::mat4(1.0f);
    // 우리의 모델뷰프로젝션 : 3개 매트릭스들을 곱한다
    glm::mat4 mvp = Projection * View * Model; // 기억하세요, 행렬곱은 계산은 반대순서로 이루어집니다

    // 입력
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        // 현재 바인딩된 쉐이더에게 변환한 메트릭스를 보냅시다. 방금 얻은 참조로요.
        // 이건 각각의 모델마다 다른 MVP 행렬을 가지고 있을 것이니, 메인 루프에 해줍시다. (VP는 같을지 몰라도, M은 다를거에요.)
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        // 버퍼의 첫번째 속성값(attribute) : 버텍스들
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,        // 0번째 속성(attribute). 0 이 될 특별한 이유는 없지만, 쉐이더의 레이아웃(layout)와 반드시 맞추어야 합니다.
            3,        // 크기(size)
            GL_FLOAT, // 타입(type)
            GL_FALSE, // 정규화(normalized)?
            0,        // 다음 요소 까지 간격(stride)
            (void *)0 // 배열 버퍼의 오프셋(offset; 옮기는 값)
        );

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,        // attribute. No particular reason for 1, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        // 삼각형 그리기!
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 버텍스 0에서 시작해서; 총 3개의 버텍스로 -> 하나의 삼각형
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    return 0;
}