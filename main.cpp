//********************************************
// Author: Russell Saenz
// File: main.cpp
// Description: Main driver for the TrafficSimulator. Manages the GLFW/ImGui
//              lifecycle, spawns simulation threads, and coordinates the
//              rendering of UI components.
//********************************************

#include <iostream>

#include <chrono>
#include <thread>
#include <conio.h>
#include <cmath>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include "traffic.h"
#include "vehicles.h"
#include "SimulationUI.h"


using namespace std;

void delay(int delayTimer);




//****************************** Performance measure
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>

double getMemoryUsageMB() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        // WorkingSetSize is the physical memory used by the process
        return (double)pmc.WorkingSetSize / (1024.0 * 1024.0);
    }
    return 0.0;
}
//************************************



int main() {
    srand(time(nullptr));

    Traffic intersection;

    thread trafficThread;
    thread trafficLights;
    thread runningTraffic;




    if (!glfwInit()) {
        return 1;
    }

    GLFWwindow *window = glfwCreateWindow(1280, 720, "Traffic Simulation", nullptr, nullptr);

    if (!window) {
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    //settings for docked windows in the ui
    const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar |
                                    ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    //user interacted variables
    int northSouthLanes = 3;
    int eastWestLanes = 3;

    int speed = 45;
    int northSouthLightTimer = 5;
    int eastWestLightTimer = 5;


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport();

        //these run during the set up screen
        if (!intersection.running) {

            setupMenu(windowFlags, intersection, northSouthLanes, eastWestLanes, speed, northSouthLightTimer, eastWestLightTimer);


           ImGui::Begin ("Begin Simulation");
               if (ImGui::Button("Begin", ImVec2(100,50))) {

                   createIntersection(intersection, static_cast<uint8_t>(northSouthLanes), static_cast<uint8_t>(eastWestLanes), static_cast<uint8_t>(speed), static_cast<uint8_t>(northSouthLightTimer), static_cast<uint8_t>(eastWestLightTimer));

                   intersection.running = true;
                   trafficThread = thread(&Traffic::trafficLoop, &intersection);
                   trafficLights = thread(&Traffic::trafficLightsLoop, &intersection);
                   runningTraffic = thread(&Traffic::passVehiclesThroughIntersection, &intersection);
               }
           ImGui::End();
        }

        //ACTUAL ROAD WINDOW
        buildRoad(northSouthLanes, eastWestLanes, windowFlags, intersection);

        //these are the second screen
        if (intersection.running){
            drawTrafficLightState(windowFlags, intersection);
            displayStats(windowFlags, intersection);
            speedSlider(windowFlags, intersection);


            //****************************** Performance measure
            ImGui::Begin("System Monitor");
                double memUsed = getMemoryUsageMB();
                ImGui::Text("RAM Usage: %.2f MB", memUsed);

                // Create a scrolling history graph
                static float memHistory[100] = { 0 };
                static int offset = 0;
                memHistory[offset] = (float)memUsed;
                offset = (offset + 1) % 100;

                ImGui::PlotLines("Memory Leak Check", memHistory, 100, offset, nullptr, 0.0f, 500.0f, ImVec2(0, 80));
            ImGui::End();
            //****************************** Performance measure
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.2f, 0.2f, 0.2f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    //ending the run loops causing threads to return
    intersection.running = false;

    trafficThread.join();
    trafficLights.join();
    runningTraffic.join();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    //console debug output
    // cout<<"Car total: "<< totalDrivers<<"\n";
    // printIntersection(intersection);
    return 0;
}
