#include <iostream>
#include <iomanip>
#include "Job.h"

namespace sdds {

   Job::Job(const std::string& jobName) : m_jobName{ jobName } {
      // For simplicity, number of units of work to be done to finish the job 
      // is equal to the remainder from dividing the number of characters in the title by 10 and adding 1.
      m_remWorkUnits = m_workUnits = (jobName.length() % 10) + 1;
      m_isJobActive = false;
   }

   bool Job::is_active() const { return m_isJobActive; }

   bool Job::is_complete() const { return !m_remWorkUnits; }

   std::string Job::name() const { return m_jobName; }

   std::ostream& Job::display(std::ostream& ostr) const {
      // `JOB NAME` [XX/XX remaining]
      return ostr << "`" << name() << "` "
         << "[" << std::setw(2) << std::setfill('0') << m_remWorkUnits
         << "/" << std::setw(2) << std::setfill('0') << m_workUnits << " remaining]";
   }

   Job& Job::operator()(const unsigned& wrkHandled) {
      if (wrkHandled > m_remWorkUnits) {
         m_remWorkUnits = 0;
         m_isJobActive = false;
         throw std::underflow_error(" Handled work is more than the remaining work");
      }
      else {
         // no remaining work, the job is marked as inactive. 
         m_isJobActive = (m_remWorkUnits -= wrkHandled);
      }      
      return *this;
   }

   std::ostream& operator<<(std::ostream& ostr, Job& job) { return job.display(ostr); }
}