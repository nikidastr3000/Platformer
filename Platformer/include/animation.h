#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include <yaml-cpp/yaml.h>
#include <unordered_map>

class Animation {
private:
    enum AnimationType {
        UNKNOWN,
        STATIC,
        LOOPED,
        ONE_SHOT
    };

public:
    Animation();

    void init(sf::Sprite* Sprite, sf::Texture* SpriteSheet, const YAML::Node& data);

    // return false if animation is ended
    bool update(std::string dir, float dt);

    // return false if animation has been interrupted
    bool restart();

    int getLeftAnimationOffset();

private:
    void loadData(const YAML::Node& data);

    AnimationType stringToAnimationType(const std::string& str);

private:
    sf::Sprite* sprite;
    sf::Texture* spriteSheet;


    //sf::Vector2i sheetSize{};
    sf::Vector2i frameSize{};

    std::unordered_map<std::string, std::vector<sf::IntRect>> frames;
    sf::IntRect currentFrame{};
    float oneFrameDuration{};
    float counter{};
    std::string direction{};

    int leftAnimationOffset{};

    AnimationType type{};

    //bools
    bool isAnimationEnded = false;
};