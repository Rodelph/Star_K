#include "./headers/Callbacks.h"
#include "./headers/Input.h"
#include "./headers/Utilities.h"
#include "./headers/Vertex.h"
#include "./headers/DrawDetails.h"
#include "./headers/MeshLoader.h"
#include "./headers/Draw.h"
#include "./headers/ShaderLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "./headers/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "./headers/Version.h"
#include "./headers/QueryShader.h"
#include <fstream>
#include <SFML/Audio.hpp>

#define WINDOW_SIZE_WIDTH 800
#define WINDOW_SIZE_HEIGHT 800
#define stepAxis 1/6
#define FPS 75

const int Radius = 6;
static int window_width, window_height;

// Compile with " g++ me.cpp -lglfw ./c_file/glad.o -ldl -lsfml-audio -o gameDev "    and then     ./gameDev

void getScreenResolution();
void calculateFps();

int main(int argc, char** argv){
    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    
    sf::Music music;
    if (!music.openFromFile("../res_music/music.wav"))
        return -1; // error
    music.play();

    if (!glfwInit()) {
        throw ("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    getScreenResolution();

    GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT, extract_prog_name(argv[0]), nullptr, nullptr);
    glfwSetWindowPos(window, window_width / 2 - WINDOW_SIZE_WIDTH / 2, window_height / 2 - WINDOW_SIZE_HEIGHT / 2);

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }

    printGLinfo();

    glfwSetWindowCloseCallback(window, glfw_window_close_callback);
    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

    std::string vertShader = ReadToString("./GLSL/VertexShader.glsl");
    std::string fragShader = ReadToString("./GLSL/FragmentShader.glsl");

    unsigned int mainShader = LoadShader(vertShader.c_str(), fragShader.c_str());

	glClearColor(.2f, .2f, .6f, 0.f);

    std::vector<DrawDetails> ourDrawDetails;
	  
    const float posData[] = {
            (float)-1*stepAxis, (float) 3*stepAxis, (float) 0*stepAxis,   //A 0
            (float) 0*stepAxis, (float) 0*stepAxis, (float) 0*stepAxis,   //B 1
            (float) 0*stepAxis, (float) 6*stepAxis, (float) 0*stepAxis,   //C 2
            (float) 1*stepAxis, (float) 3*stepAxis, (float) 0*stepAxis,   //D 3
            (float)-3*stepAxis, (float) 1*stepAxis, (float) 0*stepAxis,   //E 4
            (float)-6*stepAxis, (float) 0*stepAxis, (float) 0*stepAxis,   //F 5
            (float)-3*stepAxis, (float)-1*stepAxis, (float) 0*stepAxis,   //G 6
            (float) 0*stepAxis, (float)-6*stepAxis, (float) 0*stepAxis,   //H 7
            (float) 6*stepAxis, (float) 0*stepAxis, (float) 0*stepAxis,   //I 8
            (float) 3*stepAxis, (float) 1*stepAxis, (float) 0*stepAxis,   //J 9
            (float) 3*stepAxis, (float)-1*stepAxis, (float) 0*stepAxis,   //K 10
            (float) 1*stepAxis, (float)-3*stepAxis, (float) 0*stepAxis,   //L 11
            (float)-1*stepAxis, (float)-3*stepAxis, (float) 0*stepAxis,   //M 12
            (float)-4*stepAxis, (float) 4*stepAxis, (float) 0*stepAxis,   //N 13
            (float) 4*stepAxis, (float) 4*stepAxis, (float) 0*stepAxis,   //O 14
            (float) 4*stepAxis, (float)-4*stepAxis, (float) 0*stepAxis,   //P 15
            (float)-4*stepAxis, (float)-4*stepAxis, (float) 0*stepAxis    //Q 16
            };
    
    const float ColorData[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,

            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,

            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
    };

    const GLuint elem[] = { 1, 0, 2,
                            1, 3, 2,

                            1, 3, 14,
                            1, 10, 14,

                            1, 9, 8,
                            1, 10, 8,

                            1, 10, 15,
                            1, 11, 15,
                            
                            1, 11, 7,
                            1, 12, 7,
                            
                            1, 12, 16,
                            1, 6, 16,
                            
                            1, 6, 5,
                            1, 4, 5,
                            
                            1, 4, 13,
                            1, 0, 13
                        };

    ourDrawDetails.push_back(UploadMesh(posData, 
                                        ColorData,
                                        sizeof(posData)/sizeof(posData[0]), 
                                        elem, sizeof(elem)/sizeof(elem[0])));
    

	QueryAttribs(mainShader);
    QueryUniforms(mainShader);

    while(!glfwWindowShouldClose(window)) {
        calculateFps();
        ProcessInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(mainShader);

        glm::mat4 finalModelMatix = glm::mat4(1);
        finalModelMatix = glm::translate(finalModelMatix, glm::vec3(sin((float)glfwGetTime()) / 2, cos((float)glfwGetTime()) / 2, 0));
        finalModelMatix = glm::rotate(finalModelMatix, (float)glfwGetTime(), glm::vec3(0.f, 1.f, 0.f));
        finalModelMatix = glm::scale(finalModelMatix, glm::vec3(.5));
        GLuint location = glGetUniformLocation(mainShader, "uModelMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, &finalModelMatix[0][0]);
        for(const auto& thing : ourDrawDetails){
            Draw(ourDrawDetails);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    UnloadMesh(ourDrawDetails);
    glfwTerminate();
    return 0;
}

void getScreenResolution(){
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window_width = mode->width;
    window_height = mode->height;
}

void calculateFps(){
    static float framesPerSecond = 0.0f;
    static double lastTime = 0.0;
    static int fps;

    float time = glfwGetTime();
    float deltaTime = time - lastTime;
    ++framesPerSecond;
    if(deltaTime >= 1/FPS){
        lastTime = time;
        fps = (int)framesPerSecond * FPS;
        //std::cout << fps << "\n";
        framesPerSecond = 0;
    }
}