#include <iostream>
#include <deque>
#include <string>
#include "CustomerOrder.h"
#include "Workstation.h"

namespace sdds {
   std::deque<CustomerOrder> g_pending{};
   std::deque<CustomerOrder> g_completed{};
   std::deque<CustomerOrder> g_incomplete{};

   Workstation::Workstation(const std::string station) : Station(station) { }

   void Workstation::fill(std::ostream& os) {
      if (!m_orders.empty()) {
         m_orders.front().fillItem(*this, os);
      }
   }

   bool Workstation::attemptToMoveOrder() {
      bool isMoved = false;
      // if the order requires no more service at this station or cannot be filled (not enough inventory), 
      // move it to the next station; otherwise it will do nothing
      if (!m_orders.empty()) {
         if ( m_orders.front().isItemFilled(getItemName()) || !getQuantity() ) {
            // if there is no next station in the assembly line, then the order is moved into g_completed or g_incomplete queue
            if (getNextStation() != nullptr) { 
               *getNextStation() += std::move(m_orders.front());
            }
            else {
               (!m_orders.front().isOrderFilled() ? g_incomplete : g_completed).push_back(std::move(m_orders.front()));
            }
            m_orders.pop_front(); // remove the moved order from the deque
            isMoved = true;
         }
      }
      return isMoved;
   }

   void Workstation::setNextStation(Workstation* station) { m_pNextStation = station; }

   Workstation* Workstation::getNextStation() const { return m_pNextStation; }

   void Workstation::display(std::ostream& os) const {
      os << getItemName() << " --> "
         << (!m_pNextStation ? "End of Line" : m_pNextStation->getItemName()) << '\n';
   }

   Workstation& Workstation::operator+=(CustomerOrder&& newOrder) {
      m_orders.push_back(std::move(newOrder));
      return *this;  
   }
}