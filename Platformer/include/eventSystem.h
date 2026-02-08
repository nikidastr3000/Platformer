#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

enum class EventType {
    // default value
    NONE,

    //
    COLLISION
};

struct Event {
    EventType eventType{};


    // CollisionEvent Attribute
    Vector2f direction{};
};

class IEventHandler
{
public:
    virtual void EventHandler(const ::Event& e) = 0;

    // Mutator and selector 
    IEventHandler* GetNextHandler(void);
    void SetNextHandler(IEventHandler* next);

    IEventHandler();

protected:
    void SendEvent(::Event e);

private:
    IEventHandler* _nextHandler;
};


class EventDispatcher
{
public:
    static EventDispatcher& Get();

    void RegisterHandler(IEventHandler* device);

    // Sends the event to all the devices registered to listen 
    void SendEvent(::Event e);

private:
    IEventHandler* _deviceList;

    EventDispatcher();

    // delete copying and moving
    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;
};
