#pragma once

#include <iostream>
#include <vector>

class ConsoleDisplayer
{
private:

	std::vector<std::vector<int>> screen;

public:

	ConsoleDisplayer(std::vector<std::vector<int>> screen) : screen(screen)
	{}

	void Display()
	{
		std::vector<unsigned char> screen_str;

		for (auto& h : screen)
		{
			for (auto& w : h)
			{
				switch (w)
				{
					case 0:
						screen_str.push_back(' ');
						screen_str.push_back(' ');
						break;

					case 1:
						screen_str.push_back(176);
						screen_str.push_back(176);
						break;

					case 2:
						screen_str.push_back(177);
						screen_str.push_back(177);
						break;

					case 3:
						screen_str.push_back(178);
						screen_str.push_back(178);
						break;

					case 4:
						screen_str.push_back(219);
						screen_str.push_back(219);
						break;

					default:
						break;
				}
			}

			screen_str.push_back('\n');
		}

		for (auto c : screen_str) std::cout << c;
	}
};