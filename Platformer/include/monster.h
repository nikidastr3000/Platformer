#pragma once
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Monster {
public:
    Monster(float X, float Y, int Width, int Height, float AnimationSpeed, string PathToTexture);

    void update(float dt);

    void render();

    bool getIsAlive() const;

private:
    float x{}, y{}, Vx{}, Vy{};
    float width{}, height{};
    string direction = "right";
    float moveSpeed = 0.3;
    float jumpForce = 1.3;
    float animationSpeed{};
    float animationFrame{};
    float gravity = 0.003;
    bool inTheAir = false;

    Sprite sprite;
    Texture texture;
    bool isAlive = true;

    float hp = 5;
    float timeStep = 0.005;

    bool isInvincible = false;
    float invincDuration = 0.5;
    float invincTimer{};

    bool isStunned = false;
    float stunDuration = 0.5;
    float stunTimer{};

    void restrictions(float dt);

    void movement(float dt);

    void jump(float force);

    void checkCollision();

};
