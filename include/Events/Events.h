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

	class EventBus {
	public:

		EventBus();
		~EventBus();

		void addEvent(Event* e);
		void clearEvents();

	private:
		std::vector<Event*> eventList;
		
		
	};
}