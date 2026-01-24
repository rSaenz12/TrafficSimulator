#include <iostream>
#include <vector>

// #include <bits/this_thread_sleep.h>

#include <chrono>
#include <thread>

#include <conio.h>

#include "traffic.h"
#include "vehicles.h"
#include "SimulationUI.h"


using namespace std;

void delay(int delayTimer);

// int main() {
//     srand(time(nullptr));
//
//     thread trafficThread;
//     thread trafficLights;
//     thread runningTraffic;
//
//     Traffic intersection(45);
//     createIntersection(intersection);
//
//     SimulationUI ui(intersection);
//
//
//     int userInput = 0;
//
//     cout<<"ENTER Number >= 1 to start \n";
//     while (userInput < 1) {
//         cin>>userInput;
//     }
//
//     if (userInput>0) {
//         intersection.running = true;
//         trafficThread = thread(&Traffic::trafficLoop, &intersection);
//         trafficLights = thread(&Traffic::trafficLightsLoop, &intersection);
//         runningTraffic = thread(&Traffic::passVehiclesThroughIntersection, &intersection);
//     }
//
//     userInput = 0;
//
//     cout<<"ENTER Number >= 1 to stop \n";
//     while (userInput < 1) {
//         cin>>userInput;
//     }
//
//     if (userInput > 0) {
//         intersection.running = false;
//
//         trafficThread.join();
//         trafficLights.join();
//         runningTraffic.join();
//     }
//
//
//     printIntersection(intersection);
//     return 0;
// }

#include <cmath>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

void buildRoad(int lanes, ImGuiWindowFlags window_flags);
void drawDashedLine(ImDrawList* draw_list, ImVec2 start, ImVec2 voidArea, ImVec2 end, ImU32 color, float thickness, float dash_length);
void drawLanesPerDirection(ImDrawList* draw_list, ImVec2 center, int lanes, float totalRoadWidth, ImVec2 canvas_p0, ImVec2 canvas_p1, ImVec2 hVoid, ImVec2 vVoid);


int main() {
    srand(time(nullptr));

    thread trafficThread;
    thread trafficLights;
    thread runningTraffic;

    // Traffic intersection(45);
    // createIntersection(intersection);
    //
    // // SimulationUI ui(intersection);
    //
    //
    // intersection.running = true;
    // trafficThread = thread(&Traffic::trafficLoop, &intersection);
    // trafficLights = thread(&Traffic::trafficLightsLoop, &intersection);
    // runningTraffic = thread(&Traffic::passVehiclesThroughIntersection, &intersection);

    Traffic intersection;


    if (!glfwInit()) return 1;

    GLFWwindow *window = glfwCreateWindow(1280, 720, "Traffic Sim Learning", nullptr, nullptr);
    if (!window) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    int lanes = 3; // Change this to 1 or 2 to see it scale!
    int speed = 45;
    int northSouthLightTimer = 5;
    int eastWestLightTimer = 5;


    // bool setup = true;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport();


        // UI Controls for testing
        // ImGui::Begin("Settings");
        // ImGui::SliderInt("Lanes", &lanes, 1, 5);
        // ImGui::End();

        if (!intersection.running) {
           //  //speed
           // ImGui::Begin("Speed Limit", nullptr, window_flags);
           //  ImGui::SliderInt("Speed", &speed, 1, 85);
           //  ImGui::End();
           //
           //  //lanes
           //  // 1. Define these as static so they persist between frames
           //  static int item_current_idx = 2; // Default to index 2 (which is "3")
           //  const char *lane_options[] = {"1", "2", "3"};
           //
           //  ImGui::Begin("Lane Selection", nullptr, window_flags);
           //  // 2. The Combo function handles the logic and the string display
           //  if (ImGui::Combo("Lanes", &item_current_idx, lane_options, IM_ARRAYSIZE(lane_options))) {
           //      // 3. Update the actual 'lanes' variable used in your math
           //      // Index 0 = 1 lane, Index 1 = 2 lanes, etc.
           //      lanes = item_current_idx + 1;
           //  }
           //  ImGui::End();
           //

            setupMenu(window_flags, intersection, lanes, speed, northSouthLightTimer, eastWestLightTimer);


           ImGui::Begin ("Begin Simulation");
           if (ImGui::Button("Begin", ImVec2(100,50))) {

               createIntersection(intersection, static_cast<uint8_t>(lanes), static_cast<uint8_t>(speed), static_cast<uint8_t>(northSouthLightTimer), static_cast<uint8_t>(eastWestLightTimer));

               // SimulationUI ui(intersection);


               intersection.running = true;
               trafficThread = thread(&Traffic::trafficLoop, &intersection);
               trafficLights = thread(&Traffic::trafficLightsLoop, &intersection);
               runningTraffic = thread(&Traffic::passVehiclesThroughIntersection, &intersection);

               // setup = false;
           }

           ImGui::End();

        }


        // The Main Drawing
        buildRoad(lanes, window_flags, intersection);

        if (intersection.running){
            drawTrafficLightState(window_flags, intersection);
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


    intersection.running = false;

    trafficThread.join();
    trafficLights.join();
    runningTraffic.join();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();


    printIntersection(intersection);
    return 0;
}
