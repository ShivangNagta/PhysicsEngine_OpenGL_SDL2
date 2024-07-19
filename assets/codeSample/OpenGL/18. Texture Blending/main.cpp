//Third Party
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

//C++ STL
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

//My Custom Libraries
#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "Texture2D.hpp"

//test
const std::string texture1FileName = "./assets/textures/airplane.PNG";
const std::string texture2FileName = "./assets/textures/crate.jpg";

const int gScreenWidth = 640;
const int gScreenHeight = 480;
SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr; 
bool gQuit = false;

GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;
GLuint gElementBufferObject = 0;


float g_uOffset = 0.01f;
float g_uRotate = 0.01f;
float g_uScale = 0.5f;

// Global Camera;
Camera g_camera;
Texture2D texture1;
Texture2D texture2;

// FPS
void ShowFPS(SDL_Window*){
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;

    double currentSeconds = SDL_GetTicks() / 1000.0;

    elapsedSeconds = currentSeconds - previousSeconds;

    //Update 4 times a second
    if (elapsedSeconds > 0.25){
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed << "  "
             << "FPS: " << fps << "  "
             << "Frame Time: " << msPerFrame << " (ms)";
        SDL_SetWindowTitle(gGraphicsApplicationWindow, outs.str().c_str());
        frameCount = 0;   
    }
    frameCount++;

}


void VertexSpecification(){
    const std::vector<GLfloat> vertexData{
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
      
    };

    const std::vector<GLint> indexData{0, 1, 2, 3, 0, 2};

    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                vertexData.size() * sizeof(GLfloat),
                vertexData.data(),
                GL_STATIC_DRAW);

    glGenBuffers(1, &gElementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                indexData.size() * sizeof(GLint),
                indexData.data(),
                GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          5*sizeof(GL_FLOAT),
                          (void*)0);
 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          5*sizeof(GL_FLOAT),
                          (GLvoid*)(sizeof(GL_FLOAT)*3));

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Init(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    gGraphicsApplicationWindow = SDL_CreateWindow("OpenGL",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            gScreenWidth, gScreenHeight,
                            SDL_WINDOW_OPENGL);

    if(gGraphicsApplicationWindow == nullptr){
        std::cerr << "SDL_Window was not created" << std::endl;
        exit(1);
    }

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
    
    if(gOpenGLContext == nullptr){
        std::cerr << "OpenGL context not available\n";
        exit(1);
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }

    // V-Sync Enabled for FPS restriced with monitor fresh rate
    // if (SDL_GL_SetSwapInterval(0) < 0) {
    //     std::cerr << "Warning: Unable to disable VSync! SDL Error: " << SDL_GetError() << std::endl;
    // }

}


void Input(){
    SDL_Event e;
    static int mouseX = gScreenWidth / 2;
    static int mouseY = gScreenHeight / 2;
    static bool wireframeMode = false;
   
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case (SDL_QUIT):
            gQuit = true;
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                gQuit = true;
            }
            if (e.key.keysym.sym == SDLK_1) {
                wireframeMode = !wireframeMode;
                if (wireframeMode) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                } else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
            break;
        case (SDL_MOUSEMOTION):
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            g_camera.MouseLook(mouseX, mouseY );
            break;
        
        }
    }
    
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    float speed = 0.01f;
    if(state[SDL_SCANCODE_W]){
        g_camera.MoveForward(speed);
    }
    if(state[SDL_SCANCODE_S]){
        g_camera.MoveBackward(speed);
    }
    if(state[SDL_SCANCODE_A]){
        g_camera.MoveLeft(speed);
    }
    if(state[SDL_SCANCODE_D]){
        g_camera.MoveRight(speed);
    }
    if(state[SDL_SCANCODE_SPACE]){
        g_camera.MoveUp(speed);
    }
    if(state[SDL_SCANCODE_LCTRL]){
        g_camera.MoveDown(speed);
    }
}

void PreDraw(){
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    texture1.loadTexture(texture1FileName, true);
    texture2.loadTexture(texture2FileName, true);



    ShaderProgram shaderProgram;
    shaderProgram.LoadShader("./shaders/vert.glsl", "./shaders/frag.glsl");
    GLuint gGraphicsPipelineShaderProgram = shaderProgram.GetProgramID();
    shaderProgram.Use();

    texture1.bind(0);
    texture2.bind(1);
    GLint u_Texture1Location = glGetUniformLocation(gGraphicsPipelineShaderProgram, "myTexture1");
    glUniform1i(u_Texture1Location, 0);
    GLint u_Texture2Location = glGetUniformLocation(gGraphicsPipelineShaderProgram, "myTexture2");
    glUniform1i(u_Texture2Location, 1);


    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    g_uRotate += 0.2f;
    
    // Local to World Space
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f +g_uOffset));         
    model           = glm::rotate(model ,glm::radians(g_uRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    model           = glm::scale(model, glm::vec3(g_uScale, g_uScale, g_uScale));

    GLint u_ModelMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ModelMatrix");
    glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);

    //View Matrix
    glm::mat4 view = g_camera.GetViewMatrix();
    GLint u_ViewLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_View");
    if(u_ViewLocation >= 0){
        glUniformMatrix4fv(u_ViewLocation, 1, GL_FALSE, &view[0][0]);
    }
    else{
        std::cout << "u_View errrrrrr!!!" << std::endl;
        exit(EXIT_FAILURE);
    }


    // Persepective Projection
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), 
                                            (float)gScreenWidth/(float)gScreenHeight,
                                            0.1f,
                                            10.0f);

    GLint u_PerspectiveLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_Perspective");

    glUniformMatrix4fv(u_PerspectiveLocation, 1, GL_FALSE, &perspective[0][0]);

}

void Draw() {   
    glBindVertexArray(gVertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    SDL_GL_SwapWindow(gGraphicsApplicationWindow);
}

void MainLoop(){

    SDL_WarpMouseInWindow(gGraphicsApplicationWindow, gScreenWidth/2, gScreenHeight/2);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!gQuit) {
        
        ShowFPS(gGraphicsApplicationWindow);

        Input();
        PreDraw();
        Draw();

        
    }
}

void CleanUp(){
    SDL_GL_DeleteContext(gOpenGLContext);
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Init();

    VertexSpecification();

    MainLoop();

    CleanUp();
    return 0;
}


