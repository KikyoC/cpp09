#include "BitcoinExchange.hpp"

int main()
{

	BitcoinExchange *btc = new BitcoinExchange();
	std::ifstream *in = new std::ifstream("input.txt", std::ios::in);
	btc->compute(*in);
	delete btc;
	delete in;
}
