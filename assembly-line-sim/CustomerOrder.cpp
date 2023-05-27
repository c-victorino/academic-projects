#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "Station.h"
#include "Utilities.h"
#include "CustomerOrder.h"

namespace sdds {
   size_t CustomerOrder::m_widthField = 0;

   void CustomerOrder::operator~() {
      std::for_each(m_lstItem, m_lstItem + m_cntItem, [](Item* item) { delete item; });
      delete[] m_lstItem;
      m_lstItem = nullptr;
   }

   CustomerOrder::CustomerOrder(const std::string& str) {
      Utilities tool; // use to extract tokens from the string
      size_t pos{};
      size_t delimiterCount;
      bool more = true;

      // Extract the customer name and order name from the string
      m_name = tool.extractToken(str, pos, more);
      m_product = tool.extractToken(str, pos, more);

      // If there are more tokens, extract the list of items making up the order
      if (more) {
         // Count the number of delimiters in the string (excluding customer name and order name delimiters)
         delimiterCount = std::count(str.begin(), str.end(), tool.getDelimiter());
         // Subtract 1 from the delimiter count to exclude the delimiter separating the customer name and order name.
         delimiterCount -= 1; 
         // Set the count of items in the order
         m_cntItem = delimiterCount;
         // Create a dynamic array to store pointers to the items in the order
         m_lstItem = new Item * [m_cntItem];
         // Extract each item from the string and create a new Item object for each one
         for (size_t i = 0; i < m_cntItem; i++) {
            m_lstItem[i] = new Item(tool.extractToken(str, pos, more));
         }
      }
      m_widthField = std::max(m_widthField, tool.getFieldWidth());
   }

   CustomerOrder::CustomerOrder(const CustomerOrder& cpy) {
      throw std::logic_error("Copy constructor of CustomerOrder called. Copying not allowed.");
   }

   CustomerOrder::CustomerOrder(CustomerOrder&& src) noexcept { *this = std::move(src); }

   CustomerOrder& CustomerOrder::operator=(CustomerOrder&& src) noexcept {
      if (this != &src) {
         ~*this; // free old data
         m_name = src.m_name;
         m_product = src.m_product;
         m_cntItem = src.m_cntItem;
         m_lstItem = src.m_lstItem;

         src.m_name.clear();
         src.m_product.clear();
         src.m_lstItem = nullptr;
         src.m_cntItem = {};
      }
      return *this;
   }

   CustomerOrder::~CustomerOrder() { ~*this; }

   bool CustomerOrder::isOrderFilled() const {
      return std::all_of(m_lstItem, m_lstItem + m_cntItem, [](Item* item) { return item->m_isFilled; });
   }

   bool CustomerOrder::isItemFilled(const std::string& itemName) const { 
      bool filled = true;
      for (size_t i = 0; i < m_cntItem; i++) {
         if (m_lstItem[i]->m_itemName == itemName && !m_lstItem[i]->m_isFilled) {
            filled = false;
         }
      }
      return filled;
   }

   void CustomerOrder::fillItem(Station& station, std::ostream& os) {
      bool filled = false; // 'filled' will be use to stop loop at any point when 1 item is filled
      for (size_t i = 0; i < m_cntItem && !filled; i++) {
         if (station.getItemName() == m_lstItem[i]->m_itemName && !m_lstItem[i]->m_isFilled) {
            if (station.getQuantity() || !(os << "    Unable to fill ")) { // if inventory is empty it will print the message
               station.updateQuantity(); // this will subtracts 1 from the inventory
               m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
               filled = m_lstItem[i]->m_isFilled = true;
               os << "    Filled ";
            }
            os << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
         }
      }
   }

   void CustomerOrder::display(std::ostream& os) const {
      os << m_name << " - " << m_product << '\n';
      for (size_t i = 0; i < m_cntItem; i++) {
         os << "[" << std::setw(6) << std::setfill('0') << std::right << m_lstItem[i]->m_serialNumber << "] "
            << std::setw(m_widthField) << std::setfill(' ') << std::left << m_lstItem[i]->m_itemName << "   - "
            << (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED") << std::endl;
      }
   }
}