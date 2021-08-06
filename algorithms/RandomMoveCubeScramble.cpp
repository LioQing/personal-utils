#include <random>
#include <array>
#include <algorithm>

template <size_t MOVE_COUNT>
auto scramble(int no_face, int no_turn, std::vector<int> (*GetAdj)(int))
{
    std::random_device rd;
    std::default_random_engine face_engine(rd());
    std::default_random_engine turn_engine(rd());
    std::uniform_int_distribution<int> face_dist(0, no_face - 1);
    std::uniform_int_distribution<int> turn_dist(0, no_turn - 1);

    std::array<std::pair<int, int>, MOVE_COUNT> scramble;

    int forbid_face = -1;
    int last_face = -1;
    std::vector<int> last_adj;
    for (auto i = 0; i < MOVE_COUNT; ++i)
    {
    restart:
        int face = face_dist(face_engine);

        if (last_face == face)
            goto restart;

        if (forbid_face != -1 && face == forbid_face)
            goto restart;

        if (find(last_adj.begin(), last_adj.end(), face) == last_adj.end())
            forbid_face = last_face;
        else
            forbid_face == -1;

        scramble[i].first = face;
        scramble[i].second = turn_dist(turn_engine);

        last_face = face;
        last_adj = GetAdj(face);
    }

    return scramble;
}

/*
the following is a demonstration of the function for reference
*/

#include <string>
#include <iostream>

// provide function for adjacent faces
// so that moves such as U D U' would not happen
std::vector<int> Get333FaceAdj(int face)
{
    if (face == 0 || face == 1)
        return { 2, 3, 4, 5 };
    else if (face == 2 || face == 3)
        return { 0, 1, 4, 5};
    else if (face == 4 || face == 5)
        return { 0, 1, 2, 3};
    else
        return {};
}

// 3x3x3 notation output
std::string Get333TurnNotation(int face, int turn)
{
    static const std::string face_notation[6] = { "U", "D", "L", "R", "F", "B" };
    static const std::string turn_notation[3] = { "", "'", "2" };

    return face_notation[face] + turn_notation[turn];
}

// 2x2x2 notation output
std::string Get222TurnNotation(int face, int turn)
{
    static const std::string face_notation[6] = { "U", "R", "F" };
    static const std::string turn_notation[3] = { "", "'", "2" };

    return face_notation[face] + turn_notation[turn];
}

int main()
{
    // 3x3x3 scramble demo
    std::cout << "3x3x3: ";
    for (auto [face, turn] : scramble<25>(6, 3, Get333FaceAdj))
    {
        std::cout << Get333TurnNotation(face, turn) << ' ';
    }
    std::cout << std::endl;
    
    // 2x2x2 scramble demo
    std::cout << "2x2x2: ";
    for (auto [face, turn] : scramble<10>(3, 3, [](int) -> std::vector<int> { return {}; }))
    {
        std::cout << Get222TurnNotation(face, turn) << ' ';
    }
    std::cout << std::endl;

    return 0;
}