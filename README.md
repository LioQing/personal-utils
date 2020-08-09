# PersonalUtils

All my personal utilities, majority of the code is written myself, feel free to use.  
Unless specified otherwise, all the symbols are under the namespace **lio**.

[Code Architectures](https://github.com/LioQing/PersonalUtils/tree/master/code%20architectures)  
[Data Structures](https://github.com/LioQing/PersonalUtils/tree/master/data%20structures)  
[Tools](https://github.com/LioQing/PersonalUtils/tree/master/tools)  

### Code Architectures

File | Name | Description
------------ | ------------- | -------------
[lecs.hpp](https://github.com/LioQing/PersonalUtils/blob/master/code%20architectures/lecs.hpp) | Lio's ECS | Single header ECS library, managers for components, entities, and systems, with an event system, also managed by an event Manager
[LIC](https://github.com/LioQing/PersonalUtils/blob/master/code%20architectures/LIC) | Lio's IC | Single header data oriented and data driven library, centralized and managed Entity(ID)-Component relataionship (Can be used for ECS)
### Data Structures

File | Name | Description
------------ | ------------- | -------------
[Matrix.hpp](https://github.com/LioQing/PersonalUtils/blob/master/data%20structures/Matrix.hpp) | Matrix | A half-finished matrix class
[Vec2.hpp](https://github.com/LioQing/PersonalUtils/blob/master/data%20structures/Graphics/Vec2.hpp) | Vector2 | 2D Vector class with many graphical functions
[LineSeg.hpp](https://github.com/LioQing/PersonalUtils/blob/master/data%20structures/Graphics/LineSeg.hpp) | Line Segment | 2D line segment class
[Circle.hpp](https://github.com/LioQing/PersonalUtils/blob/master/data%20structures/Graphics/Circle.hpp) | Circle | 2D circle class
[Rect.hpp](https://github.com/LioQing/PersonalUtils/blob/master/data%20structures/Graphics/Rect.hpp) | Rectangle | 2D rectangle class
[Transformable.hpp](https://github.com/LioQing/PersonalUtils/blob/master/data%20structures/Graphics/Transformable.hpp) | Transformable | Transformable for other 2D shape classes to inherit from

### Tools

File | Name | Description
------------ | ------------- | -------------
[CSVReader](https://github.com/LioQing/PersonalUtils/blob/master/tools/CSVReader) | CSV Reader | CSV (Comma Separated Values) file reader and parser
[LConsoleScreen](https://github.com/LioQing/PersonalUtils/tree/master/tools/LConsoleScreen) | Lio's Console Screen | Display Ascii characters as 2D screen in cmd (Windows only) (no delay refresh rate) (better than the one above!!)