#ifndef SDDS_JOB_H_
#define SDDS_JOB_H_
#include <string>

namespace sdds {
   class Job {
   private:
      std::string m_jobName{}; // describing the job to be done.
      size_t m_workUnits{}; // units of work to be done to finish job.
      size_t m_remWorkUnits{}; // remaining units of work to be done to finish job.
      bool m_isJobActive{};
   public:
      Job() = default;
      Job(const std::string& jobName);
      bool is_active() const;
      bool is_complete() const;
      std::string name() const;
      std::ostream& display(std::ostream& os) const;
      Job& operator()(const unsigned&); //  perform work on the job to progress towards completion
   };
   std::ostream& operator<<(std::ostream&, Job&);
}
#endif // !SDDS_JOB_H_