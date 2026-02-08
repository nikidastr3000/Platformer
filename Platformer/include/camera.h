#pragma once
#include "map.h"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Camera {
public:
    View view;

    Camera(Vector2f Size, Vector2f Center, float MoveSpeed);

    void setVx(float speed);

    void stabilizationByY(float distance);

    void update(Map* map);
private:
    Vector2f startPos{0, 1000};
    float moveSpeed{};
    float Vx{}, Vy{};   
    float stabilizationDistance{};
    float stabilizationSpeed = 5;
    float stabilizationAcceleration = 0.02f;

    void restrictions(Map* map);
};