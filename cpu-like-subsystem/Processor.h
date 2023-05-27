#ifndef SDDS_PROCESSOR_H_
#define SDDS_PROCESSOR_H_
#include <string>
#include <functional>
#include "CentralUnit.h"

namespace sdds {
   class Processor {
   private:
      CentralUnit<Processor>* m_host{}; // stores address of the hosting central unit that specifically hosts processors.
      std::string m_brand{}; // brand of microprocessor, representing the unit type.
      std::string m_code{}; // unit name
      unsigned int m_power{}; // work units that the microprocessor can process for a job in a single run
      Job* m_current{}; // job currently assigned to the microprocessor to be processed.
      void (*m_jobFinish)(CentralUnit<Processor>&, Processor*) {}; // stores address of a function that will be run when a job finishes processing
      std::function<void(Processor*)> m_error{}; // targets a function to be run when an error occurs.
   public:
      Processor() = default;
      Processor(CentralUnit<Processor>* host, std::string brand, std::string code, unsigned int power);
      explicit operator bool() const; // representation of processor, returns true if job is assigned
      Processor& operator+=(Job*&); // assigns a job to the processor.
      Job* get_current_job() const;
      void on_complete(void (*jobFinish)(CentralUnit<Processor>&, Processor*)); // takes address of a callback function to register
      void on_error(std::function<void(Processor*)> err); // takes, as an argument, an std::function callback to register
      void operator()(); // runs the processor once
      Job* free(); //  makes the processor available by unassigns the current job 
      friend std::ostream& operator<<(std::ostream&, const Processor&); //  inserts the details about this processor into the output stream
   };  
}
#endif // !SDDS_PROCESSOR_H_
