#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <fstream>
#include <string>

BitcoinExchange::BitcoinExchange()
{
	loadHistory();
}

void remove_spaces(std::string &str)
{
	// std::cout << "Scanning '" << str << "'" << std::endl;
	for (std::string::iterator it = str.begin() ; it != str.end() ; it++)
		if (*it == ' ')
		{
			str.erase(it);
			it--;
		}
}


void BitcoinExchange::loadHistory()
{
	std::ifstream file("../data.csv", std::ios::in);
	std::string buff;

	while (std::getline(file, buff, '\n'))
	{
		int sep = buff.find(',');
		std::string first, second;
		first.append(buff.begin(), buff.begin() + sep);
		second.append(buff.begin() + sep + 1, buff.end());
		remove_spaces(first);
		remove_spaces(second);
		this->stored[first] = std::atof(second.c_str());
	}
}

bool hasAChar(std::string &str)
{
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
		if ((*it < '0' || *it > '9') && *it != '.' && *it != '-')
			return true;
	return false;
}

std::string BitcoinExchange::prevDate(std::string date)
{
	int year, month, day;
	year = std::atoi(date.substr(0, 4).c_str());
	month = std::atoi(date.substr(5, 2).c_str());
	day = std::atoi(date.substr(8, 2).c_str());
	
	int days[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (year % 4)
		days[1] ++;

	day--;
	if (day == 0)
	{
		month --;
		if (month == 0)
		{
			year --;
			if (year == 0)
				return "";
			month = 12;
		}
		day = days[month - 1];
	}
	std::ostringstream oss;
	oss << year << "-" << month << "-" << day;
	return (oss.str());
}

void BitcoinExchange::compute(std::ifstream &in)
{
	std::string buff;

	while (std::getline(in, buff, '\n'))
	{
		int sep = buff.find('|');
		std::string first;
		std::string second;
		if (sep < 1)
		{
			std::cout << "Can't parse the line '" << buff << "'" << std::endl;
			continue;
		}
		first.append(buff.begin(), buff.begin() + sep);
		second.append(buff.begin() + sep + 1, buff.end());
		remove_spaces(first);
		remove_spaces(second);

		if (!this->isValidDate(first))
		{
			std::cout << "'" << first << "' is not a valid date" << std::endl;
			continue ;
		}
		while (this->stored.find(first) == this->stored.end() && !first.empty())
		{
			first = this->prevDate(first);
		}
		if (first.empty())
		{
			std::cout << "Date is too low" << std::endl;
			continue ;
		}
		if (hasAChar(second))
		{
			std::cout << "'" << second << "' is not a number" << std::endl;
			continue ;
		}
		double value = std::atol(second.c_str());
		if (value < 0.0 || value > 1000.0)
		{
			std::cout << "'" << second << "' is not between 0 and 1000" << std::endl;
			continue ;
		}
		std::cout << first << " => " << second << " = " << value * this->stored[first] << std::endl;
	}
	in.close();
}

bool BitcoinExchange::isValidDate(const std::string &str) const
{
	int test;
	if (str[4] != '-' || str[7] != '-')
		return false;
	std::string year, month, day;
	year = str.substr(0, 4);
	month = str.substr(5, 2);
	day = str.substr(8, 2);
	if (hasAChar(year) || hasAChar(month) || hasAChar(day) || day.length() != 2)
		return false;
	test = std::atoi(month.c_str());
	if (test < 0 || test > 12)
		return false;
	test = std::atoi(day.c_str());
	if (test > 0 && test <= 28)
		return true;
	if ((test == 29 || test == 30) && month != "02")
		return true;
	if (test == 31 && (month == "01" || month == "03" || month == "05" || month == "07" || month == "08" || month == "10" || month == "12"))
		return true;
	if (test == 29 && month == "02" && std::atoi(year.c_str()) % 4 == 0)
		return true;
	return false;
}


BitcoinExchange::~BitcoinExchange()
{
}
