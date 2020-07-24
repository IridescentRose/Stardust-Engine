#include <Events/Events.h>
#include <Utilities/Logger.h>

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
		Utilities::detail::core_Logger->log("Added event handler for ID: " + std::to_string(e_type));
		eventListeners.emplace(e_type, h);
	}
	void EventBus::deleteListener(int e)
	{
		Utilities::detail::core_Logger->log("Removed event handler for ID: " + std::to_string(e));
		eventListeners.erase(eventListeners.find(e));
	}
	void EventBus::clearEventHandlers()
	{
		Utilities::detail::core_Logger->log("Cleared All Event Handlers");
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