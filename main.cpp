#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "stb_image.h"


int main(int argc, char* argv[]) {

    const int screenWidth = 1600;
    const int screenHeight = 900;

    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! " << SDL_GetError() << "\n";
        return 1;
    }

    // Set OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create window
    SDL_Window* window = SDL_CreateWindow("Learning OpenGL",
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED,
                                          screenWidth, screenHeight,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        return 1;
    }

    // Create GL context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    float vertices[] = {
        // Position           // Normal            // TexCoords
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        // Left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        // Right face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // Create Buffers
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, 
                          GL_FLOAT, GL_FALSE, 
                          8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, 
                          GL_FLOAT, GL_FALSE, 
                          8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glBindVertexArray(lampVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader cubeShader("cubeShader.vs", "cubeShader.fs");
    Shader lampShader("lampShader.vs", "lampShader.fs");

    Texture containerTexture("container2.png");
    Texture containerSpecularMap("container2_specular.png");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glViewport(0, 0, screenWidth, screenHeight);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glm::vec3 lampPos(2.0f, 1.2f, 2.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 lampAmbience = glm::vec3(0.25f, 0.25f, 0.25f);
    glm::vec3 lampDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 lampSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

    int objectShininess = 32.0f;

    SDL_bool mouseLocked = SDL_TRUE;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Main program loop
    bool running = true;
    while (running) {
        // Capture and hide mouse cursor
        SDL_SetRelativeMouseMode(mouseLocked);

        float currentFrame = SDL_GetPerformanceCounter();
        deltaTime = (currentFrame - lastFrame)*1000 / SDL_GetPerformanceFrequency();
        lastFrame = currentFrame;


        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // SDL_QUIT occurs when the "x" button in the window title bar is pressed
            if (event.type == SDL_QUIT) running = false; 

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    std::cout << "ESC key pressed" << "\n";
                    running = false;
                }
                if (event.key.keysym.sym == SDLK_COMMA) {
                    std::cout << "COMMA pressed" << "\n";
                    if (mouseLocked == SDL_TRUE) {
                        mouseLocked = SDL_FALSE;
                    } else {
                        mouseLocked = SDL_TRUE;
                    }
                }

                if (event.key.keysym.sym == SDLK_w) {
                    camera.ProcessKeyboard(FORWARD, deltaTime);
                } 
                if (event.key.keysym.sym == SDLK_s) {
                    camera.ProcessKeyboard(BACKWARD, deltaTime);
                }
                if (event.key.keysym.sym == SDLK_a) {
                    camera.ProcessKeyboard(LEFT, deltaTime);
                }
                if (event.key.keysym.sym == SDLK_d) {
                    camera.ProcessKeyboard(RIGHT, deltaTime);
                }
            }

            if (event.type == SDL_MOUSEMOTION) {
                if (mouseLocked == SDL_TRUE) {
                    int dx = event.motion.xrel;
                    int dy = -event.motion.yrel;

                    camera.ProcessMouseMovement(dx, dy);
                }
            }

            if (event.type == SDL_MOUSEWHEEL) {
                if (mouseLocked == SDL_TRUE) {
                    int scrollX = event.wheel.x;
                    int scrollY = event.wheel.y;
                    
                    camera.ProcessMouseScroll(scrollY);
                }
            }
            
            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        // Draw background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        cubeShader.use();
        cubeShader.setVec3("lightPos", lampPos);
        cubeShader.setVec3("objectColor", objectColor);
        cubeShader.setVec3("viewPos", camera.Position);

        cubeShader.setInt("material.diffuse", 0);
        cubeShader.setInt("material.specular", 1);
        cubeShader.setFloat("material.shininess", objectShininess);

        cubeShader.setVec3("light.position", lampPos); // for point lighting
        cubeShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f); // for point and diectional lighting
        // cubeShader.setVec3("light.position", camera.Position); // for spot/flashlight
        // cubeShader.setVec3("light.direction", camera.Front); // for spot/flashlight
        cubeShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        cubeShader.setVec3("light.ambient", lampAmbience);
        cubeShader.setVec3("light.diffuse", lampDiffuse);
        cubeShader.setVec3("light.specular", lampSpecular);
        cubeShader.setFloat("light.constant", 1.0f);
        cubeShader.setFloat("light.linear", 0.09f);
        cubeShader.setFloat("light.quadratic", 0.032f);

        containerTexture.bind(0);
        containerSpecularMap.bind(1);

        glm::mat4 model = glm::mat4(1.0f);


        glm::mat4 view = camera.GetViewMatrix();

        glm::mat4 projection;
        projection = glm::perspective(
          glm::radians(camera.Zoom), 
          (float)screenWidth/(float)screenHeight, 
          0.1f, 
          100.0f
        );

        cubeShader.setMat4("model", model);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);

        // Draw to the screen
        glBindVertexArray(cubeVAO);
        for(unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // also draw the lamp object
        lampShader.use();
        lampShader.setVec3("lightColor", lightColor);
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);

        model = glm::mat4(1.0f);

        model = glm::translate(model, lampPos);

        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        
        lampShader.setMat4("model", model);

        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
      
        ImGui::Begin("Testing Station");
        ImGui::Text("Hello there, Adventurer!");
        ImGui::Text("Lamp: ");
        ImGui::ColorEdit3("Lamp", (float*)&lightColor);
        ImGui::ColorEdit3("Ambient", (float*)&lampAmbience);
        ImGui::ColorEdit3("Diffuse", (float*)&lampDiffuse);
        ImGui::ColorEdit3("Specular", (float*)&lampSpecular);
        ImGui::Text("Lamp Position: ");
        ImGui::SliderFloat("X", &lampPos.x, -3.0f, 3.0f);
        ImGui::SliderFloat("Y", &lampPos.y, -3.0f, 3.0f);
        ImGui::SliderFloat("Z", &lampPos.z, -3.0f, 3.0f);
        ImGui::Text("Object: ");
        ImGui::ColorEdit3("Colour", (float*)&objectColor);
        ImGui::SliderInt("Shininess", &objectShininess, 0, 1024);
        ImGui::End();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // Draw new frame
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Cleanly close the window
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

