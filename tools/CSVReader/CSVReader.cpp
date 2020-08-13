#include "CSVReader.hpp"

#include <fstream>
#include <iostream>

namespace lio
{
	CSVReader::CSVReader(const std::string& path, bool end_line_comma)
	{
		m_val.resize(1);

		std::ifstream csv_file;
		csv_file.open(path);

		if (!csv_file.good())
			std::cerr << "Failed to read csv file from " << path << std::endl;

		char buf_c;
		std::string buf_s = "";
		size_t row = 0u;

		while (csv_file.read(&buf_c, 1))
		{
			if (buf_c == '\n')
			{
				m_val.push_back(std::vector<std::string>());
				if (!end_line_comma)
					m_val.at(row).push_back(buf_s);
				++row;
				buf_s.clear();
			}
			else if (buf_c == ',')
			{
				m_val.at(row).push_back(buf_s);
				buf_s.clear();
			}
			else
			{
				buf_s += buf_c;
			}
		}

		csv_file.close();
	}

	std::string& CSVReader::At(size_t row, size_t col)
	{
		return m_val.at(row).at(col);
	}

	const std::string& CSVReader::At(size_t row, size_t col) const
	{
		return m_val.at(row).at(col);
	}

	std::vector<std::string>& CSVReader::At(size_t row)
	{
		return m_val.at(row);
	}

	const std::vector<std::string>& CSVReader::At(size_t row) const
	{
		return m_val.at(row);
	}

	size_t CSVReader::Row() const
	{
		return m_val.size();
	}

	void CSVReader::PrintAll(char delimiter, bool end_line_delimiter) const
	{
		for (auto& row : m_val)
		{
			for (auto& str : row)
			{
				std::cout << str;
				if (end_line_delimiter || &str - &row[0] != row.size() - 1 && !end_line_delimiter)
					std::cout << delimiter;
			}
			std::cout << std::endl;
		}
	}

	void CSVReader::PrintAll(const std::string& delimiter, bool end_line_delimiter) const
	{
		for (auto& row : m_val)
		{
			for (auto& str : row)
			{
				std::cout << str;
				if (end_line_delimiter || &str - &row[0] != row.size() - 1 && !end_line_delimiter)
					std::cout << delimiter;
			}
			std::cout << std::endl;
		}
	}
}