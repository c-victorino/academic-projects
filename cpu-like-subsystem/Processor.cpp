#include <iostream>
#include "Processor.h"

namespace sdds {
   Processor::Processor(CentralUnit<Processor>* host, std::string brand, std::string code, unsigned int power) : 
      m_host{ host }, 
      m_brand{ brand }, 
      m_code{ code }, 
      m_power{power} 
   { }

   Processor::operator bool() const { return m_current != nullptr; }

   Processor& Processor::operator+=(Job*& job) {
      if (static_cast<bool>(*this)) { // checks if has job assigned
         throw std::logic_error("Processor already has a job assigned to it.");
      }
      else {         
         m_current = job;
      }
      return *this;
   }

   Job* Processor::get_current_job() const { return m_current; }

   void Processor::on_complete(void(*jobFinish)(CentralUnit<Processor>&, Processor*)) { m_jobFinish = jobFinish; }

   void Processor::on_error(std::function<void(Processor*)> err) { m_error = err; }

   void Processor::operator()() {
      if (static_cast<bool>(*this)) { // checks if has job assigned
         try {
            if (!get_current_job()->is_complete()) { // If job assigned to processor was already completed, do nothing.
               if ((*m_current)(m_power).is_complete()) { // uses operator() to run job then check if finish
                  m_jobFinish(*m_host, this); // calls registered callback function if job done after "run"
               }
            }
         }
         catch (std::underflow_error const&) { m_error(this); }
      }
   }

   Job* Processor::free() {
      // do not clear memory, simply unassigns current job and return the unassigned job.
      Job* temp = m_current;
      m_current = nullptr;
      return temp;
   }

   std::ostream& operator<<(std::ostream& os, const Processor& p) {
      // output is in the following format:
      // (POWER) BRAND CODE processing CURRENT_JOB_DETAILS
      os << "(" << p.m_power << ") " << p.m_brand << ' ' << p.m_code;
      if (static_cast<bool>(p) && p.m_current->is_active()) { // print if has job
         os << " processing ";
         p.m_current->display(os);
      }
      return os;
   }
}