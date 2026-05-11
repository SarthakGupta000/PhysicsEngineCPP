#include <vector>
#include <string>
#include "util_functions.hpp"

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
    float radius;

    Circle(float r, float m, std::vector<float> vel, std::vector<float> acc, std::vector<float> pos) : RigidObject(m, vel, acc, pos) {}
};

class Wall : public RigidObject {
    float x;
    float y;

    Wall(float width, float height, float m, std::vector<float> vel, std::vector<float> acc, std::vector<float> pos) : RigidObject(m, vel, acc, pos) {}
};