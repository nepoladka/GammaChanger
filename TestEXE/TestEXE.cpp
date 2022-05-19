#include "TestEXE.h"

#include "VMProtectSDK.h"

#include <string>
#include <intrin.h>

#pragma intrinsic(_ReturnAddress)

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#pragma comment(lib, "glfw3.lib")

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/backends/imgui_impl_glfw.h"
#include "ImGui/backends/imgui_impl_opengl3.h"


static void glfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW: Error %d: %s\n", error, description);
}

int messagef(UINT uType, const char* const format, ...)
{
    int MessageSize = snprintf(NULL, NULL, format) + 1;

    char* MessageStr = new char[MessageSize] { 0 };


    va_list ArgList;
    __crt_va_start(ArgList, format);
    vsnprintf(MessageStr, MessageSize, format, ArgList);
    __crt_va_end(ArgList);


    int Result = MessageBoxA(NULL, MessageStr, "Message", uType);

    delete[] MessageStr;

    return Result;
}


int main()
{
    FreeConsole();

    glfwSetErrorCallback(glfwErrorCallback);

    int InitCode = glfwInit();

    if (!InitCode)
    {
        messagef(MB_OK, "GLFW initialize failed with code: %d\n", InitCode);
    }

    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DEPTH_BITS, 16);


    GLFWwindow* GlfwHandle = glfwCreateWindow(300, 150, "Screen gamma changer", NULL, NULL);
    if (!GlfwHandle)
    {
        messagef(MB_OK, "GLFW creating window failed!");
    }


    glfwMakeContextCurrent(GlfwHandle);

    glfwSwapInterval(GLFW_DONT_CARE);





    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImFontConfig font_config;
    font_config.OversampleH = 1; //or 2 is the same
    font_config.OversampleV = 1;
    font_config.PixelSnapH = 1;

    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x04FF, // Cyrillic
        0,
    };

    ImGuiIO& IO = ImGui::GetIO();

    IO.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeui.ttf", 18, &font_config, ranges); // AddFontFromMemoryTTF(font, 1, 13, &font_config, ranges);

    ImGui::StyleColorsDark();

    IO.IniFilename = NULL;

    ImGui_ImplGlfw_InitForOpenGL(GlfwHandle, true);
    ImGui_ImplOpenGL3_Init("#version 130");





    static const float DefaultGamma = 1.0f;

    int MonitorsCount = 0;
    GLFWmonitor** Monitors = glfwGetMonitors(&MonitorsCount);
    float* CurrentGamma = new float[MonitorsCount] { 1.0f };

    for (int i = 0; i < MonitorsCount; i++)
    {
        CurrentGamma[i] = 1.0f;
    }


    bool SaveChanges = true;


    ImVec2 WindowSize;

    while (!glfwWindowShouldClose(GlfwHandle))
    {
        glfwPollEvents();


        glfwGetWindowSize(GlfwHandle, (int*)&WindowSize.x, (int*)&WindowSize.y);
        WindowSize.x = (float)*(int*)&WindowSize.x;
        WindowSize.y = (float)*(int*)&WindowSize.y;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        


        ImGui::SetNextWindowSize(WindowSize);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("MainWindow", NULL, ImGuiWindowFlags_NoDecoration);


        for (int i = 0; i < MonitorsCount; i++)
        {
            std::string CurrentMonitorName = glfwGetMonitorName(Monitors[i]) + std::string(" [") + std::to_string(i) + std::string("]");

            if (ImGui::TreeNode(CurrentMonitorName.c_str()))
            {
                if (ImGui::SliderFloat("Gamma", &CurrentGamma[i], 1, 4, "%.2f"))
                {
                    glfwSetGamma(Monitors[i], CurrentGamma[i]);
                }

                if (ImGui::Button("Restore"))
                {
                    CurrentGamma[i] = DefaultGamma;

                    glfwSetGamma(Monitors[i], CurrentGamma[i]);
                }

                ImGui::TreePop();
            }

            ImGui::Separator();
        }

        ImGui::Checkbox("Save", &SaveChanges);
        ImGui::Spacing();
        ImGui::Text("Powered by GLFW and Dear ImGui!");

        ImGui::End();





        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(GlfwHandle);

        Sleep(1);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();


    
    glfwDestroyWindow(GlfwHandle);
    
    if (!SaveChanges)
        glfwTerminate();

	return EXIT_SUCCESS;
}

/*
int protectedFunc()
{
	VMProtectBeginUltraLockByKey(__func__);

	int var = rand();

	int poshel = 2;

	int result = var ^ poshel;

	VMProtectEnd();

	
	return result;
}

int main()
{
	char key[15]{ "key" };

	char SerialNumber[10000];

	int res = VMProtectActivateLicense(key, SerialNumber, sizeof(SerialNumber));

	printf("activation result: %d\n", res);

	res = protectedFunc();

	printf("func result: %d\n", res);
    
	return EXIT_SUCCESS;
}
*/