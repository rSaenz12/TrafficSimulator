//********************************************
// Author: Russell Saenz
// File: traffic.h
// Description: Declares functions used in the UI: setupMenu, buildRoad, drawLine,
//              drawLanesPerDirection, drawTrafficLightState, drawTrafficGraphics,
//              displayStats, speedSlider
//********************************************

#pragma once

#include "traffic.h"

#include "imgui.h"

void setupMenu(ImGuiWindowFlags windowFlags, Traffic &intersection, int &northSouthLanes, int &eastWestLanes,
               int &speed, int &northSouthLightTimer, int &eastWestLightTimer);

void buildRoad(int northSouthLanes, int eastWestLanes, ImGuiWindowFlags windowFlags, Traffic &intersection);

void drawLine(ImDrawList *draw_list, ImVec2 start, ImVec2 voidArea, ImVec2 end, ImU32 color, float thickness,
                    float dashLength);

void drawLanesPerDirection(ImDrawList *draw_list, ImVec2 center, int northSouthLanes, int eastWestLanes,
                           float northSouthRoadWidth, float eastWestRoadWidth, ImVec2 canvasPosition0,
                           ImVec2 canvasPosition1, ImVec2 horizontalVoid, ImVec2 verticalVoid);

void drawTrafficLightState(ImGuiWindowFlags windowFlags, Traffic &intersection);

void drawTrafficGraphics(ImDrawList *draw_list, int northSouthLanes, int eastWestLanes, float northSouthRoadWidth,
                         float eastWestRoadWidth,
                         ImVec2 horizontalVoid, ImVec2 verticalVoid, float centerX, float centerY);

void displayStats(ImGuiWindowFlags windowFlags, const Traffic &intersection);

void speedSlider(ImGuiWindowFlags windowFlags, Traffic &intersection);