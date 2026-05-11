#include <vector>
#include <string>

class World {
    public:
    int y; // height
    int x; // width
    std::vector<float> g; // acceleration by gravity
    std::string window_name;

    World(int height, int width, std::vector<float> gravity, std::string name) : y(height), x(width), window_name(name), g(gravity) {}

    void gameLoop(int fps, RigidObject object);
};

class RigidObject {
    public:
    float mass;
    std::vector<float> velocity;
    std::vector<float> acceleration;
    std::vector<float> position;
    std::vector<float> force;
};

class Circle : RigidObject {};

class Wall : RigidObject {};