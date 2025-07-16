#pragma once

#include <fstream>
#include <map>
#include <string>

class BitcoinExchange
{
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &cpy);
		BitcoinExchange &operator=(const BitcoinExchange &cpy);
		~BitcoinExchange();
		void compute(std::ifstream &in);
		bool isValidDate(const std::string &str) const;
		std::string prevDate(std::string);
	private:
		std::map<std::string, double> stored;
		void loadHistory();
};


