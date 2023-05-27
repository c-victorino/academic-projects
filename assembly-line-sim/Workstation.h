#ifndef SDDS_WORK_STATION_H
#define SDDS_WORK_STATION_H
#include <iostream>
#include <deque>
#include <string>
#include "CustomerOrder.h"

namespace sdds {
   extern std::deque<CustomerOrder> g_pending; // holds orders to be placed onto the assembly line at the first station
   extern std::deque<CustomerOrder> g_completed; // holds orders that have been removed from the last station and have been completely filled
   extern std::deque<CustomerOrder> g_incomplete; // holds the orders that have been removed from the last station and could not be filled completely.

   class Workstation : public Station {
   private:
      // CustomerOrders entering the back and exiting the front. 
      // These are orders that have been placed on this station to receive service (or already received service).
      std::deque<CustomerOrder> m_orders{};
      Workstation* m_pNextStation{}; // a pointer to the next Workstation on the assembly line.
   public:
      Workstation() = default;
      Workstation(const std::string station);

      Workstation(const Workstation& cpy) = delete;
      Workstation& operator=(const Workstation& cpy) = delete;

      Workstation(Workstation&& src) = delete;
      Workstation& operator=(Workstation&& src) = delete;

      void fill(std::ostream& os); // this modifier fills the order at the front of the queue if there are CustomerOrders in the queue; otherwise, does nothing
      bool attemptToMoveOrder(); // attempts to move the order order at the front of the queue to the next station in the assembly line
      void setNextStation(Workstation* station = nullptr); // the modifier stores the address of the referenced Workstation object in the pointer to the m_pNextStation
      Workstation* getNextStation() const; // this query returns the address of next Workstation
      void display(std::ostream& os) const;
      Workstation& operator+=(CustomerOrder&& newOrder); // moves the CustomerOrder referenced in parameter newOrder to the back of the queue.
   };
}
#endif // !SDDS_WORK_STATION_H