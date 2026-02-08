#include "camera.h"

#include "map.h"
#include "Globals.h"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

Camera::Camera(Vector2f Size, Vector2f Center, float MoveSpeed) :moveSpeed(MoveSpeed) {
    view.setSize(Size);
    view.setCenter(Center);
    
    stabilizationByY(startPos.y); 
}

void Camera::setVx(float speed) {
    Vx = speed;
}

void Camera::stabilizationByY(float distance) {
    stabilizationDistance = distance;
    Vy = stabilizationDistance / (1000 / stabilizationSpeed);
}

void Camera::update(Map* map) {
    restrictions(map);

    if (Vy != 0) {
        if ((stabilizationDistance > 0 && stabilizationDistance - Vy <= 0) || (stabilizationDistance < 0 && stabilizationDistance - Vy >= 0)) {
            stabilizationDistance = 0;
            Vy = 0;
        }
        else {
            stabilizationDistance -= Vy;

            if (Vy > 0 && Vy - stabilizationAcceleration > 0.5)
                Vy -= stabilizationAcceleration;

            else if (Vy < 0 && Vy + stabilizationAcceleration < -0.5)
                Vy += stabilizationAcceleration;
        }
    }

    view.move({ Vx, Vy });
    Vx = 0;
}

void Camera::restrictions(Map* map) {
    if (view.getCenter().x + Vx - view.getSize().x / 2.f < map->topleft.x) {
        Vx = map->topleft.x + view.getSize().x / 2.f - view.getCenter().x;
    }
    else if (view.getCenter().x + Vx + view.getSize().x / 2.f > map->topleft.x + map->getWidth() * cellSizeF) {
        Vx = map->topleft.x + map->getWidth() * cellSizeF - view.getSize().x / 2.f - view.getCenter().x;
    }

    if (view.getCenter().y + Vy - view.getSize().y / 2.f < map->topleft.y) {
        Vy = map->topleft.y + view.getSize().y / 2.f - view.getCenter().y;
    }
    else if (view.getCenter().y + Vy + view.getSize().y / 2.f > map->topleft.y + map->getHeight() * cellSizeF) {
        Vy = map->topleft.y + map->getHeight() * cellSizeF - view.getSize().y / 2.f - view.getCenter().y;
    }
}