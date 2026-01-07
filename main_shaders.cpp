#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <fstream>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/stb_image.h"

#include "engine/render/FirstPersonCamera.h"
#include "engine/scene/ObjectManager.h"


// Simple texture loader using stb_image
GLuint loadTexture(const std::string& path) {
    int width, height, channels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    
    if (!data) {
        std::cout << "[ERROR] Failed to load image: " << path << "\n";
        return 0;
    }
    
    std::cout << "[INFO] Loaded image: " << path << " (" << width << "x" << height << ", " << channels << " channels)\n";
    
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    stbi_image_free(data);
    return texture;
}

class ShaderDevApp {
private:
    static constexpr int WINDOW_WIDTH = 1280;
    static constexpr int WINDOW_HEIGHT = 720;
    
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
    bool running = true;
    
    FirstPersonCamera camera;
    SceneManager scene;
    
    GLuint shaderProgram = 0;
    GLuint skyboxVAO = 0, skyboxVBO = 0;
    GLuint sunTexture = 0, moonTexture = 0;
    GLuint skyShaderProgram = 0;
    GLuint sunVAO = 0, sunVBO = 0;
    GLuint moonVAO = 0, moonVBO = 0;
    GLuint sunShader = 0, moonShader = 0;
    
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
        
        // Setup skybox
        setupSkybox();
        
        // Setup sun and moon geometry
        setupSkyObjects();
        
        // Load textures for sky
        loadSkyTextures();
        
        // Load old television model
        scene.placeObject("game/assets/shared/models/old_television.glb", 0.0f, -10.0f, -50.0f,
                 glm::radians(-90.0f), glm::radians(0.0f), glm::radians(0.0f),
                 0.2f, 0.2f, 0.2f, 1);
        
        std::cout << "[INFO] TV Position: (0, -10, -50)\n";
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
        
        // Fragment shader - PBR with textures
        const char* fragmentShaderSource = R"(
            #version 120
            
            uniform float time;
            uniform sampler2D baseColorTex;
            uniform sampler2D metallicRoughnessTex;
            uniform sampler2D normalTex;
            
            varying vec3 fragPos;
            varying vec3 fragNormal;
            varying vec2 fragTexCoord;
            
            vec3 fresnelSchlick(float cosTheta, vec3 F0) {
                return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
            }
            
            float DistributionGGX(vec3 N, vec3 H, float roughness) {
                float a = roughness * roughness;
                a = a * a;
                float NdotH = max(dot(N, H), 0.0);
                float NdotH2 = NdotH * NdotH;
                float nom = a;
                float denom = (NdotH2 * (a - 1.0) + 1.0);
                denom = 3.14159 * denom * denom;
                return nom / max(denom, 0.0000001);
            }
            
            float GeometrySchlickGGX(float NdotV, float roughness) {
                float r = (roughness + 1.0);
                float k = (r * r) / 8.0;
                float nom = NdotV;
                float denom = NdotV * (1.0 - k) + k;
                return nom / max(denom, 0.0000001);
            }
            
            void main() {
                vec4 baseColor = texture2D(baseColorTex, fragTexCoord);
                vec4 mrTex = texture2D(metallicRoughnessTex, fragTexCoord);
                vec3 normalMap = texture2D(normalTex, fragTexCoord).rgb;
                
                vec3 norm = normalize(fragNormal);
                vec3 viewDir = normalize(-fragPos);
                
                float metallic = mrTex.b;
                float roughness = mrTex.g;
                
                vec3 F0 = mix(vec3(0.04), baseColor.rgb, metallic);
                
                vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
                vec3 H = normalize(viewDir + lightDir);
                
                float distance = 1.0;
                float attenuation = 1.0 / (distance * distance);
                vec3 radiance = vec3(1.0) * attenuation;
                
                vec3 F = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);
                float NDF = DistributionGGX(norm, H, roughness);
                float G = GeometrySchlickGGX(max(dot(norm, viewDir), 0.0), roughness) * 
                         GeometrySchlickGGX(max(dot(norm, lightDir), 0.0), roughness);
                
                vec3 kS = F;
                vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);
                
                float NdotL = max(dot(norm, lightDir), 0.0);
                vec3 numerator = NDF * G * F;
                float denominator = 4.0 * max(dot(norm, viewDir), 0.0) * max(dot(norm, lightDir), 0.0);
                vec3 specular = numerator / max(denominator, 0.001);
                
                vec3 color = (kD * baseColor.rgb / 3.14159 + specular) * radiance * NdotL;
                
                // Add ambient
                color += baseColor.rgb * 0.1;
                
                // Add time animation
                color += 0.1 * sin(time) * vec3(0.5, 0.2, 0.1);
                
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
            
            // Render skybox background
            renderSkybox(appTime);
            
            // Render scene
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glm::mat4 view = camera.getViewMatrix();
            glm::mat4 projection = camera.getProjectionMatrix(45.0f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 1000.0f);
            
            scene.renderAll(shaderProgram, view, projection, appTime);
            
            // Render sun and moon orbiting around player
            renderSkyObjects(appTime, camera.position);
            
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
    
    void setupSkybox() {
        // Create a simple skybox with a single quad far from camera
        float skySize = 500.0f;
        float skyVertices[] = {
            -skySize, -skySize, -skySize,
             skySize, -skySize, -skySize,
             skySize,  skySize, -skySize,
            -skySize,  skySize, -skySize,
        };
        
        unsigned int skyIndices[] = {
            0, 1, 2,
            0, 2, 3,
        };
        
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    
    void setupSkyObjects() {
        // Create sphere geometry for sun and moon
        // Using a simple quad that always faces camera
        float size = 50.0f;  // Increased for better visibility
        float quadVertices[] = {
            -size, -size, 0.0f,  0.0f, 0.0f,
             size, -size, 0.0f,  1.0f, 0.0f,
             size,  size, 0.0f,  1.0f, 1.0f,
            -size,  size, 0.0f,  0.0f, 1.0f,
        };
        
        unsigned int quadIndices[] = {
            0, 1, 2,
            0, 2, 3,
        };
        
        // Setup sun
        glGenVertexArrays(1, &sunVAO);
        glGenBuffers(1, &sunVBO);
        GLuint sunEBO;
        glGenBuffers(1, &sunEBO);
        
        glBindVertexArray(sunVAO);
        glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sunEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
        
        // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        
        // TexCoord attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        // Setup moon (same geometry)
        glGenVertexArrays(1, &moonVAO);
        glGenBuffers(1, &moonVBO);
        GLuint moonEBO;
        glGenBuffers(1, &moonEBO);
        
        glBindVertexArray(moonVAO);
        glBindBuffer(GL_ARRAY_BUFFER, moonVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moonEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
        
        // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        
        // TexCoord attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    
    void loadSkyTextures() {
        // Load sun texture
        std::cout << "[*] Loading sun texture...\n";
        sunTexture = loadTexture("game/assets/environment/sky/sun.png");
        
        // Load moon texture
        std::cout << "[*] Loading moon texture...\n";
        moonTexture = loadTexture("game/assets/environment/sky/moon.png");
    }
    
    void renderSkyObjects(float time, const glm::vec3& playerPos) {
        // Compile shaders once
        if (sunShader == 0) {
            const char* vertexShader = R"(
                #version 120
                
                attribute vec3 aPos;
                attribute vec2 aTexCoord;
                
                uniform mat4 view;
                uniform mat4 projection;
                uniform mat4 model;
                
                varying vec2 TexCoord;
                
                void main() {
                    gl_Position = projection * view * model * vec4(aPos, 1.0);
                    TexCoord = aTexCoord;
                }
            )";
            
            // Sun shader - with texture
            const char* sunFragShader = R"(
                #version 120
                
                uniform sampler2D texture1;
                varying vec2 TexCoord;
                
                void main() {
                    gl_FragColor = texture2D(texture1, TexCoord);
                }
            )";
            
            // Moon shader - with texture
            const char* moonFragShader = R"(
                #version 120
                
                uniform sampler2D texture1;
                varying vec2 TexCoord;
                
                void main() {
                    gl_FragColor = texture2D(texture1, TexCoord);
                }
            )";
            
            // Compile sun shader
            GLuint vs = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vs, 1, &vertexShader, nullptr);
            glCompileShader(vs);
            
            // Check vertex shader compilation
            int success;
            char infoLog[512];
            glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vs, 512, nullptr, infoLog);
                std::cerr << "[ERROR] Vertex shader compilation failed:\n" << infoLog << "\n";
            }
            
            GLuint sunFs = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(sunFs, 1, &sunFragShader, nullptr);
            glCompileShader(sunFs);
            
            glGetShaderiv(sunFs, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(sunFs, 512, nullptr, infoLog);
                std::cerr << "[ERROR] Sun fragment shader compilation failed:\n" << infoLog << "\n";
            }
            
            sunShader = glCreateProgram();
            glAttachShader(sunShader, vs);
            glAttachShader(sunShader, sunFs);
            glBindAttribLocation(sunShader, 0, "aPos");
            glBindAttribLocation(sunShader, 1, "aTexCoord");
            glLinkProgram(sunShader);
            
            glGetProgramiv(sunShader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(sunShader, 512, nullptr, infoLog);
                std::cerr << "[ERROR] Sun shader linking failed:\n" << infoLog << "\n";
            }
            
            glDeleteShader(sunFs);
            
            // Compile moon shader
            GLuint moonFs = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(moonFs, 1, &moonFragShader, nullptr);
            glCompileShader(moonFs);
            
            glGetShaderiv(moonFs, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(moonFs, 512, nullptr, infoLog);
                std::cerr << "[ERROR] Moon fragment shader compilation failed:\n" << infoLog << "\n";
            }
            
            moonShader = glCreateProgram();
            glAttachShader(moonShader, vs);
            glAttachShader(moonShader, moonFs);
            glBindAttribLocation(moonShader, 0, "aPos");
            glBindAttribLocation(moonShader, 1, "aTexCoord");
            glLinkProgram(moonShader);
            
            glGetProgramiv(moonShader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(moonShader, 512, nullptr, infoLog);
                std::cerr << "[ERROR] Moon shader linking failed:\n" << infoLog << "\n";
            }
            
            glDeleteShader(vs);
            glDeleteShader(moonFs);
        }
        
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
        // Get view and projection matrices
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix(45.0f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 1000.0f);
        
        // Sun orbit - follows player position, rotates around player
        float sunAngle = time * 0.5f;
        // TO ADJUST SUN DISTANCE: Change value below (smaller = closer to player)
        float sunDistance = 35.0f;  // Distance in world units from player
        glm::vec3 sunPos = playerPos + glm::vec3(
            cos(sunAngle) * sunDistance,
            20.0f + sin(time * 0.3f) * 10.0f,
            sin(sunAngle) * sunDistance
        );
        
        // Log sun position every 2 seconds
        static float lastLogTime = 0.0f;
        glm::vec3 tvPos(0.0f, -10.0f, -50.0f);
        float playerToTvDist = glm::distance(playerPos, tvPos);
        float playerToSunDist = glm::distance(playerPos, sunPos);
        
        if (time - lastLogTime > 2.0f) {
            std::cout << "[OBJECTS] Player: (" << (int)playerPos.x << ", " << (int)playerPos.y << ", " 
                      << (int)playerPos.z << ")\n";
            std::cout << "[TV] Fixed Pos: (0, -10, -50) | Distance from player: " << (int)playerToTvDist << " units\n";
            std::cout << "[SUN] Pos: (" << (int)sunPos.x << ", " << (int)sunPos.y << ", " 
                      << (int)sunPos.z << ") | Dist: " << (int)playerToSunDist << " units | Angle: " 
                      << (int)(sunAngle * 180.0f / 3.14159f) << "°\n";
            lastLogTime = time;
        }
        
        // Create billboard matrix - make sun face camera
        glm::vec3 toCamera = glm::normalize(camera.position - sunPos);
        glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), toCamera));
        glm::vec3 up = glm::normalize(glm::cross(toCamera, right));
        
        glm::mat4 sunModel = glm::mat4(
            glm::vec4(right, 0.0f),
            glm::vec4(up, 0.0f),
            glm::vec4(-toCamera, 0.0f),
            glm::vec4(sunPos, 1.0f)
        );
        
        glUseProgram(sunShader);
        GLint viewLoc = glGetUniformLocation(sunShader, "view");
        GLint projLoc = glGetUniformLocation(sunShader, "projection");
        GLint modelLoc = glGetUniformLocation(sunShader, "model");
        GLint texLoc = glGetUniformLocation(sunShader, "texture1");
        
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sunModel));
        
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sunTexture);
        glUniform1i(texLoc, 0);
        
        // Verify texture is bound
        if (sunTexture == 0) {
            std::cerr << "[ERROR] Sun texture not loaded! ID = 0\n";
        }
        
        static int debugFrameCount = 0;
        if (debugFrameCount++ % 120 == 0) {
            std::cout << "[DEBUG] Rendering SUN quad at: (" << (int)sunPos.x << ", " << (int)sunPos.y << ", " 
                      << (int)sunPos.z << ") | Texture ID: " << sunTexture << "\n";
        }
        
        glBindVertexArray(sunVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Moon orbit - opposite side of sun
        float moonAngle = time * 0.5f + 3.14159f;
        // TO ADJUST MOON DISTANCE: Change value below (smaller = closer to player)
        float moonDistance = 35.0f;  // Distance in world units from player
        glm::vec3 moonPos = playerPos + glm::vec3(
            cos(moonAngle) * moonDistance,
            25.0f + sin(time * 0.25f) * 10.0f,  // Increased to 25.0f to be higher in sky
            sin(moonAngle) * moonDistance
        );
        
        float playerToMoonDist = glm::distance(playerPos, moonPos);
        
        // Log moon position on same interval
        if (time - lastLogTime > 2.0f && time - lastLogTime < 0.016f) {
            std::cout << "[MOON] Pos: (" << (int)moonPos.x << ", " << (int)moonPos.y << ", " 
                      << (int)moonPos.z << ") | Dist: " << (int)playerToMoonDist << " units | Angle: " 
                      << (int)(moonAngle * 180.0f / 3.14159f) << "°\n\n";
        }
        
        // Create billboard matrix for moon - make moon face camera
        glm::vec3 moonToCamera = glm::normalize(camera.position - moonPos);
        glm::vec3 moonRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), moonToCamera));
        glm::vec3 moonUp = glm::normalize(glm::cross(moonToCamera, moonRight));
        
        glm::mat4 moonModel = glm::mat4(
            glm::vec4(moonRight, 0.0f),
            glm::vec4(moonUp, 0.0f),
            glm::vec4(-moonToCamera, 0.0f),
            glm::vec4(moonPos, 1.0f)
        );
        
        glUseProgram(moonShader);
        glUniformMatrix4fv(glGetUniformLocation(moonShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(moonShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(moonShader, "model"), 1, GL_FALSE, glm::value_ptr(moonModel));
        
        GLint moonTexLoc = glGetUniformLocation(moonShader, "texture1");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, moonTexture);
        glUniform1i(moonTexLoc, 0);
        
        if (moonTexture == 0) {
            std::cerr << "[ERROR] Moon texture not loaded! ID = 0\n";
        }
        
        if (debugFrameCount % 120 == 60) {
            std::cout << "[DEBUG] Rendering MOON quad at: (" << (int)moonPos.x << ", " << (int)moonPos.y << ", " 
                      << (int)moonPos.z << ") | Texture ID: " << moonTexture << "\n\n";
        }
        
        glBindVertexArray(moonVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    void renderSkybox(float time) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        
        // Create a simple sky gradient background color
        float t = sin(time * 0.1f) * 0.5f + 0.5f; // Oscillate between 0 and 1
        glClearColor(0.3f + t * 0.3f, 0.5f + t * 0.2f, 0.8f - t * 0.2f, 1.0f);
        
        glEnable(GL_DEPTH_TEST);
    }
    
    void cleanup() {
        scene.cleanup();
        if (shaderProgram) glDeleteProgram(shaderProgram);
        if (skyShaderProgram) glDeleteProgram(skyShaderProgram);
        if (sunShader) glDeleteProgram(sunShader);
        if (moonShader) glDeleteProgram(moonShader);
        if (skyboxVAO) glDeleteVertexArrays(1, &skyboxVAO);
        if (skyboxVBO) glDeleteBuffers(1, &skyboxVBO);
        if (sunVAO) glDeleteVertexArrays(1, &sunVAO);
        if (sunVBO) glDeleteBuffers(1, &sunVBO);
        if (moonVAO) glDeleteVertexArrays(1, &moonVAO);
        if (moonVBO) glDeleteBuffers(1, &moonVBO);
        if (sunTexture) glDeleteTextures(1, &sunTexture);
        if (moonTexture) glDeleteTextures(1, &moonTexture);
        
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
