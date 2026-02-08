#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

#include "Globals.h"
#include "map.h"
#include "camera.h"
#include "player.h"
#include "monster.h"

using namespace std;
using namespace sf;

void switchWindowMode(Window& Window)
{
    if (windowMode == "Windowed")
    {
        Window.create(VideoMode::getFullscreenModes()[0], "SFML works!", State::Fullscreen);
        windowMode = "FullScreen";
    }
    else
    {
        Window.create(VideoMode({ 800, 500 }), "SFML View");
        windowMode = "Windowed";
    }
}

int main()
{
    // Init game
    float dt = 0.f;
    Clock dtClock;

    // Init window
    RenderWindow Window;
    pwindow = &Window;

    Window.create(VideoMode::getFullscreenModes()[0], "SFML works!", State::Fullscreen);


    window.width = Window.getSize().x;
    window.height = Window.getSize().y;

    // Init camera
    Camera camera({ 1920, 1080 }, { window.width / 2.f,  window.height / 2.f }, 300.f);
    pcamera = &camera;

    // Init game elements
    RectangleShape button({ 100, 50 });
    button.setPosition({ window.width - button.getSize().x, 0 });
    button.setFillColor(Color::White);

    Map map(dataPath + "GameElements/map.txt", dataPath + "Textures/cobblestone.png");
    pmap = &map;

    //Player player(static_cast<float>(window.width / 2.f), 0, 80, 80, 0.01);
    Player player;
    pplayer = &player;
    player.setSword(dataPath + "Textures/sword.png", "HUI", {80, 160}, 0.8, 1);

    vector<Monster> monsters;
    //monsters.emplace_back(static_cast<float>(window.width / 2.f), 0, 100, 100, 0.01, dataPath + "RoflTextures/kitagawa.png");
    monsters.emplace_back(static_cast<float>(window.width / 2.f), 0, 100, 100, 0.01, dataPath + "Textures/monster.png");

    while (Window.isOpen())
    {
        // Update dt
        dt = dtClock.restart().asMilliseconds();
        //cout << dt << endl;

        // Events
        while (const optional event = Window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                Window.close();
            else if (event->is<sf::Event::KeyPressed>())
            {
                const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent->code == sf::Keyboard::Key::Enter)
                {
                    switchWindowMode(Window);
                }
                else if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    Window.close();
                }
            }
        }

        // Update

        // Update game elements
        player.update(dt);
        camera.update(&map);

        for (auto& m : monsters) m.update(dt);
        // delete dead monsters
        monsters.erase(
            remove_if(
                monsters.begin(), monsters.end(), 
                [](const Monster& m) { return !m.getIsAlive(); }
                ),
            monsters.end()
            );
        if (size(monsters) == 0) {
            monsters.emplace_back(static_cast<float>(window.width / 2.f), 0, 200, 200, 0.01, dataPath + "RoflTextures/kitagawa.png");
        }

        // Render
        Window.clear(Color::White);

        // Render game elements
        Window.setView(camera.view);

        map.drawTheMap();
        player.render();

        for (auto& m : monsters) m.render();

        // Render UI
        Window.setView(Window.getDefaultView());

        player.renderUI();

        // Done drawing
        Window.display();
    }
    return 0;
}