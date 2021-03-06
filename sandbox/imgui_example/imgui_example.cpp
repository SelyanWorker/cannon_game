#include "application/application.h"
#include "event/event.h"
#include "layers/layer.h"
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

class ImGuiLayer : public selyan::Layer
{
public:
    ImGuiLayer()
    {
        auto nativeWindow = reinterpret_cast<GLFWwindow *>(
            selyan::Application::get()->getWindow()->getNativeWindow());

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();

        ImGuiStyle &Style = ImGui::GetStyle();

        // Setup Dear ImGui Style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();

        // Setup Platform/Renderer bindings

        ImGui_ImplGlfw_InitForOpenGL(nativeWindow, true);
        ImGui_ImplOpenGL3_Init("#version 430");
    }

    ~ImGuiLayer()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void onEvent(selyan::Event &e) override
    {
        // std::cout << "OnEvent" << std::endl;
    }

    void onUpdate(const selyan::TimeStep& timeStep) override {}

    void onRender() override
    {
        imguiBegin();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You
        // can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a
        // named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin(
                "Hello, world!");   // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");   // Display some text (you can use a format
                                                        // strings too)
            ImGui::Checkbox("Demo Window",
                            &show_demo_window);   // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float",
                               &f,
                               0.0f,
                               1.0f);   // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color",
                              (float *)&clear_color);   // Edit 3 floats representing a color

            if (ImGui::Button("Button"))   // Buttons return true when clicked (most widgets return
                                           // true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window",
                         &show_another_window);   // Pass a pointer to our bool variable (the window
                                                  // will have a closing button that will clear the
                                                  // bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;

        // glfwGetFramebufferSize(selyan::Application::get()->getWindow()->, &display_w,
        // &display_h);

        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w,
                     clear_color.y * clear_color.w,
                     clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions m.y change the current OpenGL context, so we save/restore it to make
        // it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) direct.y)
        //    ImGuiIO &io = ImGui::GetIO();
        //    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        //    {
        //      GLFWwindow *backup_current_context = glfwGetCurrentContext();
        //      ImGui::UpdatePlatformWindows();
        //      ImGui::RenderPlatformWindowsDefault();
        //      glfwMakeContextCurrent(backup_current_context);
        //    }

        imguiEnd();
    }

private:
    void imguiBegin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void imguiEnd()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions m.y change the current OpenGL context, so we save/restore it to make
        // it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) direct.y)
        //    ImGuiIO &io = ImGui::GetIO();
        //    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        //    {
        //      GLFWwindow *backup_current_context = glfwGetCurrentContext();
        //      ImGui::UpdatePlatformWindows();
        //      ImGui::RenderPlatformWindowsDefault();
        //      glfwMakeContextCurrent(backup_current_context);
        //    }
    }

private:
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

int main()
{
    selyan::Application app{};

    ImGuiLayer imGuiLayer;
    app.pushLayer(&imGuiLayer);

    app.run();

    return 0;
}
