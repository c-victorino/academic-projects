#ifndef SDDS_CENTRAL_UNIT_H_
#define SDDS_CENTRAL_UNIT_H_
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Job.h"

namespace sdds {
   const int MAX_JOB_QUEUE = 4; // queued up at any given time.
   template <typename T>
   class CentralUnit {
   private:
      std::string m_type{}; // type of work this central unit will be coordinating
      T** m_items{}; // individual units of type "T", are units that will be hosted in CentralUnit obj.
      Job* m_jobs[MAX_JOB_QUEUE]{}; // maximum of 4 jobs can be queued up at any given time.
      size_t m_size{}; // number of individual units hosted by the central unit.
      size_t m_count{}; // number of jobs queued up and waiting to be handled

      // Removes leading and trailing whitespace characters from the given string
      std::string& cleanString(std::string& str) {
         str.erase(0, str.find_first_not_of(" \t\n\r"));
         str.erase(str.find_last_not_of(" \t\n\r") + 1);
         return str;
      }
   public:
      std::ostream& log = std::cout; // logging information

      CentralUnit() = default;

      CentralUnit(const std::string& wrkType, const std::string& fileName) {
         std::ifstream input(fileName);
         if (!input.is_open()) {
            throw std::invalid_argument("File cannot be opened.");
         }
         else {
            size_t uToken{};
            std::size_t count{};
            std::string line{};
            std::string unitType{};
            std::string unitName{};
            std::string workCapacity{};
            m_type = wrkType;

            // Count the number of lines in the input file
            while (std::getline(input, line)) { count++; }
            input.clear();
            input.seekg(0, std::ios::beg);

            // Allocate memory for the m_items array
            m_items = new T * [count];
            while (std::getline(input, line)) {
               std::stringstream ss(line);

               // UNIT_TYPE | UNIT_NAME | WORK_CAPACITY               
               std::getline(ss, unitType, '|');
               std::getline(ss, unitName, '|');
               std::getline(ss, workCapacity);

               // remove leading and trailing spaces
               unitType = cleanString(unitType);
               unitName = cleanString(unitName);
               workCapacity = cleanString(workCapacity);

               try { 
                  uToken = std::stoi(workCapacity);
               }
               catch (const std::invalid_argument& ) {
                  uToken = 1; // default to 1
               }

               // Create a new object of type T and add it to the m_items array
               m_items[m_size] = new T(this, unitType, unitName, uToken);

               // Set up callbacks for the new object
               m_items[m_size]->on_complete(complete_job);
               m_items[m_size]->on_error([&](T* unit) {
                  log << "Failed to complete job " << unit->get_current_job()->name() << '\n';
                  log << m_size << " units available.\n";
                  unit->free();
               });
               m_size++;
            }
         }
         input.close();
      };

      // design does not support copy operations 
      CentralUnit(const CentralUnit<T>&) {
         throw std::runtime_error("Creating a new instance from a copy is not allowed.");
      };
      CentralUnit<T>& operator=(const CentralUnit<T>&) = delete;

      CentralUnit(CentralUnit<T>&& src) { *this = std::move(src); };
      CentralUnit<T>& operator=(CentralUnit<T>&& right) {
         if (this != &right) {
            delete[] m_items;
            m_items = nullptr;

            m_items = right.m_items;
            m_count = right.m_count;
            m_size = right.m_size;
            for (size_t i = 0; i < m_count; i++) {
               m_jobs[i] = right.m_jobs[i];
            }
            right.m_items = nullptr;;
            right.m_count = {};
            right.m_size = {};
            right.m_jobs = nullptr;
         }
         return *this;
      };

      ~CentralUnit() {
         for (size_t i = 0; i < m_size; i++) {
            delete m_items[i];
         }
         delete[] m_items;
         m_items = nullptr;

         for (size_t i = 0; i < MAX_JOB_QUEUE; i++) {
            delete m_jobs[i];
            m_jobs[i] = nullptr;
         }
      };

      CentralUnit& operator+=(T* unit) {
         //  receives the address of a unit (takes ownership) and 
         //  adds it to the central unit (resize the data member as needed).
         if (unit) {
            T** temp = new T * [m_size + 1];

            for (size_t i = 0; i < m_size; i++) {
               temp[i] = m_items[i];
            }

            temp[m_size] = unit;
            temp[m_size]->on_complete(complete_job);
            temp[m_size]->on_error([&](T* unit) {
               log << "Failed to complete job " << unit->get_current_job()->name() << '\n';
               log << m_size << " units available." << '\n';
               unit->free();
            });

            delete[] m_items;
            m_items = nullptr;

            m_items = temp;
            m_size++;
         }
         else { throw std::invalid_argument("Invalid unit parameter"); }
         return *this;
      }

      CentralUnit& operator+=(const std::string& job) { //  takes the name of a job as an argument
         if (m_count < MAX_JOB_QUEUE) {
            for (size_t i = 0; i < MAX_JOB_QUEUE; i++) {
               // checks for "free spot" / completed job
               if (m_jobs[i] == nullptr || (m_jobs[i]->is_complete() && !m_jobs[i]->is_active())) {
                  delete m_jobs[i];
                  m_jobs[i] = nullptr;
                  //  creates a new Job object that is added to the job queue
                  m_jobs[i] = new Job(job);
                  m_count++;
                  i = MAX_JOB_QUEUE; // break out of loop
               }
            }
         }
         else {
            throw std::string("Error: job queue is full.");
         }
         return *this;
      }

      void run() { 
         // check if job needs to be worked on
         // if there are jobs queued up and a unit is available (has no job assigned), 
         // remove the last job from the queue and assign it to the unit before running the unit.
         for (size_t i = 0, j = 0; i < m_size && m_count; i++) {
            if (!m_jobs[m_count - 1]->is_active() && !m_jobs[m_count - 1]->is_complete()) {
               // last job to be done added is first one in.
               *m_items[i] += m_jobs[m_count - 1];
               m_count--;
            }
         }
         m_count = 0;
         // runs a cycle for each individual unit. Goes through every unit and runs it once. 
         for (size_t i = 0; i < m_size; i++) {
            (*m_items[i])();  // works on the job
            if (*m_items[i]) {
               m_count++; // count how many job still need to be finished
            }
         }
      }

      bool has_jobs() const { return m_count; }

      size_t get_available_units() const { //  returns the number of units without jobs assigned to them.
         int count = 0;
         for (size_t i = 0; i < m_size; i++) {
            if (static_cast<bool>(*m_items[i])) {  // checks how many jobs are assigned
               count++;
            }
         }
         return m_size - count;
        
      }

      static void complete_job(CentralUnit<T>& cu, T* p) {
         //  frees the unit received and clears memory allocated for the job that was 
         //  assigned to that unit. This function also logs the following below:
         cu.log << "[COMPLETE] " << *p->get_current_job() << " using ";
         cu.log << *p << '\n';
         p->free(); // makes the processor available
         cu.log << cu.get_available_units() << " units available.\n";
      }

      T* operator[](const std::string& title) {
         // If no unit is assigned a matching job, function reports an std::out_of_range error
         for (size_t i = 0; i < m_size; i++) {
            if (m_items[i]->get_current_job() != nullptr && m_items[i]->get_current_job()->name() == title) {
               return m_items[i];
            }
         }
         throw std::out_of_range("Job is not being handled by a unit.");
      }

      void display() const {
         // logs central unit’s current state
         log << "Central " << m_type << " Unit list\n";
         for (size_t i = 0; i < m_size; i++)
            // logs each individual unit on a separate line in the following format: 
            // [XXXX] UNIT_DETAILS
            // [0001] (2) AMD RYZEN5 - 2600
            log << '[' << std::setw(4) << std::setfill('0') << i + 1 << "] " << *m_items[i] << '\n';
      }
   };
}
#endif // !SDDS_CENTRAL_UNIT_H_