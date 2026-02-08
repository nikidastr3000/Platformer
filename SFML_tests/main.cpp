#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

void coutNode(const YAML::Node& node){
    cout << node["frameSize"]["width"].as<int>() << endl;
}

void loadData(){
    YAML::Node animations = YAML::LoadFile("../data/Configs/animations.yaml")["player"];

    coutNode(animations["ATTACKING"]);
}

int main() {

    // IntRect rect;
    // rect = config["player"]["IDLE"]["frames"]["right"][0];
    
    loadData();

    

    

}