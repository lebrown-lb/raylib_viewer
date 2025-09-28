/*******************************************************************************************
*
*   raylib [models] example - mesh picking
*
*   Example complexity rating: [★★★☆] 3/4
*
*   Example originally created with raylib 1.7, last time updated with raylib 4.0
*
*   Example contributed by Joel Davis (@joeld42) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2017-2025 Joel Davis (@joeld42) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include "include/common.h"
#include "include/openmenu.h"
#include "include/movemenu.h"
#include "include/rotatemenu.h"
#include "include/scalemenu.h"
#include "include/savemenu.h"

#undef FLT_MAX
#define FLT_MAX     340282346638528859811704183484516925440.0f     // Maximum value of a float, from bit pattern 01111111011111111111111111111111


struct QUAD{
    Vector3 g0;
    Vector3 g1;
    Vector3 g2;
    Vector3 g3;
};


QUAD generate_quad(int g_cnt, float g_size)
{
    QUAD result;
    float x = (float)(g_cnt/2) * g_size;
    result.g0 = (Vector3){ -x, 0.0f, -x };
    result.g1 = (Vector3){ -x, 0.0f,  x };
    result.g2 = (Vector3){  x, 0.0f,  x };
    result.g3 = (Vector3){  x, 0.0f, -x };

    return result;
}



//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1500;
    const int screenHeight = 950;
    int grid_count = 20;
    float grid_size = 10.0f;

    InitWindow(screenWidth, screenHeight, "raylib Viewer");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 30.0f, 30.0f, 30.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 8.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.6f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Ray ray = { 0 };        // Picking ray

    std::vector<MODELX> models;
    OpenMenu open_menu(screenWidth, screenHeight);
    MoveMenu move_menu(screenWidth, screenHeight);
    RotateMenu rotate_menu(screenWidth, screenHeight);
    ScaleMenu scale_menu(screenWidth, screenHeight);
    SaveMenu save_menu(screenWidth, screenHeight);
    STATE currentState = MAIN_STATE;

    // Ground quad
    QUAD ground = generate_quad(grid_count, grid_size);


    Vector3 location = (Vector3){0.0f, 0.0f, 0.0f };


    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {

        // Update
        //----------------------------------------------------------------------------------
        if (IsCursorHidden()) UpdateCamera(&camera, CAMERA_FREE);          // Update camera

        // Toggle camera controls
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        // Display information about closest hit
        RayCollision collision = { 0 };
        const char *hitObjectName = "None";
        collision.distance = FLT_MAX;
        collision.hit = false;
        Color cursorColor = WHITE;

        // Get ray and test against objects
        ray = GetScreenToWorldRay(GetMousePosition(), camera);

        // Check ray collision against ground quad
        RayCollision groundHitInfo = GetRayCollisionQuad(ray, ground.g0, ground.g1, ground.g2, ground.g3);

        if ((groundHitInfo.hit) && (groundHitInfo.distance < collision.distance))
        {
            collision = groundHitInfo;
            cursorColor = GREEN;
            hitObjectName = "Ground";
        }

        COLLISION mesh;
        mesh.modelName = "none";
        mesh.hitInfo.hit = false;
        mesh.hitInfo.distance = FLT_MAX;

        for(size_t i = 0; i < models.size(); i++)
        {
            // Check ray collision against model meshes
            RayCollision meshHitInfo = { 0 };
            for (int m = 0; m < models[i].model.meshCount; m++)
            {
                // NOTE: We consider the model.transform for the collision check but
                // it can be checked against any transform Matrix, used when checking against same
                // model drawn multiple times with multiple transforms
                meshHitInfo = GetRayCollisionMesh(ray, models[i].model.meshes[m], models[i].model.transform);
                if (meshHitInfo.hit)
                {
                    // Save the closest hit mesh
                    if ((!mesh.hitInfo.hit) || (mesh.hitInfo.distance > meshHitInfo.distance))
                    {
                        mesh.hitInfo = meshHitInfo;
                        mesh.modelName = models[i].name;
                    }

                    break;  // Stop once one mesh collision is detected, the colliding mesh is m
                }
            }
        }


        if(mesh.hitInfo.hit)
        {
            collision = mesh.hitInfo;
            cursorColor = PURPLE;
            hitObjectName = mesh.modelName.c_str();
        }

        if(currentState == MAIN_STATE)
        {
            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_O))
            {
                currentState = OPEN_MENU_STATE;
                open_menu.setVisible(true);
            }
            else if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_M))
            {
                currentState = MOVE_MENU_STATE;
                move_menu.setVisible(true);
            }
            else if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_R))
            {
                currentState = ROTATE_MENU_STATE;
                rotate_menu.setVisible(true);
            }
            else if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_X))
            {
                currentState = SCALE_MENU_STATE;
                scale_menu.setVisible(true);
            }
            else if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_S))
            {
                currentState = SAVE_MENU_STATE;
                save_menu.setVisible(true);
            }

            if(IsKeyPressed(KEY_DELETE))
            {
                for(size_t i = 0; i < models.size();i++)
                {
                    if(models[i].selected)
                    {
                        UnloadModel(models[i].model);
                        UnloadTexture(models[i].texture);
                        models.erase(models.begin() + i);
                        break;
                    }
                }
            }
        }



        if(open_menu.isVisible() && (currentState == OPEN_MENU_STATE))
            open_menu.handleInput(&currentState,collision,mesh,models);
        else if(move_menu.isVisible() && (currentState == MOVE_MENU_STATE))
            move_menu.handleInput(&currentState,collision,mesh,models);
        else if(rotate_menu.isVisible() && (currentState == ROTATE_MENU_STATE))
            rotate_menu.handleInput(&currentState,mesh,models);
        else if(scale_menu.isVisible() && (currentState == SCALE_MENU_STATE))
            scale_menu.handleInput(&currentState,mesh,models);
        else if(save_menu.isVisible() && (currentState == SAVE_MENU_STATE))
            save_menu.handleInput(&currentState,mesh,models);


        if(mesh.hitInfo.hit)
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                for(size_t i = 0; i < models.size();i++)
                {
                    if(models[i].name == mesh.modelName)
                    {
                        if(models[i].selected)
                            models[i].selected = false;
                        else
                            models[i].selected = true;
                    }
                }
            }
        }
        else if(currentState == MAIN_STATE && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            for(size_t i = 0; i < models.size();i++)
                models[i].selected = false;

        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        // Draw the tower
        // WARNING: If scale is different than 1.0f,
        // not considered by GetRayCollisionModel()
        for(size_t i = 0; i < models.size();i++)
        {
            DrawModel(models[i].model,location , 1.0f, models[i].color);
            // Draw the mesh bbox if we hit it
            if (models[i].selected)
                DrawBoundingBox(models[i].bbox, LIME);
        }


        // If we hit something, draw the cursor at the hit point
        if (collision.hit)
        {
            DrawCube(collision.point, 0.3f, 0.3f, 0.3f, cursorColor);
            DrawCubeWires(collision.point, 0.3f, 0.3f, 0.3f, RED);

            Vector3 normalEnd;
            normalEnd.x = collision.point.x + collision.normal.x;
            normalEnd.y = collision.point.y + collision.normal.y;
            normalEnd.z = collision.point.z + collision.normal.z;

            DrawLine3D(collision.point, normalEnd, RED);
        }

        DrawRay(ray, MAROON);

        DrawGrid(grid_count, grid_size);

        EndMode3D();

            // Draw some debug GUI text
        DrawText(TextFormat("Hit Object: %s", hitObjectName), 10, 50, 10, BLACK);

        if (collision.hit)
        {
            int ypos = 70;

            DrawText(TextFormat("Distance: %3.2f", collision.distance), 10, ypos, 10, BLACK);

            DrawText(TextFormat("Hit Pos: %3.2f %3.2f %3.2f",
                                collision.point.x,
                                collision.point.y,
                                collision.point.z), 10, ypos + 15, 10, BLACK);

            DrawText(TextFormat("Hit Norm: %3.2f %3.2f %3.2f",
                                collision.normal.x,
                                collision.normal.y,
                                collision.normal.z), 10, ypos + 30, 10, BLACK);
        }


        if(open_menu.isVisible())
            open_menu.drawMenu();
        if(move_menu.isVisible())
            move_menu.drawMenu();
        if(rotate_menu.isVisible())
            rotate_menu.drawMenu();
        if(scale_menu.isVisible())
            scale_menu.drawMenu();
        if(save_menu.isVisible())
            save_menu.drawMenu();


        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    for(size_t i = 0; i < models.size();i++)
    {
        UnloadModel(models[i].model);         // Unload model
        UnloadTexture(models[i].texture);     // Unload texture
    }
    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

