#pragma once

#include <string>
#include <vector>

namespace lio
{
	class CSVReader
	{
	private:

		std::vector<std::vector<std::string>> m_val;

	public:

		CSVReader(const std::string& path, bool end_line_comma = false);

		std::string& At(size_t row, size_t col);
		const std::string& At(size_t row, size_t col) const;

		std::vector<std::string>& At(size_t row);
		const std::vector<std::string>& At(size_t row) const;

		size_t Row() const;

		void PrintAll(char delimiter = ',', bool end_line_delimiter = false) const;
		void PrintAll(const std::string& delimiter, bool end_line_delimiter = false) const;
	};
}