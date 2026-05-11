#include <vector>
#include <string>

std::vector<float> updateVelocity(std::vector<float> vel, std::vector<float> acc, float time) { // for projectile motion only
    std::vector<float> v = vel;
    v[1] += acc[1] * time;
    return v;
}

std::vector<float> updatePosition(std::vector<float> pos, std::vector<float> vel) {
    std::vector<float> p = pos;
    p[0] = pos[0] + vel[0];
    p[1] = pos[1] + vel[1];
    return p;
}