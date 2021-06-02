#include "./headers/glad.h"
#include <GLFW/glfw3.h>
#include "./headers/Callbacks.h"
#include "./headers/Utilities.h"
#include "./headers/Vertex.h"
#include <vector>
#include "./headers/OpenGLLoader.h"
#include "./headers/OpenGLDraw.h"
#include "./headers/Input.h"
#include "./headers/GLSLShaderLoader.h"
#include <random>
#include <math.h>
#include <SFML/Audio.hpp>

#define WINDOW_SIZE_WIDTH 800
#define WINDOW_SIZE_HEIGHT 800
#define stepAxis 1/6

#define FPS 60

const int Radius = 6;
int window_width, window_height;

// Compile with " g++ me.cpp -lglfw ./c_file/glad.o -ldl -lsfml-audio -o gameDev "    and then     ./gameDev
float calculateRotationX(float variable);
float calculateRotationY(float variable);
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

    getScreenResolution();
    GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT, extract_version(argv[0]), nullptr, nullptr);
    glfwSetWindowPos(window, window_width / 2 - WINDOW_SIZE_WIDTH / 2, window_height / 2 - WINDOW_SIZE_HEIGHT / 2);

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }

    glfwSetWindowCloseCallback(window, glfw_window_close_callback);
    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

    const char* vertexShader = 
    "#version 460 core                                                   \n"
    "layout(location = 0) in vec3 vertexPosition_modelspace;             \n"
    //"uniform mat4 modelmatrix => matrix * vec4(vertexPosition_modelspace, 1.0);\n"
    "void main(){                                                        \n"
    "   gl_Position = vec4(vertexPosition_modelspace, 1.0);\n"
    "}";

    const char* fragmentShader = 
    "#version 460 core   \n"
    "out vec3 color;     \n"
    "uniform vec3 ucolor = vec3(0.5, 0.5, 0.5);\n"
    "void main(){        \n"
    "   color = ucolor;  \n"
    "}";

    unsigned int mainShader = LoadShader(vertexShader, fragmentShader);

    //Fix rotation

    std::vector<DrawDetails> ourDrawDetails;
	{
		std::vector<Vertex> obj_pts;
        
        obj_pts.emplace_back( (float)-1*stepAxis, (float) 3*stepAxis, (float) 0*stepAxis);   //A 0
        obj_pts.emplace_back( (float) 0*stepAxis, (float) 0*stepAxis, (float) 0*stepAxis);   //B 1
        obj_pts.emplace_back( (float) 0*stepAxis, (float) 6*stepAxis, (float) 0*stepAxis);   //C 2
        obj_pts.emplace_back( (float) 1*stepAxis, (float) 3*stepAxis, (float) 0*stepAxis);   //D 3
        obj_pts.emplace_back( (float)-3*stepAxis, (float) 1*stepAxis, (float) 0*stepAxis);   //E 4
        obj_pts.emplace_back( (float)-6*stepAxis, (float) 0*stepAxis, (float) 0*stepAxis);   //F 5
        obj_pts.emplace_back( (float)-3*stepAxis, (float)-1*stepAxis, (float) 0*stepAxis);   //G 6
        obj_pts.emplace_back( (float) 0*stepAxis, (float)-6*stepAxis, (float) 0*stepAxis);   //H 7
        obj_pts.emplace_back( (float) 6*stepAxis, (float) 0*stepAxis, (float) 0*stepAxis);   //I 8
        obj_pts.emplace_back( (float) 3*stepAxis, (float) 1*stepAxis, (float) 0*stepAxis);   //J 9
        obj_pts.emplace_back( (float) 3*stepAxis, (float)-1*stepAxis, (float) 0*stepAxis);   //K 10
        obj_pts.emplace_back( (float) 1*stepAxis, (float)-3*stepAxis, (float) 0*stepAxis);   //L 11
        obj_pts.emplace_back( (float)-1*stepAxis, (float)-3*stepAxis, (float) 0*stepAxis);   //M 12
        obj_pts.emplace_back( (float)-4*stepAxis, (float) 4*stepAxis, (float) 0*stepAxis);   //N 13
		obj_pts.emplace_back( (float) 4*stepAxis, (float) 4*stepAxis, (float) 0*stepAxis);   //O 14
        obj_pts.emplace_back( (float) 4*stepAxis, (float)-4*stepAxis, (float) 0*stepAxis);   //P 15
        obj_pts.emplace_back( (float)-4*stepAxis, (float)-4*stepAxis, (float) 0*stepAxis);   //Q 16
        
        
        /*

        obj_pts.emplace_back( calculateRotationX((float)-1*stepAxis), calculateRotationY((float) 3*stepAxis), (float) 0*stepAxis);   //A 0
        obj_pts.emplace_back( calculateRotationX((float) 0*stepAxis), calculateRotationY((float) 0*stepAxis), (float) 0*stepAxis);   //B 1
        obj_pts.emplace_back( calculateRotationX((float) 0*stepAxis), calculateRotationY((float) 6*stepAxis), (float) 0*stepAxis);   //C 2
        obj_pts.emplace_back( calculateRotationX((float) 1*stepAxis), calculateRotationY((float) 3*stepAxis), (float) 0*stepAxis);   //D 3
        obj_pts.emplace_back( calculateRotationX((float)-3*stepAxis), calculateRotationY((float) 1*stepAxis), (float) 0*stepAxis);   //E 4
        obj_pts.emplace_back( calculateRotationX((float)-6*stepAxis), calculateRotationY((float) 0*stepAxis), (float) 0*stepAxis);   //F 5
        obj_pts.emplace_back( calculateRotationX((float)-3*stepAxis), calculateRotationY((float)-1*stepAxis), (float) 0*stepAxis);   //G 6
        obj_pts.emplace_back( calculateRotationX((float) 0*stepAxis), calculateRotationY((float)-6*stepAxis), (float) 0*stepAxis);   //H 7
        obj_pts.emplace_back( calculateRotationX((float) 6*stepAxis), calculateRotationY((float) 0*stepAxis), (float) 0*stepAxis);   //I 8
        obj_pts.emplace_back( calculateRotationX((float) 3*stepAxis), calculateRotationY((float) 1*stepAxis), (float) 0*stepAxis);   //J 9
        obj_pts.emplace_back( calculateRotationX((float) 3*stepAxis), calculateRotationY((float)-1*stepAxis), (float) 0*stepAxis);   //K 10
        obj_pts.emplace_back( calculateRotationX((float) 1*stepAxis), calculateRotationY((float)-3*stepAxis), (float) 0*stepAxis);   //L 11
        obj_pts.emplace_back( calculateRotationX((float)-1*stepAxis), calculateRotationY((float)-3*stepAxis), (float) 0*stepAxis);   //M 12
        obj_pts.emplace_back( calculateRotationX((float)-4*stepAxis), calculateRotationY((float) 4*stepAxis), (float) 0*stepAxis);   //N 13
		obj_pts.emplace_back( calculateRotationX((float) 4*stepAxis), calculateRotationY((float) 4*stepAxis), (float) 0*stepAxis);   //O 14
        obj_pts.emplace_back( calculateRotationX((float) 4*stepAxis), calculateRotationY((float)-4*stepAxis), (float) 0*stepAxis);   //P 15
        obj_pts.emplace_back( calculateRotationX((float)-4*stepAxis), calculateRotationY((float)-4*stepAxis), (float) 0*stepAxis);   //Q 16
        
        */
		std::vector<uint32_t> elem = {1, 0, 2,
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
                                      1, 0, 13};
		ourDrawDetails.push_back(UploadMesh(obj_pts, elem));
    }
    glClearColor(.2f, .2f, .6f, 0.f);

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.f, 1.f);

    while(!glfwWindowShouldClose(window)) {
        ProcessInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(mainShader);
        float c1 = distribution(generator);
        float c2 = distribution(generator);
        float c3 = distribution(generator);
        uint32_t var = glGetUniformLocation(mainShader, "ucolor");
        glUniform3f(var, c1, c2, c3);
        calculateFps();
        Draw(ourDrawDetails);
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
        std::cout << fps << "\n";
        framesPerSecond = 0;
    }
}

float calculateRotationX(float variable){
    float pos = .0f;
    for(int degree = 0; degree <= 360; degree++){
        pos = Radius * cos(degree);
    }
    //pos = variable - pos;
    return pos;
}

float calculateRotationY(float variable){
    float pos = .0f;
    for(int degree = 0; degree <= 360; degree++){
        pos = Radius * sin(degree);
    }
    pos = variable - pos;
    return pos;
}