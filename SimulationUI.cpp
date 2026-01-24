// //
// // Created by rusty on 1/17/2026.
// //
//
#include "SimulationUI.h"

#include <cmath>


//
//
// SimulationUI::SimulationUI(Traffic& intersection) : simulationIntersection(intersection) {}
//
// void SimulationUI::render() {
//     ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
//
//     showLaneStats();
//     showControls();
// }
//
// void SimulationUI::showLaneStats() {
//     ImGui::Begin("Lane Statistics");
//
//     ImGui::Text("North Lane:");
//     ImGui::End();
// }

using namespace ImGui;

void setupMenu(ImGuiWindowFlags window_flags, Traffic &intersection, int &lanes, int &speed, int &northSouthLightTimer, int &eastWestLightTimer) {
     //speed
    Begin("Speed Limit", nullptr, window_flags);
     SliderInt("Speed", &speed, 1, 85);
    End();

     //lanes
     // 1. Define these as static so they persist between frames
     static int item_current_idx = 2; // Default to index 2 (which is "3")
     const char *lane_options[] = {"1", "2", "3"};

     Begin("Lane Selection", nullptr, window_flags);
     // 2. The Combo function handles the logic and the string display
     if (Combo("Lanes", &item_current_idx, lane_options, IM_ARRAYSIZE(lane_options))) {
         // 3. Update the actual 'lanes' variable used in your math
         // Index 0 = 1 lane, Index 1 = 2 lanes, etc.
         lanes = item_current_idx + 1;
     }
     End();

    Begin("North / South Light Timer");
    SliderInt("Seconds", &northSouthLightTimer, 1, 255);
    End();

    Begin("East / West Light Timer");
    SliderInt("Seconds", &eastWestLightTimer, 1, 255);
    End();


}

void buildRoad(int lanes, ImGuiWindowFlags window_flags, Traffic &intersection) {
    // using namespace ImGui;


    SetNextWindowPos(ImVec2(250, 50), ImGuiCond_FirstUseEver);
    SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

    Begin("Preview", nullptr, window_flags);

    ImVec2 canvas_p0 = GetCursorScreenPos();
    ImVec2 canvas_sz = GetContentRegionAvail();
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
    ImDrawList* draw_list = GetWindowDrawList();


    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(34, 139, 34, 255));


    float centerX = canvas_p0.x + canvas_sz.x * 0.5f;
    float centerY = canvas_p0.y + canvas_sz.y * 0.5f;
    float laneWidth = 40.0f;
    // float totalRoadWidth = laneWidth * lanes * 2;
    float totalRoadWidth = laneWidth * lanes;


    //void area in middle
    ImVec2 hVoid = ImVec2(centerX - totalRoadWidth / 2, centerX + totalRoadWidth / 2);
    ImVec2 vVoid = ImVec2(centerY - totalRoadWidth / 2, centerY + totalRoadWidth / 2);

    //road
    draw_list->AddRectFilled(ImVec2(canvas_p0.x, centerY - totalRoadWidth/2), ImVec2(canvas_p1.x, centerY + totalRoadWidth/2), IM_COL32(50, 50, 50, 255));
    draw_list->AddRectFilled(ImVec2(centerX - totalRoadWidth/2, canvas_p0.y), ImVec2(centerX + totalRoadWidth/2, canvas_p1.y), IM_COL32(50, 50, 50, 255));

    //yellow center lines
    ImU32 yellow = IM_COL32(255, 255, 0, 255);
    drawDashedLine(draw_list, ImVec2(canvas_p0.x, centerY), hVoid, ImVec2(canvas_p1.x, centerY), yellow, 2.5f, 0.0f); // Solid
    drawDashedLine(draw_list, ImVec2(centerX, canvas_p0.y), vVoid, ImVec2(centerX, canvas_p1.y), yellow, 2.5f, 0.0f); // Solid

    //white lines
    drawLanesPerDirection(draw_list, ImVec2(centerX, centerY), lanes, totalRoadWidth, canvas_p0, canvas_p1, hVoid, vVoid);

    End();
}

void drawTrafficLightState(ImGuiWindowFlags window_flags, Traffic &intersection) {

    // bool NorthSouthLightGreen = false;

    uint8_t currentLight = intersection.checkActiveLane();

    SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    SetNextWindowSize(ImVec2(100, 100), ImGuiCond_FirstUseEver);

    Begin("Light Status", nullptr, window_flags);
    Text("North/South: ");
    SameLine();


    if (currentLight == northSouth) {
        PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));
    }else {
        PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));
    }

    Button("##red", ImVec2(20, 20));
    PopStyleColor();

    Text("East/West: ");
    SameLine();

    if (currentLight == eastWest) {
        PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));
    }else {
        PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));
    }

    Button("##green", ImVec2(20, 20));
    PopStyleColor();

    End();
}

void drawLanesPerDirection(ImDrawList* draw_list, ImVec2 center, int lanes, float totalRoadWidth, ImVec2 canvas_p0, ImVec2 canvas_p1, ImVec2 hVoid, ImVec2 vVoid) {
    ImU32 white = IM_COL32(255, 255, 255, 180);
    float laneDist = totalRoadWidth / (lanes * 2);

    for (int i = 1; i < lanes; i++) {
        float offset = i * laneDist;

        // Vertical Lane Markers (North/South)
        drawDashedLine(draw_list, ImVec2(center.x + offset, canvas_p0.y), vVoid, ImVec2(center.x + offset, canvas_p1.y), white, 1.0f, 15.0f);
        drawDashedLine(draw_list, ImVec2(center.x - offset, canvas_p0.y), vVoid, ImVec2(center.x - offset, canvas_p1.y), white, 1.0f, 15.0f);

        // Horizontal Lane Markers (East/West)
        drawDashedLine(draw_list, ImVec2(canvas_p0.x, center.y + offset), hVoid, ImVec2(canvas_p1.x, center.y + offset), white, 1.0f, 15.0f);
        drawDashedLine(draw_list, ImVec2(canvas_p0.x, center.y - offset), hVoid, ImVec2(canvas_p1.x, center.y - offset), white, 1.0f, 15.0f);
    }
}

void drawDashedLine(ImDrawList* draw_list, ImVec2 start, ImVec2 voidArea, ImVec2 end, ImU32 color, float thickness, float dash_length) {
    ImVec2 diff = { end.x - start.x, end.y - start.y };
    float len = sqrtf(diff.x * diff.x + diff.y * diff.y);
    if (len <= 0.0f) return;

    ImVec2 dir = { diff.x / len, diff.y / len };
    bool isVertical = (fabs(dir.y) > fabs(dir.x));

    //solic
    if (dash_length <= 0.0f) {
        //start to void
        ImVec2 p_void_entry = isVertical ? ImVec2(start.x, voidArea.x) : ImVec2(voidArea.x, start.y);
        draw_list->AddLine(start, p_void_entry, color, thickness);

        //void to end
        ImVec2 p_void_exit = isVertical ? ImVec2(start.x, voidArea.y) : ImVec2(voidArea.y, start.y);
        draw_list->AddLine(p_void_exit, end, color, thickness);

        return;
    }

    //dashed
    for (float i = 0; i < len; i += dash_length * 2) {
        float seg_start_dist = i;
        float seg_end_dist = (i + dash_length > len) ? len : i + dash_length;

        ImVec2 p_start = ImVec2(start.x + dir.x * seg_start_dist, start.y + dir.y * seg_start_dist);
        ImVec2 p_end = ImVec2(start.x + dir.x * seg_end_dist, start.y + dir.y * seg_end_dist);

        float checkCoord = isVertical ? p_start.y : p_start.x;

        //skip if this dash starts inside the intersection
        if (checkCoord > voidArea.x && checkCoord < voidArea.y) {
            continue;
        }

        draw_list->AddLine(p_start, p_end, color, thickness);
    }
}
