# PersonalUtils

All my personal utilities, majority of the code is written myself, feel free to use.
The column 'Language Standard/Version' just means the standard/version the code was developed and tested in.

[Algorithms](algorithms)
[Code Architectures](code%20architectures)  
[Data Structures](data%20structures)  
[Tools](tools)  

### Algorithms

File | Description | Language Standard/Version | Dependencies | Namespace/Class
--- | --- | --- | --- | ---
[AStarPathfinder.hpp](algorithms/AStarPathfinder.hpp) | An A* path finder class can be used on a 2D std::vector | C++14 | N/A | lio
[RandomMoveCubeScramble.cpp](algorithms/RandomMoveCubeScramble.cpp) | Generate a Rubik's Cube scramble with a given number of sides | C++14 | N/A | N/A

### Code Architectures

File | Description | Language Standard/Version | Dependencies | Namespace/Class
--- | --- | --- | --- | ---
[lecs.hpp](code%20architectures/lecs.hpp) | Single header ECS library, managers for components, entities, and systems, with an event system, also managed by an event Manager | C++14 | N/A | lecs
[lev.hpp](code%20architectures/lev.hpp) | Single header simple event system library | C++20 | N/A | lev
[LIC](code%20architectures/LIC) | Single header data oriented and data driven library, centralized and managed Entity(ID)-Component relationship | C++20 | N/A | lic
[Pipe.hpp](code%20architectures/Pipe.hpp) | Single header pipe function, with overloaded operators so it feels like functional programming in nature | C++20 | N/A | lio 
### Data Structures

File | Description | Language Standard/Version | Dependencies | Namespace/Class
--- | --- | --- | --- | ---
[Mat.hpp](data%20structures/Math/Mat.hpp) | A Matrix class with many math operations | C++14 | N/A | lio
[Vec2.hpp](data%20structures/Math/Vec2.hpp) | 2D Vector class with many graphical functions | C++14 | N/A | lio
[Vec3.hpp](data%20structures/Math/Vec3.hpp) | Same as Vec2 except 3D | C++14 | N/A | lio
[Vec.hpp](data%20structures/Math/Vec.hpp) | Template functions for Vec2 and Vec3 | C++14 | N/A | lio::Vec
[MatVecConversion.hpp](data%20structures/Math/MatVecConversion.hpp) | Functions to convert Mat to Vec2/Vec3 or vice versa | C++14 | [Mat.hpp](data%20structures/Math/Mat.hpp), [Vec2.hpp](data%20structures/Math/Vec2.hpp), [Vec3.hpp](data%20structures/Math/Vec3.hpp) | lio
[LineSeg.hpp](data%20structures/Math/LineSeg.hpp) | 2D line segment class | C++14 | [Vec2.hpp](data%20structures/Math/Vec2.hpp) | lio
[Data Structures for C](data%20structures/Data%20Structures%20for%20C) | Template structs for C with a lot of functions from C++ STL classes | C99 | N/A | N/A

### Tools

File | Description | Language Standard/Version | Dependencies | Namespace/Class
--- | --- | --- | --- | ---
[ANSIEscaper](tools/ANSIEscaper) | A group of cross-platform ANSI escape code wrapper functions | C++14 / C99 | [windows.h](https://en.wikipedia.org/wiki/Windows.h) (for windows)|
[CSVReader](tools/CSVReader) | CSV (Comma Separated Values) file reader and parser | C++11 | N/A | lio
[LConsoleScreen](tools/LConsoleScreen) | Display Ascii characters as 2D screen in cmd (Windows only) | C++11 / C99 | [windows.h](https://en.wikipedia.org/wiki/Windows.h) | lio
[PortableSystemFunctions.h](tools/PortableSystemFunctions.h) | Cross-platform system functions | C++17 / C17 | [windows.h](https://en.wikipedia.org/wiki/Windows.h) / [unistd.h](https://en.wikipedia.org/wiki/Unistd.h) | N/A
[mem_usage](tools/mem_usage) | To track and print out heap memory informations in console | C++14 | N/A | lio
[ImGui-SFML.NET](tools/ImGui-SFML.NET) | Use Dear ImGui</span>.NET with SFML</span>.NET | C# 9.0, .NET 5.0 | [SFML.NET 2.5.0](https://www.sfml-dev.org/download/sfml.net/), [ImGui.NET 1.78.0](https://github.com/mellinoe/ImGui.NET) | ImGuiNET.ImGuiSFML
