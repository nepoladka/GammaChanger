#include "Window.h"


GLFWwindow* Window::GlfwHandle = NULL;


int main()
{
    if (!Window::Initialize())
    {
        system("pause");

        return EXIT_FAILURE;
    }

    FreeConsole();

    
    static constexpr const float DefaultGamma = 1.0f;

    int MonitorsCount = 0;
    GLFWmonitor** Monitors = glfwGetMonitors(&MonitorsCount);
    float* CurrentGamma = new float[MonitorsCount] { 1.0f };

    for (int i = 0; i < MonitorsCount; i++)
    {
        CurrentGamma[i] = 1.0f;
    }


    bool SaveChanges = true;


    ImVec2 WindowSize;

    while (!glfwWindowShouldClose(Window::GlfwHandle))
    {
        glfwPollEvents();

        glfwGetWindowSize(Window::GlfwHandle, (int*)&WindowSize.x, (int*)&WindowSize.y);
        WindowSize.x = (float)*(int*)&WindowSize.x;
        WindowSize.y = (float)*(int*)&WindowSize.y;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();



        ImGui::SetNextWindowSize(WindowSize);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("MainWindow", NULL, ImGuiWindowFlags_NoDecoration);


        if (ImGui::TreeNode("Screens", "Screens (%d)", MonitorsCount))
        {
            for (int i = 0; i < MonitorsCount; i++)
            {
                if(i) ImGui::Separator();

                std::string CurrentMonitorName = glfwGetMonitorName(Monitors[i]) + std::string(" [") + std::to_string(i) + std::string("]");

                ImGui::SetNextItemWidth(300);
                if (ImGui::SliderFloat(CurrentMonitorName.c_str(), &CurrentGamma[i], 0, 4, "%.2f"))
                {
                    glfwSetGamma(Monitors[i], CurrentGamma[i]);
                }

                if (ImGui::BeginPopupContextItem())
                {
                    ImGui::Text(CurrentMonitorName.c_str());

                    ImGui::Separator();

                    if (ImGui::MenuItem("Restore to 1")) glfwSetGamma(Monitors[i], CurrentGamma[i] = DefaultGamma);

                    ImGui::EndPopup();
                }

            }

            ImGui::TreePop();
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
        if (ImGui::TreeNode("Settings"))
        {
            ImGui::Checkbox("Save after exit", &SaveChanges);

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Info"))
        {
            ImGui::Text("Powered by github.com/glfw/glfw\nand github.com/ocornut/imgui\ncreated by github.com/Nepoladka");

            ImGui::TreePop();
        }


        ImGui::End();



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(Window::GlfwHandle);

        Sleep(25);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();


    glfwDestroyWindow(Window::GlfwHandle);
    
    if (!SaveChanges) glfwTerminate();

	return EXIT_SUCCESS;
}
