#pragma once
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Sword {
    friend class Player;
public:
    Sword();

    Sword(string PathToTexture, string Name, Vector2f Size, float AttackSpeed, float Damage);

    void init(string PathToTexture, string Name, Vector2f Size, float AttackSpeed, float Damage);

    Sprite getSprite();
    float getDamage();

private:
    Sprite sprite;
    Texture texture;
    string name{};
    Vector2f size{};
    float attackSpeed{};
    float damage{};
};