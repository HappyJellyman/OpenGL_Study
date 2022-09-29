#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

extern GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path);

static constexpr int WIDTH = 1600;
static constexpr int HEIGHT = 900;

int main()
{
    if (!glfwInit())
    {
        cout << "GLFW 초기화 실패!" << endl;
        return -1;
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
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;

    if (glewInit() != GLEW_OK)
    {
        cout << "GLFW 윈도우 생성 실패!" << endl;
        glfwTerminate();
        return -1;
    }

    cout << glGetString(GL_VERSION) << endl;

    // VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // 3 버텍스들을 표현하는 3 벡터들의 배열
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,
        -1.0f,
        0.0f,
        1.0f,
        -1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
    };

    // 이것이 우리의 버텍스 버퍼를 가리킵니다.
    GLuint vertexbuffer;
    // 버퍼를 하나 생성합니다. vertexbuffer 에 결과 식별자를 넣습니다
    glGenBuffers(1, &vertexbuffer);
    // 아래의 명령어들은 우리의 "vertexbuffer" 버퍼에 대해서 다룰겁니다
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // 우리의 버텍스들을 OpenGL로 넘겨줍니다
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // Shader 호출
    GLuint programID = LoadShaders("shaders\\SimpleVertexShader.glsl", "shaders\\SimpleFragmentShader.glsl");

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

    // 우리의 "MVP" 행렬에 참조를 얻습니다.
    // 아. 초기화때만 하셔야 해요.
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

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
        // 삼각형 그리기!
        glDrawArrays(GL_TRIANGLES, 0, 3); // 버텍스 0에서 시작해서; 총 3개의 버텍스로 -> 하나의 삼각형
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    return 0;
}