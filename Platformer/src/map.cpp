#include "map.h"

#include "Globals.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;


Map::Map(string pathToMap, string pathToBlockTexture = "") :block(blockTexture) {

    // Read map from file
    ifstream file(pathToMap);
    if (!file) {
        cout << "Error with opening map.txt" << endl;
    }

    // Write it to _array(reversed)
    vector<vector <int>> _array;
    string line;
    while (getline(file, line)) {
        vector<int> row;
        for (char c : line) {
            if (c == '0' || c == '1') {
                row.push_back(c - '0');
            }
        }
        if (!row.empty()) {
            _array.push_back(row);
        }
    }
    file.close();

    width = _array[0].size(); height = _array.size();

    // Reverse _array to normal array
    array.resize(width, vector<int>(height, 0));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            array[x][y] = _array[y][x];
        }
    }

    // Textures & Sprites
    Image transparentImage({ cellSizeU, cellSizeU }, Color(0, 0, 0, 0));
    Texture transparentTexture;
    if (!transparentTexture.loadFromImage(transparentImage))
        cout << "Error with loading the emptyTexture" << endl;
    Sprite transparentSprite(transparentTexture);



    if (!blockTexture.loadFromFile(pathToBlockTexture))
        cout << "Error with loading the blockTexture" << endl;
    block.setTexture(blockTexture, true);
    Vector2i size = block.getTextureRect().size;
    block.setScale({ cellSizeF / size.x, cellSizeF / size.y });

    // Fill out the tileMap with Sprites
    tileMap.resize(width, vector<Sprite>(height, block));
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (array[x][y] == 0)
                tileMap[x][y].setColor(Color(0, 0, 0, 0));
            tileMap[x][y].setPosition({ topleft.x + x * cellSizeF, topleft.y + y * cellSizeF });
        }
    }
}

void Map::drawTheMap() {
    //Calculate visible area
    int fromX = (pcamera->view.getCenter().x - pcamera->view.getSize().x / 2.f - topleft.x) / cellSizeF - 1;
    int fromY = (pcamera->view.getCenter().y - pcamera->view.getSize().y / 2.f - topleft.y) / cellSizeF - 1;
    int toX = (pcamera->view.getCenter().x + pcamera->view.getSize().x / 2.f - topleft.x) / cellSizeF + 1;
    int toY = (pcamera->view.getCenter().y + pcamera->view.getSize().y / 2.f - topleft.y) / cellSizeF + 1;

    if (fromX < 0)
        fromX = 0;
    else if (fromX >= width)
        fromX = width - 1;

    if (toX < 0)
        toX = 0;
    else if (toX >= width)
        toX = width - 1;

    if (fromY < 0)
        fromY = 0;
    else if (fromY >= height)
        fromY = height - 1;

    if (toY < 0)
        toY = 0;
    else if (toY >= height)
        toY = height - 1;

    for (int x = fromX; x <= toX; x++) {
        for (int y = fromY; y <= toY; y++) {
            if (array[x][y] == 1)
                pwindow->draw(tileMap[x][y]);
        }
    }
}

float Map::getWidth() { return width; };
float Map::getHeight() { return height; };