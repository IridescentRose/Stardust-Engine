#pragma once
#include <Utilities/UUID.h>
#include <vector>
#include <map>

/**
 * The events subsystem for external program usage.
 * This code is never touched internally by the engine - it's purely a userspace utility.
**/
namespace Stardust::Events {
	
	/**
	 * The basic Event type - is a base class and can be derived from.
	 * Has nothing to destruct.
	**/
	class Event {
	public:

		/**
		 * Basic constructor - initializes the Event's UUID and sets the basic type to 0.
		 * The type should be changed depending on what you derive your own events to be.
		**/
		Event() {
			id = Utilities::generateUUID();
			event_type = 0;
		}

		/**
		 * Pseudo random (string-based) UUID.
		**/
		Utilities::UUID id;

		/**
		 * The type of the event. This should be used by your handler to pointer cast to your derived event.
		**/
		unsigned short event_type;
	};

	/**
	 * Simple event handler is defined as a void function with Event pointer input - this will be the event to handle.
	**/
	typedef void (*EventHandler)(Event*);

	/**
	 * The Event Bus is a class which collects events and pushes them to their respective handlers.
	**/
	class EventBus {
	public:

		/**
		 * Default constructor.
		**/
		EventBus();

		/**
		 * Default destructor.
		**/
		~EventBus();

		/**
		 * Push an event onto our bus.
		 * This also takes ownership of the Event.
		 * Your code should not delete or modify the event after this point!
		 * 
		 * @param e - Event to be moved onto the bus
		**/
		void addEvent(Event* e);

		/**
		 * Clear all of the events in the bus. Not recommended unless there is a change of program state.
		**/
		void clearEvents();

		/**
		 * Adds a handler for a specific event type.
		 * 
		 * @param e - The event type ID to handle.
		 * @param h - A pointer to the handler function (should be a static class member or external function).
		**/
		void addEventHandler(int e, EventHandler h);

		/**
		 * Deletes a specific event handler.
		 * 
		 * @param e - The event type ID to remove a handler for.
		**/
		void deleteListener(int e);

		/**
		 * Removes all event handlers. Not recommended unless there is a change of program state.
		**/
		void clearEventHandlers();

		/**
		 * Propagate takes all events currently in the bus and pushes them to their respective handlers upon this command.
		 * Events are automatically freed after the handler is called - make sure you are not referencing the events after propagate!
		**/
		void propagate();

	private:
		std::vector<Event*> eventList;
		std::map<int, EventHandler> eventListeners;
		
		
	};

	/**
	 * Stardust provides a default global event bus for programs.
	**/
	extern EventBus g_EventBus;
}