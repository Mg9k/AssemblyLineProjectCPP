// This module represents a class that contains multiple stations. (see Station module)
// Orders are queued within a workstation class, and are processed through each station within the class.
// Ex. Wardrobe station -> Bookshelf station -> Bed station -> ... 

#ifndef SDDS_WORKSTATION_H
#define SDDS_WORKSTATION_H
#include <deque>
#include <string>
#include <iostream>
#include "Station.h"
#include "CustomerOrder.h"

namespace sdds {

	extern std::deque<CustomerOrder> g_pending;
	extern std::deque<CustomerOrder> g_completed;
	extern std::deque<CustomerOrder> g_incomplete;

	class Workstation : public Station {
		std::deque<CustomerOrder> m_orders{};
		Workstation* m_pNextStation{};
	public:
		Workstation(const std::string& str);
		Workstation(const Workstation& other) = delete;
		Workstation& operator=(const Workstation& other) = delete;
		Workstation(Workstation&& other) = delete;
		Workstation& operator=(Workstation&& other) = delete;
		void fill(std::ostream& os);
		bool attemptToMoveOrder();
		void setNextStation(Workstation* station = nullptr);
		Workstation* getNextStation() const;
		void display(std::ostream& os) const;
		Workstation& operator+=(CustomerOrder&& newOrder);
	};
}

#endif 
