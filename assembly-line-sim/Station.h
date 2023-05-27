#ifndef SDDS_STATION_H_
#define SDDS_STATION_H_
#include <string>

namespace sdds {
   class Station {
   private:
      int m_stationID{};
      std::string m_itemHandled{}; // name of the item handled by the station 
      std::string stationDescription{}; 
      size_t m_nextSerialNumber{}; // the next serial number to be assigned to an item at this station
      size_t m_itemsInStock{}; // the number of items currently in stock

      static size_t m_widthField; // initial value 0, represents the maximum characters needed to print an item name for any Station object.
      static size_t id_generator; // used to generate Station IDs. New instances store curr id_generator val & is incremented. Init value: 0.
   public:
      Station() = default;
      Station(const std::string& str);
      const std::string& getItemName() const; // returns name of the current Station object
      size_t getNextSerialNumber(); // returns the next serial number to be used on the assembly line
      size_t getQuantity() const; // returns the remaining quantity of items in the Station object
      void updateQuantity(); // subtracts 1 from the available quantity; should not drop below 0.
      void display(std::ostream& os, bool full) const; // inserts information about the current object into stream
   };
}
#endif // !SDDS_STATION_H_