#include <Events/Events.h>

namespace Stardust::Events {
	EventBus::EventBus()
	{
		eventList.clear();
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
}