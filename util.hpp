#include <vector>
#include <cmath>
#include <string>
#include <chrono>
#include <thread>

class RigidObject {
    public:
    float mass; // take 0 for static objects (exception)
    std::vector<float> velocity; // 2d vector
    std::vector<float> acceleration; // 2d vector (usually with no x component)
    std::vector<float> position; // 2d vector

    RigidObject(float m, std::vector<float> vel, std::vector<float> acc, std::vector<float> pos) : mass(m), velocity(vel), acceleration(acc), position(pos) {}
};

class Circle : public RigidObject {
    public:
    float radius;

    Circle(float r, float m, std::vector<float> vel, std::vector<float> acc, std::vector<float> pos) : RigidObject(m, vel, acc, pos), radius(r) {}
};

class Wall : public RigidObject {
    public:
    float x;
    float y;

    Wall(float width, float height, std::vector<float> vel, std::vector<float> acc, std::vector<float> pos) : RigidObject(0, vel, acc, pos), x(width), y(height) {}
};

std::vector<float> updateVelocity(Circle& circle, float time) { // not for collisions
    std::vector<float> v = circle.velocity;
    v[0] += circle.acceleration[0] * time;
    v[1] += circle.acceleration[1] * time;
    return v;
}

std::vector<float> updatePosition(Circle& circle, float time) {
    std::vector<float> p = circle.position;
    p[0] += circle.velocity[0] * time;
    p[1] += circle.velocity[1] * time;
    return p;
}

float getDistance(std::vector<float> point1, std::vector<float> point2) {
    float num = (float) pow((point1[0] - point2[0]), 2) + (float) pow((point1[1] - point2[1]), 2);
    float dist = (float) sqrt(num);
    return dist;
}

// returns
// posx posy
// velx vely
std::vector<std::vector<float>> handleWallCollision(Wall& wall, Circle& circle, float time, float restitution, float friction) { // res and fric between 0 and 1
    // cases not to handle collision for
    if (circle.position[1] < wall.position[1] - (wall.y / 2) - circle.radius || circle.position[1] > wall.position[1] + (wall.y / 2) + circle.radius) {
        std::vector<std::vector<float>> toreturn = {circle.position, circle.velocity};
        return toreturn;
    }
    if (circle.position[0] < wall.position[0] - (wall.x / 2) - circle.radius || circle.position[0] > wall.position[0] + (wall.x / 2) + circle.radius) {
        std::vector<std::vector<float>> toreturn = {circle.position, circle.velocity};
        return toreturn;
    }
    // handling collision if possible
    int topOrBottom = getDistance(circle.position, wall.position) > (wall.x / 2) ? 0 : 1;
    std::vector<float> velclone = circle.velocity;
    std::vector<float> posclone = circle.position;
    if (topOrBottom) {
        velclone[1] *= -1 * restitution; // makes bouncing possible
        velclone[0] *= (1 - friction);
    } else {
        velclone[0] *= -1 * restitution; // makes bouncing possible
        velclone[1] *= (1 - friction);
    }
    posclone[1] += velclone[1] * time;
    posclone[0] += velclone[0] * time;
    std::vector<std::vector<float>> toreturn = {posclone, velclone};
    return toreturn;
}

class World {
    public:
    int y; // height
    int x; // width
    std::vector<float> g; // acceleration by gravity
    std::string window_name;

    World(int height, int width, std::vector<float> gravity, std::string name) : y(height), x(width), window_name(name), g(gravity) {}

    void gameLoop(float g, float friction, float restitution, int fps, Wall *wallObjects, int numOfWallObjects, Circle *circleObjects, int numOfCircleObjects); // main loop for code
};

void World::gameLoop(float g, float friction, float restitution, int fps, Wall *wallObjects, int numOfWallObjects, Circle *circleObjects, int numOfCircleObjects) { // g must be positive
    double dt = (double) 1 / fps;
    for (int x = 0; x < numOfCircleObjects; x++) {
        circleObjects[x].acceleration = {0, (-1 * g)};
    }
    auto previousTime = std::chrono::steady_clock::now();
    auto frameTime = std::chrono::steady_clock::now() - previousTime; // dummy
    auto accumulator = previousTime - previousTime; // dummy
    while (1) { // main loop for everything
        auto currentTime = std::chrono::steady_clock::now();
        frameTime = currentTime - previousTime;
        previousTime = currentTime;
        accumulator += frameTime;
        while ((std::chrono::duration<double>(accumulator).count()) >= dt) {
            // updating vel and pos
            for (int i = 0; i < numOfCircleObjects; i++) {
                circleObjects[i].velocity = updateVelocity(circleObjects[i], dt);
                circleObjects[i].position = updatePosition(circleObjects[i], dt);
            }
            accumulator -= std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(dt));
            // handling collisions
            std::vector<std::vector<float>> mat {{}, {}};
            for (int i = 0; i < numOfCircleObjects; i++) {
                for (int j = 0; j < numOfWallObjects; j++) {
                    mat = handleWallCollision(wallObjects[j], circleObjects[i], dt, restitution, friction);
                    circleObjects[i].velocity = mat[1];
                    circleObjects[i].position = mat[0];
                }
            }
        }
        // rendering happens here
    }
}