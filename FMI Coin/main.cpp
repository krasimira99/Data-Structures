#include "HelperFunctions.h"
#include<iostream>
#include<iomanip>
int main() {
	std::cout << ">>>>>WELCOME TO THE FMI-COIN TRADING PROGRAM!<<<<<" << std::endl;
	std::cout << std::endl;
	std::cout << "->Create yout wallet by entering 'add-wallet'" << std::endl;
	std::cout << "->If you want to SELL or BUY FMI coins enter 'make-order'" << std::endl;
	std::cout << "->To see the 10 richest users enter 'attract-investors'" << std::endl;
	std::cout << "->To see information about a user enter 'wallet-info' and the id of the user" << std::endl;
	std::cout << "->To exit the program enter 'quit'" << std::endl;
	start();
	return 0;
}