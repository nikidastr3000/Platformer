#include "player.h"

#include "Globals.h"
#include "weapon.h"
#include "animation.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/yaml.h>

using namespace std;
using namespace sf;

// Public
Player::Player(): sprite(texture), heartSprite(heartTexture){

    // Constants init
    loadConstants(dataPath + "Configs/classes_constants.yaml");
    currentHealth = maxHealth;

    //width = 68;
    //height = 118;
    width = cellSizeF * 0.6;
    height = cellSizeF * 1.8;
    heartSize = {cellSizeF / 3, cellSizeF / 3};
    healthbarPos = { window.width / 2 - 300, window.height - 130 };


    // Texture init
    if (!texture.loadFromFile(dataPath + "SpriteSheets/player.png")) {
        cout << "Error with loading player texture" << endl;
    }
    texture.setSmooth(false);
    sprite.setTexture(texture, true);
    sprite.setPosition(Vector2f(x, y));


    // Animation init
    loadAnimations("/home/niki/Progr/Games/Platformer/data/Configs/animations.yaml");

    idleAnimation.update("right", 0.f);

    Vector2i originalSize = sprite.getTextureRect().size;
    sprite.setScale({ width / originalSize.x, height / originalSize.y });
    cout << "Player scale: " << sprite.getScale().x << " " << sprite.getScale().y << endl;

    // Heart init
    if (!heartTexture.loadFromFile(dataPath + "Textures/heart.png")) {
        cout << "Error with loading heart texture" << endl;
    }

    heartSprite.setTexture(heartTexture, true);
    originalSize = heartSprite.getTextureRect().size;
    heartSprite.setScale({ heartSize.x / originalSize.x, heartSize.y / originalSize.y });
}

void Player::update(float dt) {
    // movement
    {

    if(!isStunned){
        movement(dt);
    }

    if (inTheAir){
        Vy += gravity * dt;
    }

    restrictions(dt);

    //move camera with sprite if necessary
    if ((x + Vx > pcamera->view.getCenter().x && Vx > 0) || (x + Vx < pcamera->view.getCenter().x && Vx < 0)) {
        pcamera->view.move({ Vx * dt, 0.f });
    }
    //move sprite
    x += Vx * dt;
    y += Vy * dt;
    Vx = 0;
    sprite.setPosition(Vector2f(x, y));

    }  

    // other updates
    if (!isInvincible){
        updateAttack();
    }

    updateAnimation(dt);



    //timers
    if (invincTimer > 0.f) {
        invincTimer -= dt;
        if (blinkStopClock - invincTimer >= invincBlinkDuration) {
            blinkStopClock = invincTimer;
            isVisible = !isVisible;
        }

    }
    else if (isInvincible) {
        isInvincible = false;
        isVisible = true;
    }
        

    if (stunTimer > 0.f) {
        stunTimer -= dt;
    }
    else if (isStunned){
        isStunned = false;
    }
}

void Player::render() {
    if (!isVisible)
        return;

    pwindow->draw(sprite);

    if (isAttack) {
        pwindow->draw(sword.sprite);
    }
}

void Player::renderUI() {

    renderHealthbar();

}

void Player::EventHandler(const ::Event& e){
    switch (e.eventType)
    {
    case EventType::COLLISION:
        if (!isInvincible) {
            Vx = e.direction.x * 20;
            Vy = -0.5;

            currentHealth -= 1;

            isInvincible = true;
            invincTimer = invincDuration;
            blinkStopClock = invincDuration;

            isStunned = true;
            stunTimer = stunDuration;

            isAttack = false;

        }
        break;
        
    case EventType::NONE:
        break;
    }
}

int Player::getX() { return x; }
int Player::getY() { return y; }

int Player::getWidth() { return width; }
int Player::getHeight() { return height; }

bool Player::getIsAttack() { return isAttack; }
Sword Player::getWeapon() { return sword; }

bool Player::getIsInvincible() { return isInvincible; }

void Player::setSword(string PathToTexture, string Name, Vector2f Size, float AttackSpeed, float Damage) {
    sword.init(PathToTexture, Name, Size, AttackSpeed, Damage);
}

// Private
void Player::loadConstants(string pathToConfigFile){
    try {
        YAML::Node constants = YAML::LoadFile(pathToConfigFile)["player"];

        x = constants["startX"].as<int>();
        y = constants["startY"].as<int>();
        direction = constants["startDirection"].as<string>();

        moveSpeed = constants["moveSpeed"].as<float>();
        jumpForce = constants["jumpForce"].as<float>();
        gravity = constants["gravity"].as<float>();

        maxHealth = constants["maxHealth"].as<int>();
        heartOffset = constants["heartOffset"].as<float>();

        invincDuration = constants["invincibilityDuration"].as<float>();
        invincBlinkDuration = constants["invincibilityBlinkDuration"].as<float>();

        stunDuration = constants["stunDuration"].as<float>();

    } catch (const YAML::Exception& e) {
        cerr << "Error with loading classes_constants.yaml file: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

}

void Player::loadAnimations(string pathToConfigFile){
    try {
        YAML::Node animations = YAML::LoadFile(pathToConfigFile)["player"];

        idleAnimation.init(&sprite, &texture, animations["IDLE"]);
        moveAnimation.init(&sprite, &texture, animations["MOVING"]);
        attackAnimation.init(&sprite, &texture, animations["ATTACKING"]);

    } catch (const YAML::Exception& e) {
        cerr << "Error with loading animations.yaml file: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void Player::restrictions(float dt) {
    // if sprite above the screen       (not good)
    if (y + Vy * dt < 0) {
        return;
    }

    // real grid_pos of sprite now
    int left_r = static_cast<int>((x - pmap->topleft.x) / cellSizeU);
    int right_r = static_cast<int>((x + sprite.getGlobalBounds().size.x - pmap->topleft.x) / cellSizeU);
    int up_r = static_cast<int>((y - pmap->topleft.y) / cellSizeU);
    int down_r = static_cast<int>((y + sprite.getGlobalBounds().size.y - pmap->topleft.y) / cellSizeU);

    // virtual grid_pos of sprite if + Vx/Vy
    int left_v = static_cast<int>(((x + Vx * dt) - pmap->topleft.x) / cellSizeU);
    int right_v = static_cast<int>(((x + Vx * dt) + sprite.getGlobalBounds().size.x - pmap->topleft.x) / cellSizeU);
    int up_v = static_cast<int>(((y + Vy * dt) - pmap->topleft.y) / cellSizeU);
    int down_v = static_cast<int>(((y + Vy * dt) + sprite.getGlobalBounds().size.y - pmap->topleft.y) / cellSizeU);

    // inTheAir check
    bool isBlockDown = false;
    for (int i = left_r; i <= right_r; i++) {
        if (pmap->array[i][down_r + 1])
            isBlockDown = true;
    }

    if (!inTheAir) {
        if (static_cast<int>(y + sprite.getGlobalBounds().size.y + 1 - pmap->topleft.y) % cellSizeU != 0) {
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
                }
            }
    }

    // left side collision check
    if (Vx < 0) {
        if (x + Vx * dt < 0) {  // left map border
            x = 0;
            Vx = 0;
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
                }
            }
    }

    // under collision check
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

    // above collision check
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

void Player::movement(float dt) {
    int dir = 0;

    if (Keyboard::isKeyPressed(Keyboard::Scancode::D) || Keyboard::isKeyPressed(Keyboard::Scancode::Right)) {
        dir = 1;
    }
    if (Keyboard::isKeyPressed(Keyboard::Scancode::A) || Keyboard::isKeyPressed(Keyboard::Scancode::Left)) {
        if (dir == 1)
            dir = 0;
        else
            dir = -1;
    }
    if (dir == 1) {
        Vx = moveSpeed;

        if (animationState != ATTACKING){
            animationState = MOVING;
            direction = "right";
        }
    }
    else if (dir == -1) {
        Vx = -moveSpeed;

        if (animationState != ATTACKING){
            animationState = MOVING;
            direction = "left";
        }
    }
    else if (dir == 0){
        if (animationState != ATTACKING){
            animationState = IDLE;
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Scancode::Space)) {
        jump(jumpForce);
    }
}

void Player::jump(float force) {
    if (!inTheAir) {
        //inTheAir = true;
        Vy = -force;
    }
}

void Player::renderHealthbar() {
    float x = healthbarPos.x;
    float y = healthbarPos.y;
    for (int i = 0; i < currentHealth; i++) {
        x += heartOffset;
        heartSprite.setPosition({x, y});
        pwindow->draw(heartSprite);
        x += heartSize.x;
    }

}

void Player::updateAttack() {
    if (Mouse::isButtonPressed(Mouse::Button::Left)) {
        if (true) {        // if its begin of attack
            //finding dx between player's center and mousecursor
            Vector2f globalPlayerCenter = sprite.getGlobalBounds().getCenter();        // global(world) position(center)
            Vector2i playerCenter = pwindow->mapCoordsToPixel(globalPlayerCenter, pcamera->view);
            Vector2i mousePos = Mouse::getPosition(*pwindow);
            float dx = mousePos.x - playerCenter.x;
            
            // if animation has been interrupted
            if (!attackAnimation.restart()){

                // if prev animation was in the left and this will be in the left
                if (direction == "left" and dx < 0){
                    this->x -= sprite.getScale().x * attackAnimation.getLeftAnimationOffset(); 
                    //cout << "shift to right(1): " << -sprite.getScale().x * attackAnimation.getLeftAnimationOffset() << endl;
                }

                // if prev animation was in the left and this will be in the right
                if (direction == "left" and dx >= 0){
                    this->x -= sprite.getScale().x * attackAnimation.getLeftAnimationOffset();
                    //cout << "shift to right(1): " << -sprite.getScale().x * attackAnimation.getLeftAnimationOffset() << endl;
                }

            }

            if (dx >= 0){
                direction = "right";
            }
            else if (dx < 0){
                direction = "left";
            }

            isAttack = true;
            animationState = ATTACKING;

            // shift player
            if (direction == "left"){
                this->x += sprite.getScale().x * attackAnimation.getLeftAnimationOffset();
                //cout << "shift to left: " << sprite.getScale().x * attackAnimation.getLeftAnimationOffset() << endl;
            }
        }        
    }
}

void Player::updateAnimation(float dt){

    if (animationState == MOVING){
        moveAnimation.update(direction, dt);
    }
    else if(animationState == IDLE){
        idleAnimation.update(direction, dt);
    }
    else if(animationState == ATTACKING){
        if (attackAnimation.update(direction, dt)){       // while attacking
            
        }
        else if(animationState == ATTACKING){       // end of attacking
            //reset texture
            animationState = IDLE;
            isAttack = false;

            // shift player back
            if (direction == "left"){
                this->x -= sprite.getScale().x * attackAnimation.getLeftAnimationOffset();
                //cout << "shift to right(2): " << -sprite.getScale().x * attackAnimation.getLeftAnimationOffset() << endl;
            }
        }
    }
}

