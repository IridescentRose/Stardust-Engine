#pragma once
#include <Utilities/UUID.h>
#include <vector>
#include <map>

namespace Stardust::Events {
	
	class Event {
	public:
		Event() {
			id = Utilities::generateUUID();
			event_type = 0;
		}

		Utilities::UUID id;
		unsigned short event_type;
	};

	typedef int (*EventHandler)(Event*);

	class EventBus {
	public:

		EventBus();
		~EventBus();

		void addEvent(Event* e);
		void clearEvents();

		void addEventHandler(int e, EventHandler h);
		void deleteListener(int e);
		void clearEventHandlers();

		void propagate();

	private:
		std::vector<Event*> eventList;
		std::map<int, EventHandler> eventListeners;
		
		
	};

	extern EventBus g_EventBus;
}