#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <fstream>

const int gScreenWidth = 640;
const int gScreenHeight = 480;
SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr; 
bool gQuit = false;

std::string LoadShaderAsString(const std::string& filename){
    std::string result = "";
    std::string line = "";
    std::ifstream myFile(filename.c_str());
    
    if(myFile.is_open()){
        while(std::getline(myFile, line)){
            result += line + "\n";
        };
        myFile.close();
    }
    return result;
}


GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;

GLuint gGraphicsPipelineShaderProgram = 0;

GLuint CompileShader(GLuint type, const std::string& source){
    GLuint shaderObject;

    if(type == GL_VERTEX_SHADER){
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }else if(type == GL_FRAGMENT_SHADER){
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vertexshadersource,
                        const std::string& fragmentshadersource){
    GLuint programObject = glCreateProgram();

    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexshadersource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentshadersource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    glValidateProgram(programObject);

    return programObject;

}

void CreateGraphicsPipeline(){
    std::string vertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shaders/frag.glsl");
    gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
};

void GetOpenGLVersionInfo(){
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void VertexSpecification(){
    const std::vector<GLfloat> vertexData{
        -0.5f, -0.5f, 0.0f,
         1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.0f,  0.0f, 1.0f,

         0.5f,  0.5f, 0.0f,
         1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  1.0f, 0.0f,
    };


    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                vertexData.size() * sizeof(GLfloat),
                vertexData.data(),
                GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6*sizeof(GL_FLOAT),
                          (void*)0);
 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6*sizeof(GL_FLOAT),
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

    GetOpenGLVersionInfo();
}

void Input(){
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            gQuit = true;
            break;
        }
    }
}

void PreDraw(){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);
}

void Draw() {
    glBindVertexArray(gVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    SDL_GL_SwapWindow(gGraphicsApplicationWindow);
}

void MainLoop(){
    while (!gQuit) {
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

    CreateGraphicsPipeline();

    MainLoop();

    CleanUp();
    return 0;
}
