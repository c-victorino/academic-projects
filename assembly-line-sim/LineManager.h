#ifndef SDDS_LINE_MANAGER_H
#define SDDS_LINE_MANAGER_H
#include<iostream>
#include <vector>
#include <string>
#include "Workstation.h"

namespace sdds {
   class LineManager {
   private:
      std::vector<Workstation*> m_activeLine{}; // collection of workstations for the current assembly line.
      size_t m_cntCustomerOrder{}; // total number of CustomerOrder objects
      Workstation* m_firstStation{}; // points to the first active station on the current line
   public:
      LineManager() = default;
      // constructor receives the name of the file that identifies the active stations on the assembly line (example: AssemblyLine.txt)
      // and the collection of workstations available for configuring the assembly line
      LineManager(const std::string& file, const std::vector<Workstation*>& stations); 
      void reorderStations(); // reorders the workstations present in the instance variable m_activeLine
      bool run(std::ostream& os); //  performs one iteration of operations on all of the workstations in the current assembly 
      void display(std::ostream& os) const; // query displays all active stations on the assembly line in their current order
   };
}
#endif // !SDDS_LINE_MANAGER_H