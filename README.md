# PersonalUtils

All my personal utilities, majority of the code is written myself, feel free to use.  

[Algorithms](algorithms)
[Code Architectures](code%20architectures)  
[Data Structures](data%20structures)  
[Tools](tools)  

### Algorithms

File | Name | Description | C++ Standard | Dependcies | Namespace
--- | --- | --- | --- | --- | ---
[AStarPathfinder.hpp](algorithms/AStarPathfinder.hpp) | A Star Pathfinder | An A* path finder class can be used on a 2D std::vector | C++14 | [Vec2.hpp](data%20structures/Graphics/Vec2.hpp) | lio

### Code Architectures

File | Name | Description | C++ Standard | Dependcies | Namespace
--- | --- | --- | --- | --- | ---
[lecs.hpp](code%20architectures/lecs.hpp) | Lio's ECS | Single header ECS library, managers for components, entities, and systems, with an event system, also managed by an event Manager | C++14 | N/A | lecs
[lev.hpp](code%20architectures/lev.hpp) | Lio's Event System | Single header simple event system library | C++20 | N/A | lev
[LIC](code%20architectures/LIC) | Lio's IC | Single header data oriented and data driven library, centralized and managed Entity(ID)-Component relataionship (Can be used for ECS) | C++20 | N/A | lic
### Data Structures

File | Name | Description | C++ Standard | Dependcies | Namespace
--- | --- | --- | --- | --- | ---
[Matrix.hpp](data%20structures/Matrix.hpp) | Matrix | A half-finished matrix class | C++14 | N/A | lio
[Vec2.hpp](data%20structures/Graphics/Vec2.hpp) | Vector2 | 2D Vector class with many graphical functions | C++14 | N/A | lio
[LineSeg.hpp](data%20structures/Graphics/LineSeg.hpp) | Line Segment | 2D line segment class | C++14 | [Vec2.hpp](data%20structures/Graphics/Vec2.hpp) | lio
[String (C)](data%20structures/String%20(C)) | String for C | String struct for C with a lot of dedicated functions, many are from C++ std::string class | C99 | N/A | N/A

### Tools

File | Name | Description | C++ Standard | Dependcies | Namespace
--- | --- | --- | --- | --- | ---
[CSVReader](tools/CSVReader) | CSV Reader | CSV (Comma Separated Values) file reader and parser | C++11 | N/A | lio
[LConsoleScreen](tools/LConsoleScreen) | Lio's Console Screen | Display Ascii characters as 2D screen in cmd (Windows only) | C++11 / C99 | [windows.h](https://en.wikipedia.org/wiki/Windows.h) | lio
[PortableSystemFunctions.h](tools/PortableSystemFunctions.h) | Portable System Functions | Cross-platform system functions | C++17 / C17 | [windows.h](https://en.wikipedia.org/wiki/Windows.h) / [unistd.h](https://en.wikipedia.org/wiki/Unistd.h) | N/A
[mem_usage](tools/mem_usage) | Memory Usage | To track and print out heap memory informations in console | C++14 | N/A | lio
