//
// Created by rusty on 1/17/2026.
//

#pragma once

#include "traffic.h"

#include "imgui.h"


struct Traffic;

class SimulationUI {
public:
    SimulationUI(Traffic &simulationIntersection);

    void render();

private:
    Traffic &simulationIntersection;

    void showLaneStats();

    void showControls();
};

void setupMenu(ImGuiWindowFlags window_flags, Traffic &intersection, int &northSouthLanes, int &eastWestLanes,
               int &speed, int &northSouthLightTimer, int &eastWestLightTimer);

void buildRoad(int northSouthLanes, int eastWestLanes, ImGuiWindowFlags window_flags, Traffic &intersection);

void drawDashedLine(ImDrawList *draw_list, ImVec2 start, ImVec2 voidArea, ImVec2 end, ImU32 color, float thickness,
                    float dash_length);

void drawLanesPerDirection(ImDrawList *draw_list, ImVec2 center, int northSouthLanes, int eastWestLanes,
                           float northSouthRoadWidth, float eastWestRoadWidth, ImVec2 canvas_p0,
                           ImVec2 canvas_p1, ImVec2 hVoid, ImVec2 vVoid);

void drawTrafficLightState(ImGuiWindowFlags window_flags, Traffic &intersection);

void drawTrafficGraphics(ImDrawList *draw_list, int northSouthLanes, int eastWestLanes, float northSouthRoadWidth,
                         float eastWestRoadWidth, ImVec2 canvas_p0,
                         ImVec2 canvas_p1, ImVec2 hVoid, ImVec2 vVoid, float centerX, float centerY);

void displayStats(ImGuiWindowFlags window_flags, const Traffic &intersection);

void speedSlider(ImGuiWindowFlags window_flags, Traffic &intersection);
