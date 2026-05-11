#include <vector>
#include <cmath>

std::vector<float> updateVelocity(std::vector<float> vel, std::vector<float> acc, float time) { // not for collisions
    std::vector<float> v = vel;
    v[1] += acc[1] * time;
    return v;
}

std::vector<float> updatePosition(std::vector<float> pos, std::vector<float> vel, float time) {
    std::vector<float> p = pos;
    p[0] += vel[0] * time;
    p[1] += vel[1] * time;
    return p;
}

std::vector<float> getNormalBetweenCircles(std::vector<float> pos1, float rad1, std::vector<float> pos2, float rad2) { // use only when collision is there
    float num = (float) pow((pos1[0] - pos2[0]), 2) + (float) pow((pos1[1] - pos2[1]), 2);
    float dist = (float) pow(num, 0.5);
    std::vector<float> normal = {(pos1[0] - pos2[0]) / dist, (pos1[1] - pos2[1]) / dist};
    return normal;
}