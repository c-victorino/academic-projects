#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "Station.h"
#include "Utilities.h"

namespace sdds {
   size_t Station::m_widthField{ 0 };
   size_t Station::id_generator{ 0 };

   Station::Station(const std::string& str) {
      bool more = true;
      size_t pos = 0;
      Utilities utilities; // Utilities object to extract tokens

      // Increment id_generator and set the current instance's station ID
      id_generator++; 
      m_stationID = id_generator;

      try {
         // Extract item name, starting serial number, stock quantity, & description from the input string
         m_itemHandled = utilities.extractToken(str, pos, more);
         m_nextSerialNumber = std::stoi(utilities.extractToken(str, pos, more));
         m_itemsInStock = std::stoi(utilities.extractToken(str, pos, more));
         m_widthField = std::max(utilities.getFieldWidth(), m_widthField); // Update Station::m_widthField to the maximum value
         stationDescription = utilities.extractToken(str, pos, more);     
      }
      catch(const std::invalid_argument& e) {
         std::cerr << e.what() << std::endl;
         std::cerr << "Could not construct Station object from the following record:" << std::endl;
         std::cerr << str << std::endl;
      }
   }

   const std::string& Station::getItemName() const { return m_itemHandled; }

   size_t Station::getNextSerialNumber() { return m_nextSerialNumber++; }

   size_t Station::getQuantity() const { return m_itemsInStock; }

   void Station::updateQuantity() {
      // subtracts 1 from the available quantity; should not drop below 0
      if (m_itemsInStock != 0) {
         m_itemsInStock--;
      }
   }
   void Station::display(std::ostream& os, bool full) const {
      os << std::setw(3) << std::setfill('0') << std::right << m_stationID << " | " 
         << std::setw(m_widthField) << std::setfill(' ') << std::left << m_itemHandled << "  | "
         << std::setw(6) << std::setfill('0') << std::right << m_nextSerialNumber << " | ";

      // If the 'full' parameter is true, insert the item quantity and description into the output stream
      if (full) {
         os << std::setw(4) << std::setfill(' ') << std::right << m_itemsInStock << " | " 
            << stationDescription;
      }
      os << std::endl;   
   }
}