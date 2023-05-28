# CPU Like Subsystem

CPU Like Subsystem is a C++ project that simulates a central processing unit environment for job processing. It includes modules such as Job, Processor, and CentralUnit. The project demonstrates the implementation of dynamic memory allocation, function objects, and exception handling. It applies modern C++ practices, object-oriented programming principles, and utilizes data structures for efficient code execution. Additionally, the project leverages the observer pattern to notify the central unit of job completion or errors, showcasing its advanced functionality. The incorporation of features like templated classes further enhances the project's flexibility and code reusability.

## Techniques and Skills Implemented

- Implement member functions that accept callback functions as arguments, allowing for dynamic behavior
- Use C++ exception handling mechanisms to detect and handle errors, including throwing and catching exceptions of different types.
- Implement the observer pattern to enable communication between different modules
- Utilize C++ standard library algorithms and containers to improve code efficiency and readability
- Use function overloading to provide different implementations for different input parameters
- Develop debugging skills to identify and fix errors in code

## Modules

- **Job:** Defines a class for managing job information, including the job title, number of units of work, remaining work, and active status. Provides member functions for checking job status, displaying job details, and updating remaining work.
- **Processor:** Implements a microprocessor as an individual unit that can process a job. It provides the ability to assign a job to the processor, run the job with the specified processing power, and check if the job is complete. The module is responsible for notifying the central unit of completion or errors using registered callbacks. It also allows for freeing the processor from a job assignment and provides a function object for processing a job.
- **CentralUnit:** responsible for coordinating and managing a collection of individual units of type T that work together to handle a specific type of job. It uses a custom constructor to read a file of formatted data and initialize the units, and provides member functions to add jobs to the queue, run the units, and check for job availability. Each individual unit hosted by this object must implement a run() method that performs work towards completing a job. Additionally, the module registers callbacks with each unit to handle job completion and errors. It is designed to be lightweight and does not support copy operations.

## Usage

To use the program, ensure that it is run with `processors.txt` passed as a command line argument. The file processors.txt contains the information about the processors.
