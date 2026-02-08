#include "monster.h"

#include "Globals.h"
#include "eventSystem.h"
#include "weapon.h"

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

// Public
Monster::Monster(float X, float Y, int Width, int Height, float AnimationSpeed, string PathToTexture)
    : x(X), y(Y), width(Width), height(Height), animationSpeed(AnimationSpeed), sprite(texture) {

    if (!texture.loadFromFile(PathToTexture)) {
        cout << "Error with loading monster texture" << endl;
    }

    sprite.setPosition(Vector2f(x, y));
    sprite.setTexture(texture, true);

    Vector2i originalSize = sprite.getTextureRect().size;
    sprite.setScale({ width / originalSize.x, height / originalSize.y });

}

void Monster::update(float dt) {
    if (!isStunned)
        movement(dt);

    if (inTheAir) 
        Vy += gravity * dt;

    restrictions(dt);

    checkCollision();

    x += Vx * dt;
    y += Vy * dt;
    Vx = 0;
    sprite.setPosition(Vector2f(x, y));

    // ifs
    if (hp <= 0) {
        isAlive = false;
    }

    //timers
    if (invincTimer > 0.f) {
        invincTimer -= timeStep;
    }
    else if (isInvincible)
        isInvincible = false;

    if (stunTimer > 0.f) {
        stunTimer -= timeStep;
    }
    else if (isStunned)
        isStunned = false;
}

void Monster::render() {
    pwindow->draw(sprite);
}

bool Monster::getIsAlive() const { return isAlive; }

// Private
void Monster::restrictions(float dt) {
    // if sprite above the screen       (not good)
    if (y + Vy * dt < 0) {
        return;
    }

    // real grid_pos of sprite now
    int left_r = static_cast<int>((x - pmap->topleft.x) / cellSizeF);
    int right_r = static_cast<int>((x + sprite.getGlobalBounds().size.x - pmap->topleft.x) / cellSizeF);
    int up_r = static_cast<int>((y - pmap->topleft.y) / cellSizeF);
    int down_r = static_cast<int>((y + sprite.getGlobalBounds().size.y - pmap->topleft.y) / cellSizeF);

    // virtual grid_pos of sprite if + Vx/Vy
    int left_v = static_cast<int>(((x + Vx * dt) - pmap->topleft.x) / cellSizeF);
    int right_v = static_cast<int>(((x + Vx * dt) + sprite.getGlobalBounds().size.x - pmap->topleft.x) / cellSizeF);
    int up_v = static_cast<int>(((y + Vy * dt) - pmap->topleft.y) / cellSizeF);
    int down_v = static_cast<int>(((y + Vy * dt) + sprite.getGlobalBounds().size.y - pmap->topleft.y) / cellSizeF);

    // inTheAir check
    bool isBlockDown = false;
    for (int i = left_r; i <= right_r; i++) {
        if (pmap->array[i][down_r + 1])
            isBlockDown = true;
    }

    if (!inTheAir) {
        if (static_cast<int>(y + sprite.getGlobalBounds().size.y + 1 - pmap->topleft.y) % static_cast<int>(cellSizeF) != 0) {
            inTheAir = true;
        }
        else if (!isBlockDown) {
            inTheAir = true;
        }
    }

    // right side collision check
    if (Vx > 0) {
        if (right_v >= pmap->getWidth()) {  // right map border
            x = pmap->topleft.x + (right_r + 1) * cellSizeF - sprite.getGlobalBounds().size.x - 1;
            Vx = 0;
            direction = "left";
        }
        else
            if (right_v > right_r) {    // if sprite steps on new block
                bool blockRight = false;
                for (int i = up_r; i <= down_r; i++) {
                    if (pmap->array[right_v][i])
                        blockRight = true;
                }

                if (blockRight) {
                    x = pmap->topleft.x + (right_r + 1) * cellSizeF - sprite.getGlobalBounds().size.x - 1;
                    Vx = 0;
                    direction = "left";
                }
            }
    }

    // left side collision check
    if (Vx < 0) {
        if (x + Vx * dt < 0) {  // left map border
            x = 0;
            Vx = 0;
            direction = "right";
        }
        else
            if (left_v < left_r) {  // if sprite steps on new block
                bool blockLeft = false;
                for (int i = up_r; i <= down_r; i++) {
                    if (pmap->array[left_v][i])
                        blockLeft = true;
                }

                if (blockLeft) {
                    x = pmap->topleft.x + (left_v + 1) * cellSizeF + 1;
                    Vx = 0;
                    direction = "right";
                }
            }
    }

    // above collision check
    if (Vy > 0) {
        if (down_v > down_r) {  // if sprite steps on new block
            bool isBlockDown = false;
            for (int i = left_r; i <= right_r; i++) {
                if (pmap->array[i][down_v])
                    isBlockDown = true;
            }

            if (isBlockDown) {
                y = pmap->topleft.y + (down_r + 1) * cellSizeF - sprite.getGlobalBounds().size.y - 1;
                Vy = 0;
                inTheAir = false;

                //pcamera->stabilizationByY(y - pcamera->view.getCenter().y);
            }
        }
    }

    // under collision check
    if (Vy < 0) {
        if (up_v < up_r) {  // if sprite steps on new block
            bool blockUp = false;
            for (int i = left_r; i <= right_r; i++) {
                if (pmap->array[i][up_v])
                    blockUp = true;
            }

            if (blockUp) {
                y = pmap->topleft.y + (up_v + 1) * cellSizeF + 1;
                Vy = 0.3;
            }
        }
    }
}

void Monster::movement(float dt) {
    if (direction == "right") {
        Vx = moveSpeed;
    }
    else if (direction == "left") {
        Vx = -moveSpeed;
    }

    /*if (Keyboard::isKeyPressed(Keyboard::Scancode::Up)) {
        jump(jumpForce);
    }*/



}

void Monster::jump(float force) {
    if (!inTheAir) {
        inTheAir = true;
        Vy = -force;
    }
}

void Monster::checkCollision() {
    if (!(x + width <= pplayer->getX() ||
        pplayer->getX() + pplayer->getWidth() <= x ||
        y + height <= pplayer->getY() ||
        pplayer->getY() + pplayer->getHeight() <= y) &&
        !pplayer->getIsInvincible()) {

        ::Event e;
        e.eventType = EventType::COLLISION;
        e.direction = { Vx, Vy };
        EventDispatcher::Get().SendEvent(e);

        // small collision effect
        isStunned = true;
        stunTimer = stunDuration / 2;
        Vy = -0.2;
        if (pplayer->getX() < x) {
            Vx = 10;
        }
        else
            Vx = -10;
    }

    if (pplayer->getIsAttack() && !isInvincible) {
        Sword playerWeapon = pplayer->getWeapon();
         
        Sprite weaponSprite = playerWeapon.getSprite();
        FloatRect weapon = weaponSprite.getGlobalBounds();        // global(world) position
        FloatRect monster = sprite.getGlobalBounds();        // global(world) position

        if (weapon.findIntersection(monster)) {
            isInvincible = true;
            invincTimer = invincDuration;

            isStunned = true;
            stunTimer = stunDuration;

            Vy = -0.5;
            if (weapon.position.x < monster.position.x) {
                Vx = 10;
            }
            else
                Vx = -10;

            hp -= playerWeapon.getDamage();
        }
    }

}

