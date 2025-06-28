#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <iostream>
#include <string>

#include <SDL3/SDL.h>
#include <GL/gl3w.h>          // For OpenGL functions initialization
#include <imgui.h>
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>

class SimpleHandler : public osmium::handler::Handler {
public:
    void node(const osmium::Node& node) {
        std::cout << "Node ID: " << node.id() << "\n";
    }
};

int main(int argc, char** argv) {
    // Initialize SDL3 with video and timer subsystems

    std::cout << "SDL_Init Initializing...\n";
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD);
    if (((std::string)SDL_GetError()) == "" || ((std::string)SDL_GetError()).length() == 0) {
        std::cout << "SDL_Init successful!\n";
    }
    else {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    // Setup OpenGL attributes (version 3.3 Core)
    std::cout << "Setting up OpenGL attributes\n";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create SDL3 window with OpenGL
    std::cout << "Creating SDL3 window\n";
    SDL_Window* window = SDL_CreateWindow("ImGui + Osmium Test", 1280, 720, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Failed to create SDL3 window: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    std::cout << "Creating OpenGL context\n";
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize OpenGL loader (gl3w)
    std::cout << "Initializing OpenGL (GL3W)\n";
    if (gl3wInit()) {
        std::cerr << "Failed to initialize OpenGL loader (gl3w)\n";
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Enable vsync
    SDL_GL_SetSwapInterval(1);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup ImGui SDL3 + OpenGL3 backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // Main loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Show ImGui demo window (can be removed later)
        ImGui::ShowDemoWindow();

        // Render
        ImGui::Render();
        glViewport(0, 0, 1280, 720);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
