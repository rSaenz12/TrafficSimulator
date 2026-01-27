// //
// // Created by rusty on 1/17/2026.
// //
//
#include "SimulationUI.h"

#include <cmath>

using namespace ImGui;

void setupMenu(const ImGuiWindowFlags window_flags, Traffic &intersection, int &northSouthLanes, int &eastWestLanes,
               int &speed, int &northSouthLightTimer, int &eastWestLightTimer) {
    //speed
    Begin("Speed Limit", nullptr, window_flags);
    SliderInt("Speed", &speed, 1, 85);
    End();

    //lanes
    static int northSouthIndex = 2;
    static int eastWestIndex = 2;
    const char *lane_options[] = {"1", "2", "3"};

    Begin("Lane Selection", nullptr, window_flags);
    if (Combo("North/South Lanes", &northSouthIndex, lane_options, IM_ARRAYSIZE(lane_options))) {
        //index 0 = 1 lane, Index 1 = 2 lanes, etc.
        northSouthLanes = northSouthIndex + 1;
    }

    if (Combo("East/West Lanes", &eastWestIndex, lane_options, IM_ARRAYSIZE(lane_options))) {
        //index 0 = 1 lane, Index 1 = 2 lanes, etc.
        eastWestLanes = eastWestIndex + 1;
    }
    End();

    Begin("North / South Light Timer");
    SliderInt("Seconds", &northSouthLightTimer, 1, 255);
    End();

    Begin("East / West Light Timer");
    SliderInt("Seconds", &eastWestLightTimer, 1, 255);
    End();

    speedSlider(window_flags, intersection);
}

void buildRoad(const int northSouthLanes, const int eastWestLanes, const ImGuiWindowFlags window_flags,
               Traffic &intersection) {
    SetNextWindowPos(ImVec2(250, 50), ImGuiCond_FirstUseEver);
    SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

    Begin("Preview", nullptr, window_flags);

    const ImVec2 canvasPosition0 = GetCursorScreenPos();
    const ImVec2 canvas_sz = GetContentRegionAvail();
    const ImVec2 canvasPosition1 = ImVec2(canvasPosition0.x + canvas_sz.x, canvasPosition0.y + canvas_sz.y);
    ImDrawList *draw_list = GetWindowDrawList();


    draw_list->AddRectFilled(canvasPosition0, canvasPosition1, IM_COL32(34, 139, 34, 255));


    const float centerX = canvasPosition0.x + canvas_sz.x * 0.5f;
    const float centerY = canvasPosition0.y + canvas_sz.y * 0.5f;
    const float laneWidth = 40.0f;
    // float totalRoadWidth = laneWidth * lanes * 2;
    const float northSouthRoadWidth = laneWidth * northSouthLanes;
    const float eastWestRoadWidth = laneWidth * eastWestLanes;


    //void area in middle
    //horizontal left(x) to right(y)
    const ImVec2 horizontalVoid = ImVec2(centerX - northSouthRoadWidth / 2, centerX + northSouthRoadWidth / 2);
    //vertical top(x) to bottom(y)
    const ImVec2 verticalVoid = ImVec2(centerY - eastWestRoadWidth / 2, centerY + eastWestRoadWidth / 2);

    //road
    draw_list->AddRectFilled(ImVec2(canvasPosition0.x, centerY - eastWestRoadWidth / 2),
                             ImVec2(canvasPosition1.x, centerY + eastWestRoadWidth / 2), IM_COL32(50, 50, 50, 255));
    draw_list->AddRectFilled(ImVec2(centerX - northSouthRoadWidth / 2, canvasPosition0.y),
                             ImVec2(centerX + northSouthRoadWidth / 2, canvasPosition1.y), IM_COL32(50, 50, 50, 255));

    //yellow center lines
    ImU32 yellow = IM_COL32(255, 255, 0, 255);
    drawDashedLine(draw_list, ImVec2(canvasPosition0.x, centerY), horizontalVoid, ImVec2(canvasPosition1.x, centerY),
                   yellow, 2.5f,
                   0.0f); // Solid
    drawDashedLine(draw_list, ImVec2(centerX, canvasPosition0.y), verticalVoid, ImVec2(centerX, canvasPosition1.y),
                   yellow, 2.5f,
                   0.0f); // Solid

    //white lines
    drawLanesPerDirection(draw_list, ImVec2(centerX, centerY), northSouthLanes, eastWestLanes, northSouthRoadWidth,
                          eastWestRoadWidth, canvasPosition0, canvasPosition1, horizontalVoid, verticalVoid);

    if (intersection.running) {
        drawTrafficGraphics(draw_list, northSouthLanes, eastWestLanes, northSouthRoadWidth, eastWestRoadWidth,
                            canvasPosition0, canvasPosition1, horizontalVoid, verticalVoid, centerX, centerY);
    }


    End();
}

void drawTrafficLightState(ImGuiWindowFlags window_flags, Traffic &intersection) {
    // bool NorthSouthLightGreen = false;

    const uint8_t currentLight = intersection.checkActiveLane();

    SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    SetNextWindowSize(ImVec2(100, 100), ImGuiCond_FirstUseEver);

    Begin("Light Status", nullptr, window_flags);
    Text("North/South: ");
    SameLine();


    if (currentLight == northSouth) {
        PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));
    } else {
        PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));
    }

    Button("##red", ImVec2(20, 20));
    PopStyleColor();
    if (currentLight == northSouth && greenLightTimeRemaining > chrono::seconds(0)) {
        SameLine();
        Text("Time Remaining: %lld seconds", greenLightTimeRemaining);
    }

    Text("East/West: ");
    SameLine();

    if (currentLight == eastWest) {
        PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));
    } else {
        PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));
    }

    Button("##green", ImVec2(20, 20));
    PopStyleColor();
    if (currentLight == eastWest && greenLightTimeRemaining > chrono::seconds(0)) {
        SameLine();
        Text("Time Remaining: %lld seconds", greenLightTimeRemaining);
    }

    End();
}

void drawTrafficGraphics(ImDrawList *draw_list, int northSouthLanes, int eastWestLanes, float northSouthRoadWidth,
                         float eastWestRoadWidth, ImVec2 canvas_p0, ImVec2 canvas_p1, ImVec2 hVoid, ImVec2 vVoid,
                         float centerX, float centerY) {
    const float steps = 10.0f;

    const ImU32 green = IM_COL32(0, 255, 0, 50);
    const ImU32 yellow = IM_COL32(255, 255, 0, 50);
    const ImU32 red = IM_COL32(255, 0, 0, 50);
    ImU32 currentColor;

    int currentSize = 0;

    int northHeadedSize;
    useLock(northMutex, [&] {
        northHeadedSize = northHeaded.size();
    });

    int southHeadedSize;
    useLock(southMutex, [&] {
        southHeadedSize = southHeaded.size();
    });

    int eastHeadedSize;
    useLock(eastMutex, [&] {
        eastHeadedSize = eastHeaded.size();
    });

    int westHeadedSize;
    useLock(westMutex, [&] {
        westHeadedSize = westHeaded.size();
    });

    if (northHeadedSize > 0) {
        currentSize = ceil(northHeadedSize / northSouthLanes);

        if (currentSize >= 10) {
            currentColor = red;
        } else if (currentSize >= 5) {
            currentColor = yellow;
        } else {
            currentColor = green;
        }

        draw_list->AddRectFilled(ImVec2(centerX, vVoid.y),
                                 ImVec2(centerX + northSouthRoadWidth / 2, vVoid.y + (steps * currentSize)),
                                 currentColor);
    }

    if (southHeadedSize > 0) {
        currentSize = ceil(southHeadedSize / northSouthLanes);

        if (currentSize >= 10) {
            currentColor = red;
        } else if (currentSize >= 5) {
            currentColor = yellow;
        } else {
            currentColor = green;
        }

        draw_list->AddRectFilled(ImVec2(centerX, vVoid.x - (steps * currentSize)),
                                 ImVec2(centerX - northSouthRoadWidth / 2, vVoid.x),
                                 currentColor);
    }

    if (eastHeadedSize > 0) {
        currentSize = ceil(eastHeadedSize / eastWestLanes);


        if (currentSize >= 10) {
            currentColor = red;
        } else if (currentSize >= 5) {
            currentColor = yellow;
        } else {
            currentColor = green;
        }

        draw_list->AddRectFilled(ImVec2(hVoid.x - (steps * currentSize), centerY),
                                 ImVec2(hVoid.x, centerY + eastWestRoadWidth / 2),
                                 currentColor);
    }

    if (westHeadedSize > 0) {
        currentSize = ceil(westHeadedSize / eastWestLanes);

        if (currentSize >= 10) {
            currentColor = red;
        } else if (currentSize >= 5) {
            currentColor = yellow;
        } else {
            currentColor = green;
        }

        draw_list->AddRectFilled(ImVec2(hVoid.y, centerY),
                                 ImVec2(hVoid.y + (steps * currentSize), centerY - eastWestRoadWidth / 2),
                                 currentColor);
    }
}

void drawLanesPerDirection(ImDrawList *draw_list, const ImVec2 center, const int northSouthLanes,
                           const int eastWestLanes, const float northSouthRoadWidth, const float eastWestRoadWidth,
                           const ImVec2 canvas_p0, const ImVec2 canvas_p1, const ImVec2 hVoid, const ImVec2 vVoid) {
    const ImU32 white = IM_COL32(255, 255, 255, 180);
    const float northSouthLaneDistance = northSouthRoadWidth / (northSouthLanes * 2);
    const float eastWestLaneDistance = eastWestRoadWidth / (eastWestLanes * 2);

    int lanes = northSouthLanes >= eastWestLanes ? northSouthLanes : eastWestLanes;

    for (int i = 1; i < lanes; i++) {
        if (i < northSouthLanes) {
            float northSouthOffset = i * northSouthLaneDistance;
            // Vertical Lane Markers (North/South)
            drawDashedLine(draw_list, ImVec2(center.x + northSouthOffset, canvas_p0.y), vVoid,
                           ImVec2(center.x + northSouthOffset, canvas_p1.y), white, 1.0f, 15.0f);
            drawDashedLine(draw_list, ImVec2(center.x - northSouthOffset, canvas_p0.y), vVoid,
                           ImVec2(center.x - northSouthOffset, canvas_p1.y), white, 1.0f, 15.0f);
        }


        if (i < eastWestLanes) {
            float eastWestOffset = i * eastWestLaneDistance;
            // Horizontal Lane Markers (East/West)
            drawDashedLine(draw_list, ImVec2(canvas_p0.x, center.y + eastWestOffset), hVoid,
                           ImVec2(canvas_p1.x, center.y + eastWestOffset), white, 1.0f, 15.0f);
            drawDashedLine(draw_list, ImVec2(canvas_p0.x, center.y - eastWestOffset), hVoid,
                           ImVec2(canvas_p1.x, center.y - eastWestOffset), white, 1.0f, 15.0f);
        }
    }
}

void drawDashedLine(ImDrawList *draw_list, const ImVec2 start, const ImVec2 voidArea, const ImVec2 end,
                    const ImU32 color, const float thickness, const float dash_length) {
    const ImVec2 difference = {end.x - start.x, end.y - start.y};
    const float length = sqrtf(difference.x * difference.x + difference.y * difference.y);
    if (length <= 0.0f) return;

    const ImVec2 direction = {difference.x / length, difference.y / length};
    const bool isVertical = (fabs(direction.y) > fabs(direction.x));

    //solic
    if (dash_length <= 0.0f) {
        //start to void
        const ImVec2 p_void_entry = isVertical ? ImVec2(start.x, voidArea.x) : ImVec2(voidArea.x, start.y);
        draw_list->AddLine(start, p_void_entry, color, thickness);

        //void to end
        const ImVec2 p_void_exit = isVertical ? ImVec2(start.x, voidArea.y) : ImVec2(voidArea.y, start.y);
        draw_list->AddLine(p_void_exit, end, color, thickness);

        return;
    }

    //dashed
    for (float i = 0; i < length; i += dash_length * 2) {
        const float seg_start_dist = i;
        const float seg_end_dist = (i + dash_length > length) ? length : i + dash_length;

        const ImVec2 p_start = ImVec2(start.x + direction.x * seg_start_dist, start.y + direction.y * seg_start_dist);
        const ImVec2 p_end = ImVec2(start.x + direction.x * seg_end_dist, start.y + direction.y * seg_end_dist);

        const float checkCoord = isVertical ? p_start.y : p_start.x;

        //skips over void area
        if (checkCoord > voidArea.x && checkCoord < voidArea.y) {
            continue;
        }

        draw_list->AddLine(p_start, p_end, color, thickness);
    }
}

void displayStats(const ImGuiWindowFlags window_flags, const Traffic &intersection) {
    int northHeadedSize;
    useLock(northMutex, [&] {
        northHeadedSize = northHeaded.size();
    });

    int southHeadedSize;
    useLock(southMutex, [&] {
        southHeadedSize = southHeaded.size();
    });

    int eastHeadedSize;
    useLock(eastMutex, [&] {
        eastHeadedSize = eastHeaded.size();
    });

    int westHeadedSize;
    useLock(westMutex, [&] {
        westHeadedSize = westHeaded.size();
    });

    Begin("Traffic Stats", nullptr, window_flags);
    Text("Speed Limit: %u MPH", intersection.speedLimit);

    Text("North/South Light Duration: %u seconds", intersection.northSouthLight.greenLightTime);

    Text("East/West Light Duration: %u seconds", intersection.eastWestLight.greenLightTime);

    Text("Cars Headed North: %d", northHeadedSize);

    Text("Cars Headed South: %d", southHeadedSize);

    Text("Cars Headed East: %d", eastHeadedSize);

    Text("Cars Headed West: %d", westHeadedSize);

    Text("Total Vehicles: %u", totalDrivers.load());

    Text("Intoxicated Drivers: %u", intoxicatedDrivers.load());

    Text("Distracted Drivers: %u", distractedDrivers.load());

    Text("Accidents: %u", crashes.load());

    //added inline if to avoid dividing by 0
    Text("Total Passengers: %u, Passengers per Vehicle: %d ", totalPassengers.load(),
         static_cast<int>(totalPassengers.load() / (totalDrivers.load() == 0 ? 1 : totalDrivers.load())));

    Text("Total Weight of Vehicles: %u lbs, Average Vehicle Weight: %d lbs", totalWeight.load(),
         static_cast<int>(totalWeight / (totalDrivers.load() == 0 ? 1 : totalDrivers.load())));

    Text("Average Speed of Vehicles: %d MPH", static_cast<int>(totalSpeed.load() / totalDrivers.load()));

    End();
}

void speedSlider(const ImGuiWindowFlags window_flags, Traffic &intersection) {
    int speed = intersection.speedMultiplier;

    Begin("Speed Multiplier", nullptr, window_flags);
    SliderInt("Multiplier", &speed, 1, 5);
    End();

    intersection.speedMultiplier = speed;
}
