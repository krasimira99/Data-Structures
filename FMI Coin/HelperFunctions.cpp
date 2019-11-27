#define _CRT_SECURE_NO_WARNINGS
#include "HelperFunctions.h"
void start() {
	bool stop = false;

	const char* const quit = "quit";
	const char* const add_wallet = "add-wallet";
	const char* const make_order = "make-order";
	const char* const wallet_info = "wallet-info";
	const char* const attract_investors = "attract-investors";

	while (!stop) {
		char instruction[18];
		std::cin >> instruction;
		if (strcmp(instruction, quit) == 0) {
			stop = true;
		}
		else if (strcmp(instruction, add_wallet) == 0) {
			if (addWallet() == 0) {
				std::cerr << "Problem with adding wallet." << std::endl;
			}
		}
		else if (strcmp(instruction, make_order) == 0) {
			makeOrder();
		}
		else if (strcmp(instruction, attract_investors) == 0) {
			topWealthiest();
		}
		else if (strcmp(instruction, wallet_info) == 0) {
			int id;
			std::cin >> id;
			if (id > 0) {
				walletInfo(id);
			}
			else {
				std::cerr << "Error: wrong arguments for wallet-info" << std::endl;
			}
		}
		else {
			std::cerr << "Undefined command." << std::endl;
		}
	}

}


