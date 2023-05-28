# Assembly Line Sim

Assembly Line Sim is a C++ program that simulates an assembly line system for processing customer orders. It incorporates modules such as Utilities, Station, CustomerOrder, and LineManager. The program utilizes parsing techniques, modern C++ practices, and object-oriented programming principles while implementing error handling and leveraging data structures for efficient code execution.

![Assembly Line](assemblyline.jpg)

## Features

- Utilities module supports parsing of input files to set up and configure the assembly line system's objects
- Parsing string data from input files into tokens is uniformly performed for all objects in the assembly line simulation system using the Utilities module
- Station module manages information about a station on the assembly line that holds a specific item and fills customer orders
- Each Station object manages a single station on the assembly line, handling a specific item for filling customer orders

## Techniques and Skills Implemented

- Utilized parsing techniques to extract data from input files containing delimiter-separated values, and converted them into tokens for further processing
- Applied modern C++ practices, including STL algorithms and move semantics, to write optimized and efficient code
- Implemented dynamic memory allocation of pointers to pointers (Item\*\*) to manage resources in a class
- Utilized fundamental object-oriented programming principles, including encapsulation, inheritance, and polymorphism
- Utilized C++ inheritance to effectively reuse code and demonstrate object-oriented programming proficiency
- Incorporated robust error handling techniques, such as try-catch blocks and standard exception classes, to ensure program reliability and stability
- Demonstrated proficiency in lambda expressions for concise and efficient code
- Leveraged data structures such as unordered sets

## Modules

- **Utilities:** Supports parsing of input files and provides basic functionality required for all objects in the system.
- **Station:** Manages info about a station on assembly line, which holds a specific item and fills customer orders.
- **CustomerOrder:** Contains all the functionality for processing customer orders as they move from Station to Station along the assembly line. Manages a single order on the assembly line.
- **Workstation:** Defines the structure of an active station on the assembly line and contains all the functionality for filling customer orders with station items. Each Workstation is a kind of Station and manages order processing for a single Item on the assembly line.
- **LineManager:** Manages the assembly line of active stations. Configures the Workstation objects identified by the user and moves orders along the line one step at a time.

## Usage

To use the program, ensure that it is run with `Stations1.txt Stations2.txt CustomerOrders.txt AssemblyLine.txt` passed as a command line argument.
