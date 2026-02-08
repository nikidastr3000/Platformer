#pragma once

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Map {
private:
    float height{}, width{};

    float stabilizationDistance{};
    float stabilizationSpeed = 10;
    float stabilizationAcceleration = -0.1;

    Texture blockTexture;
    Texture background;
    Sprite block;

    vector<vector <Sprite>> tileMap;

public:
    vector<vector <int>> array;
    Vector2f topleft{ 0, 0 };

    Map(string pathToMap, string pathToBlockTexture);

    void drawTheMap();

    float getWidth();
    float getHeight();
};