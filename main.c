#include "stdio.h"
#include "raylib.h"
#include "math.h"

typedef struct
{
    double x;
    double y;
    int radius;
    double x_vel;
    double y_vel;
    double mass;
    Color color;
} CelestialBody;

const double AU = 149.6e6 * 1000;
const double G = 6.67428e-11;
const int TIME = 3600 * 24;

const int WWIDTH = 800;
const int WHEIGHT = 800;

const int TOTAL_BODIES = 9;
CelestialBody bodies[9];

double scale = 150 / AU;
double scale_radius = 1;

typedef struct {
    double fx;
    double fy;
} Force;

void draw_body(CelestialBody body) {
    double x = body.x * scale + WWIDTH / 2;
    double y = body.y * scale + WHEIGHT / 2;
    DrawCircle((int)x, (int)y, body.radius * scale_radius, body.color);
}

Force attraction_bodies(CelestialBody body, CelestialBody other) {
    double dist_x = other.x - body.x;
    double dist_y = other.y - body.y;
    double distance = sqrt(dist_x * dist_x + dist_y * dist_y);

    if (distance == 0) return (Force){0, 0};

    double force = G * body.mass * other.mass / (distance * distance);
    double theta = atan2(dist_y, dist_x);
    return (Force){cos(theta) * force, sin(theta) * force};
}

void update_body_position(CelestialBody* body) {
    double total_fx = 0, total_fy = 0;

    for (int i = 0; i < TOTAL_BODIES; i++) {
        if (body == &bodies[i])
            continue;

        Force f = attraction_bodies(*body, bodies[i]);
        total_fx += f.fx;
        total_fy += f.fy;
    }

    body->x_vel += total_fx / body->mass * TIME;
    body->y_vel += total_fy / body->mass * TIME;

    body->x += body->x_vel * TIME;
    body->y += body->y_vel * TIME;
}

int main(void) {
    // Sun
    bodies[0] = (CelestialBody){0, 0, 30, 0, 0, 1.98892 * pow(10, 30), YELLOW}; 
    // Mercury
    bodies[1] = (CelestialBody){0.387 * AU, 0, 5, 0, -47870, 3.3011 * pow(10, 23), DARKGRAY}; 
    // Venus
    bodies[2] = (CelestialBody){0.723 * AU, 0, 12, 0, -35020, 4.867 * pow(10, 24), GOLD}; 
    // Earth
    bodies[3] = (CelestialBody){-1 * AU, 0, 16, 0, 29780, 5.9742 * pow(10, 24), BLUE}; 
    // Mars
    bodies[4] = (CelestialBody){-1.524 * AU, 0, 10, 0, 24077, 6.39 * pow(10, 23), RED}; 
    // Jupiter
    bodies[5] = (CelestialBody){-5.204 * AU, 0, 25, 0, 13070, 1.898 * pow(10, 27), ORANGE}; 
    // Saturn
    bodies[6] = (CelestialBody){-9.582 * AU, 0, 21, 0, 9690, 5.683 * pow(10, 26), YELLOW}; 
    // Uranus
    bodies[7] = (CelestialBody){-19.18 * AU, 0, 18, 0, 6810, 8.681 * pow(10, 25), {50, 50, 255}}; 
    // Neptune
    bodies[8] = (CelestialBody){-30.07 * AU, 0, 17, 0, 5430, 1.024 * pow(10, 26), BLUE}; 

    InitWindow(WWIDTH, WHEIGHT, "Solar System Simulation");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_EQUAL)) { scale *= 1.1; scale_radius *= 1.05;}  // Zoom in
        if (IsKeyDown(KEY_MINUS)) { scale /= 1.1; scale_radius /= 1.05;}  // Zoom out

        BeginDrawing();

        ClearBackground(BLACK);
        DrawText("Use '+' and '-' to zoom in and out!", 3, 3, 20, WHITE);

        DrawText("The Sun", 350, 300, 24, WHITE);

        for (int i = 0; i < TOTAL_BODIES; i++) {
            update_body_position(&bodies[i]);
            draw_body(bodies[i]);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
