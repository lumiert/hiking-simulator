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
#include "engine/render/PlaneGenerator.h"

class FirstPersonApp {
private:
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
    bool running = true;
    
    FirstPersonCamera camera;
    GLuint VAO = 0, VBO = 0, EBO = 0;
    GLuint shaderProgram = 0;
    unsigned int planeIndexCount = 0;
    
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    const float PLANE_WIDTH = 100.0f;
    const float PLANE_HEIGHT = 100.0f;
    
public:
    FirstPersonApp() : camera(glm::vec3(0.0f, 2.0f, 10.0f)) {}
    
    ~FirstPersonApp() {
        cleanup();
    }
    
    bool initialize() {
        std::cout << "[START] First-Person Camera Demo\n";
        
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
            "Hiking Simulator - First Person",
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
        
        // Setup OpenGL
        glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        SDL_GL_SetSwapInterval(1);
        
        // Create shaders
        if (!createShaders()) {
            return false;
        }
        
        // Create plane
        if (!createPlane()) {
            return false;
        }
        
        std::cout << "[INFO] Controls:\n";
        std::cout << "  W/A/S/D - Move forward/left/back/right\n";
        std::cout << "  Space/Ctrl - Move up/down\n";
        std::cout << "  Mouse - Look around\n";
        std::cout << "  ESC - Exit\n";
        
        return true;
    }
    
    bool createShaders() {
        // Vertex shader
        const char* vertexShaderSource = R"glsl(
            #version 330 core
            layout (location = 0) in vec3 aPosition;
            layout (location = 1) in vec3 aColor;
            
            uniform mat4 view;
            uniform mat4 projection;
            
            out vec3 vertexColor;
            
            void main() {
                gl_Position = projection * view * vec4(aPosition, 1.0);
                vertexColor = aColor;
            }
        )glsl";
        
        // Fragment shader
        const char* fragmentShaderSource = R"glsl(
            #version 330 core
            in vec3 vertexColor;
            out vec4 FragColor;
            
            void main() {
                FragColor = vec4(vertexColor, 1.0);
            }
        )glsl";
        
        // Compile vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        
        // Check for errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cerr << "[ERROR] Vertex shader compilation failed: " << infoLog << "\n";
            return false;
        }
        
        // Compile fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "[ERROR] Fragment shader compilation failed: " << infoLog << "\n";
            return false;
        }
        
        // Link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cerr << "[ERROR] Shader program linking failed: " << infoLog << "\n";
            return false;
        }
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        std::cout << "[OK] Shaders compiled successfully\n";
        return true;
    }
    
    bool createPlane() {
        // Generate plane
        auto vertices = PlaneGenerator::generatePlane(PLANE_WIDTH, PLANE_HEIGHT, 50);
        auto indices = PlaneGenerator::generatePlaneIndices(50);
        planeIndexCount = indices.size();
        
        std::cout << "[INFO] Plane: " << vertices.size() << " vertices, " 
                  << planeIndexCount << " indices\n";
        
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        std::cout << "[OK] Plane created\n";
        return true;
    }
    
    void run() {
        std::cout << "\n[*] Starting render loop...\n\n";
        
        bool running = true;
        SDL_Event event;
        int frameCount = 0;
        int lastMouseX = WINDOW_WIDTH / 2;
        int lastMouseY = WINDOW_HEIGHT / 2;
        bool firstMouse = true;
        
        // Capture mouse
        SDL_SetRelativeMouseMode(SDL_TRUE);
        
        auto lastTime = std::chrono::high_resolution_clock::now();
        auto lastInputTime = std::chrono::high_resolution_clock::now();
        
        while (running && frameCount < 30000) {  // 30000 frames ~ 500 seconds at 60 FPS
            auto frameStart = std::chrono::high_resolution_clock::now();
            auto deltaTime = std::chrono::duration<float>(frameStart - lastTime).count();
            lastTime = frameStart;
            
            // Handle events
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT || 
                    (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                    running = false;
                }
                // Mouse look
                if (event.type == SDL_MOUSEMOTION) {
                    int xpos = event.motion.xrel;
                    int ypos = event.motion.yrel;
                    
                    float xoffset = xpos * 0.1f;
                    float yoffset = ypos * 0.1f;
                    
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
            
            // Collision detection with ground (plane at y=0)
            const float GROUND_LEVEL = 0.5f;  // Player eye height above ground
            if (camera.position.y < GROUND_LEVEL) {
                camera.position.y = GROUND_LEVEL;
            }
            
            // Render
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glUseProgram(shaderProgram);
            
            glm::mat4 view = camera.getViewMatrix();
            glm::mat4 projection = camera.getProjectionMatrix(45.0f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 1000.0f);
            
            GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
            GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
            
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, planeIndexCount, GL_UNSIGNED_INT, 0);
            
            SDL_GL_SwapWindow(window);
            
            frameCount++;
            
            // Performance output
            if (frameCount % 60 == 0) {
                auto pos = camera.getPosition();
                std::cout << "[FRAME " << frameCount << "] FPS: ~60 | "
                          << "Pos: (" << (int)pos.x << ", " << (int)pos.y << ", " << (int)pos.z << ")\n";
            }
        }
        
        std::cout << "\n[OK] Render loop finished (" << frameCount << " frames)\n";
    }
    
private:
    void cleanup() {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);
        if (shaderProgram) glDeleteProgram(shaderProgram);
        
        if (glContext) SDL_GL_DeleteContext(glContext);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

int SDL_main(int, char*[]) {
    FirstPersonApp app;
    
    if (!app.initialize()) {
        std::cerr << "[ERROR] Initialization failed\n";
        return 1;
    }
    
    app.run();
    
    std::cout << "[OK] Application terminated\n";
    return 0;
}
