#include <iostream>
#include "util.hpp"

int main() {
    Circle circle1(1, 10, {1, -1}, {0, 0}, {10, 10});
    Circle *circles = new Circle[1];
    Wall wall1(1000, 5, {10, 1});
    Wall *walls = new Wall[1];
    walls[0] = wall1;
    circles[0] = circle1;
    World world({0, -9.8}, "Physics Engine");
    world.gameLoop(0.1, 0.9, 240, walls, 1, circles, 1);
    delete[] circles;
    delete[] walls;
}