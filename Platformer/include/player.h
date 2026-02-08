#pragma once

#include "eventSystem.h"
#include "weapon.h"
#include "animation.h"

#include <SFML/Graphics.hpp>
#include <yaml-cpp/yaml.h>

using namespace std;
using namespace sf;

class Player : public IEventHandler {
private:
    enum AnimationState {
        IDLE,
        MOVING,
        ATTACKING
    };

public:
    Player();

    void update(float dt);

    void render();

    void renderUI();

    void EventHandler(const ::Event& e) override;

    int getX();
    int getY();

    int getWidth();
    int getHeight();

    bool getIsAttack();
    Sword getWeapon();

    bool getIsInvincible();

    void setSword(string PathToTexture, string Name, Vector2f Size, float AttackSpeed, float Damage);

private:
    void loadConstants(string pathToConfigFile);

    void loadAnimations(string pathToConfigFile);

    void restrictions(float dt);

    void movement(float dt);

    void jump(float force);

    void renderHealthbar();

    void updateAttack();

    void updateAnimation(float dt);

private:
    // basa
    Sprite sprite;
    Texture texture;
    float width{}, height{};

    // Movement
    float x{}, y{};             // Global coords (not sure)
    float Vx{}, Vy{};
    string direction {};
    float moveSpeed{};
    float jumpForce{};
    float gravity{};
    bool inTheAir = false;
    
    // Health
    int maxHealth{};
    int currentHealth{};
    Sprite heartSprite;
    Texture heartTexture;
    Vector2f heartSize;
    Vector2f healthbarPos;
    float heartOffset{};

    // Effects
    bool isInvincible = false;
    float invincDuration{};     // in milliseconds
    float invincTimer{};
    bool isVisible = true;
    float invincBlinkDuration{};        // Duration of 1 blink during invinc.
    float blinkStopClock{};                 // used for countdown for each blink 

    bool isStunned = false;
    float stunDuration{};
    float stunTimer{};

    // Attack
    Sword sword;

    bool isAttack = false;
    float swordAttackAngle{};

    // Animation
    AnimationState animationState = IDLE;

    Animation* currentAnimation;

    Animation idleAnimation;
    Animation moveAnimation;
    float moveAnimationFrameDur{};
    Animation attackAnimation;
    float attackAnimationFrameDur{};

    bool hasSpriteMap = true;
    Vector2i oneSpriteSize{68, 118};
    IntRect textureRect;


};

