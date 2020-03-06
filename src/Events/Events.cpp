#include <Events/Events.h>

namespace Stardust::Events {
	EventBus::EventBus()
	{
		eventList.clear();
		eventListeners.clear();
	}
	EventBus::~EventBus()
	{
		clearEvents();
	}
	void EventBus::addEvent(Event* e)
	{
		eventList.push_back(e);
	}
	void EventBus::clearEvents()
	{
		for (int i = 0; i < eventList.size(); i++) {
			delete eventList.at(i);
		}
		eventList.clear();
	}
	void EventBus::addEventHandler(int e_type, EventHandler h)
	{
		eventListeners.emplace(e_type, h);
	}
	void EventBus::deleteListener(int e)
	{
		eventListeners.erase(eventListeners.find(e));
	}
	void EventBus::clearEventHandlers()
	{
		eventListeners.clear();
	}
	void EventBus::propagate()
	{
		for (int i = 0; i < eventList.size(); i++) {
			if (eventListeners.find(eventList[i]->event_type) != eventListeners.end()) {
				eventListeners[eventList[i]->event_type](eventList[i]);
			}
		}

		eventList.clear();
	}
	EventBus g_EventBus;
}