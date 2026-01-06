#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include "engine/render/FirstPersonCamera.h"

class ShaderDevApp {
private:
    static constexpr int WINDOW_WIDTH = 1280;
    static constexpr int WINDOW_HEIGHT = 720;
    
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
    bool running = true;
    
    FirstPersonCamera camera;
    
    GLuint VAO = 0, VBO = 0, EBO = 0;
    GLuint shaderProgram = 0;
    unsigned int cubeIndexCount = 0;
    
public:
    ShaderDevApp() 
        : camera(glm::vec3(0.0f, 2.0f, 3.0f))
    {
    }
    
    ~ShaderDevApp() {
        cleanup();
    }
    
    bool initialize() {
        std::cout << "[START] Shader Development Tool\n";
        
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "[ERROR] SDL Init failed: " << SDL_GetError() << "\n";
            return false;
        }
        std::cout << "[OK] SDL initialized\n";
        
        // Create window
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        
        window = SDL_CreateWindow(
            "Shader Development - Central Object",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );
        
        if (!window) {
            std::cerr << "[ERROR] Window creation failed: " << SDL_GetError() << "\n";
            SDL_Quit();
            return false;
        }
        std::cout << "[OK] Window created\n";
        
        // Create OpenGL context
        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            std::cerr << "[ERROR] OpenGL context failed: " << SDL_GetError() << "\n";
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }
        std::cout << "[OK] OpenGL context created\n";
        
        // Initialize GLEW
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (GLEW_OK != err) {
            std::cerr << "[ERROR] GLEW init failed\n";
            SDL_GL_DeleteContext(glContext);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }
        std::cout << "[OK] GLEW initialized\n";
        
        // Enable VSync
        SDL_GL_SetSwapInterval(1);
        
        // OpenGL setup
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Compile shaders
        if (!compileShaders()) {
            return false;
        }
        
        // Create cube
        if (!createCube()) {
            return false;
        }
        
        std::cout << "[INFO] Controls:\n";
        std::cout << "  W/A/S/D - Move camera\n";
        std::cout << "  Space/Ctrl - Move up/down\n";
        std::cout << "  Mouse - Look around\n";
        std::cout << "  1-9 - Switch shader variants\n";
        std::cout << "  R - Reload shaders\n";
        std::cout << "  ESC - Exit\n";
        
        return true;
    }
    
    bool compileShaders() {
        // Vertex shader
        const char* vertexShaderSource = R"(
            #version 120
            
            uniform mat4 view;
            uniform mat4 projection;
            uniform mat4 model;
            uniform float time;
            
            varying vec3 fragPos;
            varying vec3 fragNormal;
            varying vec2 fragTexCoord;
            
            void main() {
                fragPos = vec3(model * gl_Vertex);
                fragNormal = normalize(mat3(model) * gl_Normal);
                fragTexCoord = vec2(gl_MultiTexCoord0);
                
                gl_Position = projection * view * vec4(fragPos, 1.0);
            }
        )";
        
        // Fragment shader - Basic Phong
        const char* fragmentShaderSource = R"(
            #version 120
            
            uniform float time;
            
            varying vec3 fragPos;
            varying vec3 fragNormal;
            varying vec2 fragTexCoord;
            
            void main() {
                vec3 norm = normalize(fragNormal);
                vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
                
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 color = vec3(0.2 + 0.8 * diff);
                
                // Add time animation
                color += 0.2 * sin(time) * vec3(1.0, 0.5, 0.3);
                
                gl_FragColor = vec4(color, 1.0);
            }
        )";
        
        // Compile vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        
        // Check for vertex shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cerr << "[ERROR] Vertex shader compilation failed:\n" << infoLog << "\n";
            return false;
        }
        
        // Compile fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        
        // Check for fragment shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "[ERROR] Fragment shader compilation failed:\n" << infoLog << "\n";
            return false;
        }
        
        // Link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        
        // Check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cerr << "[ERROR] Shader program linking failed:\n" << infoLog << "\n";
            return false;
        }
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        std::cout << "[OK] Shaders compiled successfully\n";
        return true;
    }
    
    bool createCube() {
        // Cube vertices (position + normal)
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
        };
        
        std::vector<Vertex> vertices = {
            // Front face
            {{ -1.0f, -1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f }},
            {{  1.0f, -1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f }},
            {{  1.0f,  1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f }},
            {{ -1.0f,  1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f }},
            
            // Back face
            {{ -1.0f, -1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f }},
            {{ -1.0f,  1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f }},
            {{  1.0f,  1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f }},
            {{  1.0f, -1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f }},
            
            // Top face
            {{ -1.0f,  1.0f, -1.0f }, {  0.0f,  1.0f,  0.0f }},
            {{ -1.0f,  1.0f,  1.0f }, {  0.0f,  1.0f,  0.0f }},
            {{  1.0f,  1.0f,  1.0f }, {  0.0f,  1.0f,  0.0f }},
            {{  1.0f,  1.0f, -1.0f }, {  0.0f,  1.0f,  0.0f }},
            
            // Bottom face
            {{ -1.0f, -1.0f, -1.0f }, {  0.0f, -1.0f,  0.0f }},
            {{  1.0f, -1.0f, -1.0f }, {  0.0f, -1.0f,  0.0f }},
            {{  1.0f, -1.0f,  1.0f }, {  0.0f, -1.0f,  0.0f }},
            {{ -1.0f, -1.0f,  1.0f }, {  0.0f, -1.0f,  0.0f }},
            
            // Right face
            {{  1.0f, -1.0f, -1.0f }, {  1.0f,  0.0f,  0.0f }},
            {{  1.0f,  1.0f, -1.0f }, {  1.0f,  0.0f,  0.0f }},
            {{  1.0f,  1.0f,  1.0f }, {  1.0f,  0.0f,  0.0f }},
            {{  1.0f, -1.0f,  1.0f }, {  1.0f,  0.0f,  0.0f }},
            
            // Left face
            {{ -1.0f, -1.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }},
            {{ -1.0f, -1.0f,  1.0f }, { -1.0f,  0.0f,  0.0f }},
            {{ -1.0f,  1.0f,  1.0f }, { -1.0f,  0.0f,  0.0f }},
            {{ -1.0f,  1.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }},
        };
        
        std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0,      // Front
            4, 5, 6, 6, 7, 4,      // Back
            8, 9, 10, 10, 11, 8,   // Top
            12, 13, 14, 14, 15, 12,// Bottom
            16, 17, 18, 18, 19, 16,// Right
            20, 21, 22, 22, 23, 20 // Left
        };
        
        cubeIndexCount = indices.size();
        
        // Create VAO, VBO, EBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        
        // VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        
        // EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        std::cout << "[OK] Cube created with " << cubeIndexCount << " indices\n";
        return true;
    }
    
    void run() {
        std::cout << "\n[*] Starting shader development loop...\n\n";
        
        bool running = true;
        SDL_Event event;
        int frameCount = 0;
        
        // Capture mouse
        SDL_SetRelativeMouseMode(SDL_TRUE);
        
        auto lastTime = std::chrono::high_resolution_clock::now();
        auto appStartTime = std::chrono::high_resolution_clock::now();
        
        while (running && frameCount < 30000) {
            auto frameStart = std::chrono::high_resolution_clock::now();
            auto deltaTime = std::chrono::duration<float>(frameStart - lastTime).count();
            lastTime = frameStart;
            auto appTime = std::chrono::duration<float>(frameStart - appStartTime).count();
            
            // Handle events
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT || 
                    (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                    running = false;
                }
                // Mouse look
                if (event.type == SDL_MOUSEMOTION) {
                    float xoffset = event.motion.xrel * 0.1f;
                    float yoffset = event.motion.yrel * 0.1f;
                    
                    camera.yaw += xoffset;
                    camera.pitch -= yoffset;
                    
                    if (camera.pitch > 89.0f) camera.pitch = 89.0f;
                    if (camera.pitch < -89.0f) camera.pitch = -89.0f;
                    
                    camera.updateCameraVectors();
                }
            }
            
            // Input handling
            const Uint8* keys = SDL_GetKeyboardState(nullptr);
            if (keys[SDL_SCANCODE_W]) camera.moveForward(deltaTime);
            if (keys[SDL_SCANCODE_S]) camera.moveBackward(deltaTime);
            if (keys[SDL_SCANCODE_A]) camera.moveLeft(deltaTime);
            if (keys[SDL_SCANCODE_D]) camera.moveRight(deltaTime);
            if (keys[SDL_SCANCODE_SPACE]) camera.moveUp(deltaTime);
            if (keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL]) camera.moveDown(deltaTime);
            
            // Render
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glUseProgram(shaderProgram);
            
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = camera.getViewMatrix();
            glm::mat4 projection = camera.getProjectionMatrix(45.0f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 1000.0f);
            
            GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
            GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
            GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
            GLint timeLoc = glGetUniformLocation(shaderProgram, "time");
            
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            glUniform1f(timeLoc, appTime);
            
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, cubeIndexCount, GL_UNSIGNED_INT, 0);
            
            SDL_GL_SwapWindow(window);
            
            frameCount++;
            
            if (frameCount % 60 == 0) {
                std::cout << "[FRAME " << frameCount << "] FPS: ~60 | Pos: (" 
                          << (int)camera.position.x << ", " 
                          << (int)camera.position.y << ", " 
                          << (int)camera.position.z << ")\n";
            }
        }
        
        std::cout << "\n[OK] Render loop finished (" << frameCount << " frames)\n";
    }
    
    void cleanup() {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);
        if (shaderProgram) glDeleteProgram(shaderProgram);
        
        if (glContext) {
            SDL_GL_DeleteContext(glContext);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
        
        std::cout << "[OK] Application terminated\n";
    }
};

int main() {
    ShaderDevApp app;
    
    if (!app.initialize()) {
        std::cerr << "[ERROR] Initialization failed\n";
        return 1;
    }
    
    app.run();
    
    return 0;
}

// SDL requires this on Windows
extern "C" {
    int SDL_main(int argc, char* argv[]) {
        return main();
    }
}
