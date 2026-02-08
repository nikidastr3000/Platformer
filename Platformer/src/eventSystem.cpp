#include "eventSystem.h"
#include <cassert>

////////////IEventHandler///////////////// 

// Mutator and selector 
IEventHandler* IEventHandler::GetNextHandler(void) { return _nextHandler; }
void  IEventHandler::SetNextHandler(IEventHandler* next) { _nextHandler = next; }

IEventHandler::IEventHandler() : _nextHandler(0)
{
    EventDispatcher::Get().RegisterHandler(this);
}

//protected:
void  IEventHandler::SendEvent(::Event e)
{
    EventDispatcher::Get().SendEvent(e);
}

////////////EventDispatcher///////////////// 

//public:
EventDispatcher& EventDispatcher::Get() {
    static EventDispatcher instance;
    return instance;
}

EventDispatcher::EventDispatcher()
    : _deviceList(nullptr) // Инициализация указателя _deviceList
{
    // Конструктор оставлен пустым
}

void EventDispatcher::RegisterHandler(IEventHandler* device)
{
    device->SetNextHandler(_deviceList);
    _deviceList = device;
}

// Sends the event to all the devices registered to listen
void EventDispatcher::SendEvent(::Event e)
{
    IEventHandler* curDevice = _deviceList;

    for (; curDevice; curDevice = curDevice->GetNextHandler())
    {
        assert(curDevice != curDevice->GetNextHandler());
        curDevice->EventHandler(e);
    }
}
