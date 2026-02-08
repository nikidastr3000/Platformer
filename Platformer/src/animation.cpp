#include "animation.h"
//#include "Globals.h"

#include <iostream>
#include <yaml-cpp/yaml.h>

using namespace sf;
using namespace std;

Animation::Animation(){};

void Animation::init(Sprite* Sprite, Texture* SpriteSheet, const YAML::Node& data){
    sprite = Sprite;
    spriteSheet = SpriteSheet;
    loadData(data);

    currentFrame.size = frameSize;
}

bool Animation::update(string dir, float dt){
    if (ONE_SHOT){
        //cout <<
    }

    if (isAnimationEnded){
        return false;
    }


    if (direction != dir){
        counter = 0;
        direction = dir;
    }

    if (type == STATIC){
        sprite->setTextureRect(frames[direction][0]);
        return true;
    }

    
    counter += dt;
    if (counter >= frames[dir].size() * oneFrameDuration){
        counter = 0;
        if (type == ONE_SHOT){
            isAnimationEnded = true;
            return false;
        }
    }

    if (oneFrameDuration == 0){
        cout << "devision by zero: oneFrameDuration is 0" << endl;
        exit(EXIT_FAILURE);
    }
    currentFrame = frames[dir][static_cast<int>(counter / oneFrameDuration)];
    sprite->setTextureRect(currentFrame);
    return true;
}

bool Animation::restart(){
    counter = 0;

    if (isAnimationEnded == false){
        return false;
    }

    isAnimationEnded = false;
    return true;
}

int Animation::getLeftAnimationOffset(){ return leftAnimationOffset; } 

// Private
void Animation::loadData(const YAML::Node& data){
    // reading type of animation
    type = stringToAnimationType(data["type"].as<string>());
    if (type == AnimationType::UNKNOWN){
        cout << "Animation type is unknown" << endl;
    }

    // reading frameSize
    frameSize = {data["frameSize"]["width"].as<int>(), data["frameSize"]["height"].as<int>()};

    // reading oneFrameDuration
    if (data["oneFrameDuration"]){
        oneFrameDuration = data["oneFrameDuration"].as<float>();
    }
    else if( type != STATIC){
        cout << "Not STATIC animation has no oneFrameDuration" << endl;
        exit(EXIT_FAILURE);
    }

    // reading leftAnimationOffset
    if (data["leftAnimationOffset"]){
        leftAnimationOffset = data["leftAnimationOffset"].as<int>();
    }
    
    IntRect frame;
    vector<IntRect> directionFrames;        // frames of one direction

    for (const string dir : {"right", "left"}){
        directionFrames.clear();
        for (int i = 0; i < data["frames"][dir].size(); i++) {
            frame.position.x = data["frames"][dir][i]["x"].as<int>();
            frame.position.y = data["frames"][dir][i]["y"].as<int>();
            frame.size = frameSize;
            
            directionFrames.push_back(frame);
        }
        frames[dir] = directionFrames;    
    }

    //print frames
    // for (const auto& [dir, frames] : frames){
    //     cout << dir << ": ";
    //     for (const auto& frame : frames){
    //         cout << "(" << frame.size.x << ", " << frame.size.y << ") ";
    //     }
    //     cout << endl;
    // }
}

Animation::AnimationType Animation::stringToAnimationType(const std::string& str) {
    if (str == "LOOPED") return AnimationType::LOOPED;
    if (str == "STATIC") return AnimationType::STATIC;
    if (str == "ONE_SHOT") return AnimationType::ONE_SHOT;
    return AnimationType::UNKNOWN;
}

