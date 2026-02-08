#include "weapon.h"

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

Sword::Sword(): sprite(texture) {};

Sword::Sword(string PathToTexture, string Name, Vector2f Size, float AttackSpeed, float Damage) :
    sprite(texture), name(Name), size(Size), attackSpeed(AttackSpeed), damage(Damage) {

    if (!texture.loadFromFile("data/sword.png")) {
        cout << "Error with loading sword texture" << endl;
    }

    sprite.setTexture(texture, true);

    Vector2i originalSize = sprite.getTextureRect().size;
    sprite.setScale({ size.x / originalSize.x, size.y / originalSize.y });


    // �������
    FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2, bounds.size.y });
}

void Sword::init(string PathToTexture, string Name, Vector2f Size, float AttackSpeed, float Damage){
    name = Name;
    size = Size;
    attackSpeed = AttackSpeed;
    damage = Damage;


    if (!texture.loadFromFile(PathToTexture)) {
        cout << "Error with loading sword texture: " << PathToTexture << endl;
    }

    sprite.setTexture(texture, true);

    Vector2i originalSize = sprite.getTextureRect().size;
    sprite.setScale({ size.x / originalSize.x, size.y / originalSize.y });


    // �������
    FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2, bounds.size.y });
}

Sprite Sword::getSprite() { return sprite; }
float Sword::getDamage() { return damage; }


