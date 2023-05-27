#ifndef SDDS_CUSTOMER_ORDER_H
#define SDDS_CUSTOMER_ORDER_H
#include <iostream>
#include <string>
#include "Station.h"

namespace sdds {
   class CustomerOrder {
   private:
      struct Item {
         std::string m_itemName;
         size_t m_serialNumber{ 0 };
         bool m_isFilled{ false };
         Item(const std::string& src) : m_itemName(src) {};
      };

      std::string m_name{}; // the name of the customer 
      std::string m_product{}; // the name of the product being assembled
      size_t m_cntItem{}; // a count of the number of items in the customer's order

      // Dynamically allocated array of pointers. Each element of the array points to 
      // a dynamically allocated object of type Item. This is a resource that the class will manage.
      Item** m_lstItem{}; 

      static size_t m_widthField; // the maximum width of a field, used for display purposes
      void operator~(); // deallocates the memory occupied by the m_lstItem.
   public:
      CustomerOrder() = default;
      // Constructor uses a local Utilities obj to extract tokens
      // from the string and populate the current instance
      CustomerOrder(const std::string& str);

      CustomerOrder(const CustomerOrder& cpy);
      CustomerOrder& operator=(const CustomerOrder& cpy) = delete; // disable copy assignment operator

      CustomerOrder(CustomerOrder&& src) noexcept;
      CustomerOrder& operator=(CustomerOrder&& src) noexcept;
      
      ~CustomerOrder();

      bool isOrderFilled() const; //  returns true if all the items in the order have been filled; false otherwise
      bool isItemFilled(const std::string& itemName) const; // returns true if all items specified by itemName have been filled. If the item doesn't exist in the order, this query returns true.
      void fillItem(Station& station, std::ostream& os); // modifier fills one item in the current order that the Station specified in the first parameter handles.
      void display(std::ostream& os) const; // displays the state of the current object
   };
}
#endif // !SDDS_CUSTOMER_ORDER_H