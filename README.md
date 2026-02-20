# train-linked-list-simulator
Circular doubly linked list train simulator in C with command queue execution and dynamic memory management.

Overview
  - This project is a train simulation system implemented in C.
  - The train is modeled as a circular doubly linked list with a sentinel node (the locomotive), and a mechanic pointer that moves across wagons.
  - Commands are not executed immediately — they are stored in a queue and processed only when the EXECUTE command is received. The queue can also be reversed using SWITCH.
  - This project was built to practice linked lists, queues, pointer manipulation, and dynamic memory management.

Data Structures Used
  - Circular doubly linked list (train)
  - Sentinel node (locomotive)
  - Command queue (doubly linked list)
  - Dynamic memory allocation

Supported Operations
  - MOVE_LEFT / MOVE_RIGHT
  - INSERT_L / INSERT_R
  - WRITE
  - CLEAR_CELL / CLEAR_ALL
  - SEARCH / SEARCH_LEFT / SEARCH_RIGHT
  - SHOW / SHOW_CURRENT
  - EXECUTE
  - SWITCH

Memory Management
  - All memory is dynamically allocated and properly freed.
  - The project was tested with Valgrind to ensure there are no memory leaks.
