#pragma once
#include "map.h"
#include "camera.h"
#include "player.h"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct O {
    float width{};
    float height{};
};

inline float cellSizeF = 90.f;
inline unsigned int cellSizeU = 90;
inline O window;
inline string windowMode = "FullScreen";
inline string dataPath = "/home/niki/Progr/Games/Platformer/data/";

inline RenderWindow* pwindow;
inline Clock* pclock;
inline Map* pmap;
inline Camera* pcamera;
inline Player* pplayer;