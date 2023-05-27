#include<iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <unordered_set>
#include "Workstation.h"
#include "LineManager.h"
#include "Utilities.h"

namespace sdds {
   sdds::LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
      try {         
         std::string line{};
         Utilities util; // use to extract tokens
         // The file contains the linkage between workstation pairs.
         // The format of each record in the file is WORKSTATION | NEXT_WORKSTATION.The records themselves are not in any particular order.
         std::ifstream input(file);

         if (!input.is_open()) {
            throw std::invalid_argument("File cannot be opened.");
         }

         auto findStationByName = [&](const std::string& name) {
            return std::find_if(stations.begin(), stations.end(), [&](Workstation* curr) { return curr->getItemName() == name; });
         };
         
         std::unordered_set<std::string> nextStationItemName;
         while (std::getline(input, line)) {     
            bool more = true;
            size_t pos = 0;
            auto itActiveLine = findStationByName( util.extractToken(line, pos, more) ); // Find active line by name

            if (itActiveLine != stations.end()) {  // Add active line to m_activeLine if found
               m_activeLine.push_back(*itActiveLine);               
               if (more) {  // indicates if there is NEXT_WORKSTATION in the file record.
                  auto iNextStation = findStationByName(util.extractToken(line, pos, more));
                  m_activeLine.back()->setNextStation(*iNextStation);

                  // Inserts the name of the next station in the active line into an unordered set to keep track of which stations are
                  // already included in the assembly line. This will be used to compare and determine the first station
                  nextStationItemName.insert(m_activeLine.back()->getNextStation()->getItemName());
               }                            
            }
         }
         m_cntCustomerOrder = g_pending.size(); // Set the number of customer orders in the g_pending queue

         // Finds the first station in the assembly line by searching through m_activeLine and identifying the station
         // whose item name is not found in the nextStationItemName set. The resulting Workstation pointer is stored in
         // the m_firstStation attribute.
         m_firstStation = *std::find_if(m_activeLine.begin(), m_activeLine.end(), [&](Workstation* aLine) {
            return nextStationItemName.find(aLine->getItemName()) == nextStationItemName.end();
          });
      }
      catch (const std::exception& err) {
         std::cerr << "An exception occurred in LineManager: " << err.what() << std::endl;
      }
      catch (...) {
         std::cerr << "An unknown exception occurred in LineManager." << std::endl;
      }
   }

   void LineManager::reorderStations() {
      // The elements in the reordered collection start with the first station, proceeds to the next, and so forth until the end of the line.
      for (auto& sort : m_activeLine) {
         sort = m_firstStation;
         m_firstStation = m_firstStation->getNextStation();
      }
      m_firstStation = m_activeLine.front();
   }

   bool LineManager::run(std::ostream& os) { // return true if all customer orders have been filled or cannot be filled, otherwise returns false.
      static size_t count{ 1 }; // keeps track of the current iteration number (how many times this function has been called by the client)
      auto begin = m_activeLine.begin();
      auto end = m_activeLine.end();

      os << "Line Manager Iteration: " << count++ << std::endl;

      if (!g_pending.empty()) {
         // moves the order at the front of the g_pending queue to the m_firstStation and remove it from the queue
         (*m_firstStation) += std::move(g_pending.front());
         g_pending.pop_front();
      }
      std::for_each(begin, end, [&os](Workstation* ws) { ws->fill(os); }); // each station on the line, executes one fill operation
      std::for_each(begin, end, [](Workstation* ws) {	ws->attemptToMoveOrder(); }); // each station on the line, attempts to move an order down the line
      return m_cntCustomerOrder == (g_completed.size() + g_incomplete.size());
   }

   void LineManager::display(std::ostream& os) const {
      for (auto line : m_activeLine) {
         line->display(os);
      }
   }
}