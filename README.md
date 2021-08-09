# PersonalUtils

All my personal utilities, majority of the code is written myself, feel free to use.  
The column 'Language Standard/Version' just means the standard/version the code was developed and tested in.

[Algorithms](algorithms)
[Code Architectures](code%20architectures)  
[Data Structures](data%20structures)  
[Tools](tools)  

### Algorithms

File | Name | Description | Language Standard/Version | Dependcies | Namespace/Class
--- | --- | --- | --- | --- | ---
[AStarPathfinder.hpp](algorithms/AStarPathfinder.hpp) | A Star Pathfinder | An A* path finder class can be used on a 2D std::vector | C++14 | [Vec2.hpp](data%20structures/Graphics/Vec2.hpp) | lio
[RandomMoveCubeScramble.cpp](algorithms/RandomMoveCubeScramble.cpp) | Random Move Cube Scramble Algorithm | Generate a Rubik's Cube scramble with a given number of sides | C++14 | N/A | N/A

### Code Architectures

File | Name | Description | Language Standard/Version | Dependcies | Namespace/Class
--- | --- | --- | --- | --- | ---
[lecs.hpp](code%20architectures/lecs.hpp) | Lio's ECS | Single header ECS library, managers for components, entities, and systems, with an event system, also managed by an event Manager | C++14 | N/A | lecs
[lev.hpp](code%20architectures/lev.hpp) | Lio's Event System | Single header simple event system library | C++20 | N/A | lev
[LIC](code%20architectures/LIC) | Lio's IC | Single header data oriented and data driven library, centralized and managed Entity(ID)-Component relataionship (Can be used for ECS) | C++20 | N/A | lic
### Data Structures

File | Name | Description | Language Standard/Version | Dependcies | Namespace/Class
--- | --- | --- | --- | --- | ---
[Matrix.hpp](data%20structures/Matrix.hpp) | Matrix | A half-finished matrix class | C++14 | N/A | lio
[Vec2.hpp](data%20structures/Graphics/Vec2.hpp) | Vector2 | 2D Vector class with many graphical functions | C++14 | N/A | lio
[LineSeg.hpp](data%20structures/Graphics/LineSeg.hpp) | Line Segment | 2D line segment class | C++14 | [Vec2.hpp](data%20structures/Graphics/Vec2.hpp) | lio
[Data Structures for C](data%20structures/Data%20Structures%20for%20C) | Template Data Structures for C | Template structs for C with a lot of functions from C++ STL classes | C99 | N/A | N/A

### Tools

File | Name | Description | Language Standard/Version | Dependcies | Namespace/Class
--- | --- | --- | --- | --- | ---
[ANSIEscaper](tools/ANSIEscaper) | ANSI Escaper | A group of cross-platform ANSI escape code wrapper functions | C++14 / C99 | [windows.h](https://en.wikipedia.org/wiki/Windows.h) (for windows)
[CSVReader](tools/CSVReader) | CSV Reader | CSV (Comma Separated Values) file reader and parser | C++11 | N/A | lio
[LConsoleScreen](tools/LConsoleScreen) | Lio's Console Screen | Display Ascii characters as 2D screen in cmd (Windows only) | C++11 / C99 | [windows.h](https://en.wikipedia.org/wiki/Windows.h) | lio
[PortableSystemFunctions.h](tools/PortableSystemFunctions.h) | Portable System Functions | Cross-platform system functions | C++17 / C17 | [windows.h](https://en.wikipedia.org/wiki/Windows.h) / [unistd.h](https://en.wikipedia.org/wiki/Unistd.h) | N/A
[mem_usage](tools/mem_usage) | Memory Usage | To track and print out heap memory informations in console | C++14 | N/A | lio
[ImGui-SFML.NET](tools/ImGui-SFML.NET) | ImGui</span>.Net-SFML</span>.NET Implementation | Use Dear ImGui</span>.NET with SFML</span>.NET | C# 9.0, .NET 5.0 | [SFML.NET 2.5.0](https://www.sfml-dev.org/download/sfml.net/), [ImGui.NET 1.78.0](https://github.com/mellinoe/ImGui.NET) | ImGuiNET.ImGuiSFML
