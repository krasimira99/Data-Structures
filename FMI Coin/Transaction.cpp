#define _CRT_SECURE_NO_WARNINGS
#include "Transaction.h"
#include "HelperFunctions.h"
#include <ctime>
#include<iomanip>
#define TRANSACTION_PATH "transactions.dat"

bool addTransaction(Wallet& receiver_wallet) {
	Transaction transaction;
	const int system_wallet_id = 4294967295;
	const unsigned rate = 375;
	std::time_t t_now = time(0);
	transaction.senderId = system_wallet_id;
	transaction.time = t_now;
	transaction.receiverId = receiver_wallet.id;
	transaction.fmiCoins = receiver_wallet.fiatMoney / rate;

	std::fstream file(TRANSACTION_PATH, std::ios::binary | std::ios::app);
	if (!file) {
		std::cerr << TRANSACTION_PATH << " didn't open" << std::endl;
		return false;
	}
	file.write((char*)& transaction, sizeof(transaction));
	if (!file) {
		std::cerr << TRANSACTION_PATH << ": error while writing" << std::endl;
	}
	file.close();
	return true;
}

bool addTransaction(Wallet& receiver_wallet, Wallet& sender_wallet, double coins) {
	Transaction transaction;
	transaction.senderId = sender_wallet.id;
	transaction.receiverId = receiver_wallet.id;
	std::time_t t_now = time(0);
	transaction.time = t_now;
	transaction.fmiCoins = coins;
	std::ofstream file(TRANSACTION_PATH, std::ios::binary | std::ios::app);
	if (!file) {
		std::cerr << TRANSACTION_PATH << ": error while opening." << std::endl;
		return false;
	}
	file.write((char*)& transaction, sizeof(transaction));
	if (!file) {
		std::cerr << TRANSACTION_PATH << ": error while writing." << std::endl;
		file.close();
		return false;
	}
	file.close();
	return true;
}

double walletFmiCoins(unsigned id) {
	Transaction transaction;
	double coins = 0;
	std::ifstream file(TRANSACTION_PATH, std::ios::binary);
	if (!file) {
		std::cerr << TRANSACTION_PATH << ": error while opening." << std::endl;
		return -1;
	}
	while (!file.eof()) {
		file.read((char*)& transaction, sizeof(transaction));
		if (file) {
			if (transaction.receiverId == id) {
				coins += transaction.fmiCoins;
			}
			else if (transaction.senderId == id) {
				coins -= transaction.fmiCoins;
			}
		}
	}
	file.close();
	return coins;
}

unsigned countTransactions(unsigned id) {
	Transaction transaction;
	unsigned counter= 0;
	std::ifstream file(TRANSACTION_PATH, std::ios::binary);
	if (!file) {
		std::cerr << TRANSACTION_PATH << ": error while opening." << std::endl;
		return -1;
	}
	while (!file.eof()) {
		file.read((char*)& transaction, sizeof(transaction));
		if (file) {
			if (transaction.receiverId == id || transaction.senderId == id) {
				counter++;
			}
		}
	}
	file.close();
	return counter;
}
//saw how to use the time on cplusplus.com but it doesn't work very good :)
time_t timeTransaction(unsigned mode, unsigned id) {
	Transaction transaction;
	time_t transaction_time;
	struct tm* timeInfo;
	time(&transaction_time);
	timeInfo = localtime(&transaction_time);
	if (mode == 1) {
		std::ifstream file(TRANSACTION_PATH, std::ios::binary);
		if (!file) {
			std::cerr << TRANSACTION_PATH << ": error while opening." << std::endl;
			return -1;
		}
		bool found = false;
		while (!found) {
			file.read((char*)& transaction, sizeof(transaction));
			if (file) {
				if (transaction.senderId == id || transaction.receiverId == id) {
					found = true;
					transaction_time = (time_t)asctime(timeInfo);
				}
			}
		}
		file.close();
	}
	else {
		std::ifstream file(TRANSACTION_PATH, std::ios::binary);
		if (!file) {
			std::cerr << TRANSACTION_PATH << ": error while opening." << std::endl;
			return -1;
		}
		bool found_last = false;
		while (!found_last) {
			file.read((char*)& transaction, sizeof(transaction));
			if (!file) {
				found_last = true;
			}
			if (transaction.senderId == id || transaction.receiverId == id) {
				transaction_time = (time_t)asctime(timeInfo);
			}
		}
		file.close();
	}
	return transaction_time;
}

void printLastAddedTransaction() {
	Transaction transaction;
	std::ifstream file(TRANSACTION_PATH, std::ios::binary);
	if (!file) {
		std::cerr << TRANSACTION_PATH << ": error while opening." << std::endl;
		return;
	}
	file.seekg(-(int)sizeof(transaction), std::ios::end);
	file.read((char*)& transaction, sizeof(transaction));
	if (!file) {
		std::cerr << TRANSACTION_PATH << ": error while reading." << std::endl;
		file.close();
		return;
	}
	printTransaction(transaction);
	file.close();
}

void printTransaction(Transaction& transaction) {
	std::cout << "Sender id: " << transaction.senderId << std::endl;
	std::cout << "Receiver id: " << transaction.receiverId << std::endl;
	std::cout << "Time: " << transaction.time << std::endl;
	std::cout << "FMICoins: " << transaction.fmiCoins << std::endl;
}

