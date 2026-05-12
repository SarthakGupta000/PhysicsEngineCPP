#include <vector>
#include <cmath>
#include <string>

class World {
    public:
    int y; // height
    int x; // width
    std::vector<float> g; // acceleration by gravity
    std::string window_name;

    World(int height, int width, std::vector<float> gravity, std::string name) : y(height), x(width), window_name(name), g(gravity) {}

    void gameLoop(int fps, RigidObject object); // main loop for code
};

class RigidObject {
    public:
    float mass; // take 0 for static objects (exception)
    std::vector<float> velocity; // 2d vector
    std::vector<float> acceleration; // 2d vector (usually with no x component)
    std::vector<float> position; // 2d vector
    std::vector<float> force; // first zero, then used for adding up the vectors

    RigidObject(float m, std::vector<float> vel, std::vector<float> acc, std::vector<float> pos) : mass(m), velocity(vel), acceleration(acc), position(pos) {
        force = {0, 0};
    }
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

std::vector<float> updateVelocity(Circle circle, float time) { // not for collisions
    std::vector<float> v = circle.velocity;
    v[0] += circle.acceleration[0] * time;
    v[1] += circle.acceleration[1] * time;
    return v;
}

std::vector<float> updatePosition(Circle circle, float time) {
    std::vector<float> p = circle.position;
    p[0] += circle.velocity[0] * time;
    p[1] += circle.velocity[1] * time;
    return p;
}

float getDistance(std::vector<float> point1, std::vector<float> point2) {
    float num = (float) pow((point1[0] - point2[0]), 2) + (float) pow((point1[1] - point2[1]), 2);
    float dist = (float) pow(num, 0.5);
    return dist;
}

// returns
// posx posy
// velx vely
std::vector<std::vector<float>> handleWallCollision(Wall wall, Circle circle, int time) {
    // cases not to handle collision for
    if (circle.position[1] < wall.position[1] - (wall.y / 2) - circle.radius || circle.position[1] > wall.position[1] + (wall.y / 2) + circle.radius) {
        std::vector<std::vector<float>> toreturn = {circle.position, circle.velocity};
        return toreturn;
    }
    if (circle.position[1] < wall.position[1] - (wall.y / 2) - circle.radius && circle.velocity[1] < 0) {
        std::vector<std::vector<float>> toreturn = {circle.position, circle.velocity};
        return toreturn;
    }
    if (circle.position[1] > wall.position[1] + (wall.y / 2) + circle.radius && circle.velocity[1] > 0) {
        std::vector<std::vector<float>> toreturn = {circle.position, circle.velocity};
        return toreturn;
    }
}
