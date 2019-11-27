#include "Order.h"
#include"HelperFunctions.h"
#define ORDER_PATH "orders.dat"
#include <string>
#include<ctime>
void makeOrder() {
	Order order;
	char order_type[5];
	std::cout << "SELL/BUY: ";
	std::cin.ignore();
	std::cin.getline(order_type, 5, ' ');
	std::cout << "FMI coins: ";
	std::cin >> order.fmiCoins;
	std::cout << "ID: ";
	std::cin >> order.walletId;
	if (strcmp(order_type, "SELL") == 0 || strcmp(order_type, "sell") == 0) {
		order.type = order.SELL;
	}
	else if (strcmp(order_type, "BUY") == 0 || strcmp(order_type, "buy") == 0) {
		order.type = order.BUY;
	}
	else {
		std::cerr << "Undefined command." << std::endl;
		return;
	}

	if (!isWalletContained(order.walletId)) {
		std::cerr << "Order error. Didn't find walletId." << std::endl;
		return;
	}
	if (order.type == order.SELL) {
		if (walletFmiCoins(order.walletId) - returnOrderCoins(order.walletId) < order.fmiCoins) {
			std::cerr << "Order error. Not enough FMICoins to sell." << std::endl;
			return;
		}
	}
	else if (order.type == order.BUY) {
		if (walletFiatMoney(order.walletId) < order.fmiCoins * 375) {
			std::cerr << "Order error. Not enough fiat money to buy coins." << std::endl;
			return;
		}
	}

	processOrder(order);
}

void processOrder(Order& order) {
	std::streampos file_size;
	std::ifstream file_r(ORDER_PATH, std::ios::binary);
	if (!file_r) {
		file_size = 0;
		file_r.clear();
	}
	else {
		file_r.seekg(0, std::ios::end);
		file_size = file_r.tellg();
	}
	file_r.close();

	if (file_size < sizeof(Order)) {
		extendOrder(order);
		return;
	}

	std::ifstream file(ORDER_PATH, std::ios::binary | std::ios::app);
	if (!file) {
		std::cerr << ORDER_PATH << ": error while opening." << std::endl;
		return;
	}
	unsigned number_orders = file_size / sizeof(Order);
	Order* orders = new(std::nothrow) Order[number_orders];
	if (!orders) {
		std::cerr << "Error while allocating memory." << std::endl;
		file.close();
		return;
	}
	file.seekg(0, std::ios::beg);
	file.read((char*)orders, sizeof(Order) * number_orders);
	if (!file) {
		std::cerr << ORDER_PATH << ": error while reading." << std::endl;
		file.close();
		delete[] orders;
		return;
	}
	file.close();

	if (orders[0].type == order.type) {
		extendOrder(order);
		delete[] orders;
		return;
	}

	bool* is_orders_processed = new(std::nothrow) bool[number_orders];
	if (!is_orders_processed) {
		std::cerr << "Error while allocating memory." << std::endl;
		delete[] orders;
		return;
	}
	for (unsigned i = 0; i < number_orders; i++) {
		is_orders_processed[i] = false;
	}
	bool is_processed = false;
	unsigned order_count = 0;
	Wallet order_wallet;
	getWallet(order.walletId, order_wallet);
	while (!is_processed) {
		if (order_count >= number_orders) {
			is_processed = true;
		}
		else {
			if (order.fmiCoins >= orders[order_count].fmiCoins) {
				order.fmiCoins -= orders[order_count].fmiCoins;
				is_orders_processed[order_count] = true;
				if (order.type == order.BUY) {
					if (order.walletId != orders[order_count].walletId) {
						Wallet sender_wallet;
						getWallet(orders[order_count].walletId, sender_wallet);
						addTransaction(order_wallet, sender_wallet, orders[order_count].fmiCoins);
						sender_wallet.fiatMoney += orders[order_count].fmiCoins * 375;
						order_wallet.fiatMoney -= orders[order_count].fmiCoins * 375;
						if (!changeFiatMoney(sender_wallet)) {
							delete[] orders;
							delete[] is_orders_processed;
							return;
						}
						if (!changeFiatMoney(order_wallet)) {
							delete[] orders;
							delete[] is_orders_processed;
							return;
						};
					}

				}
				else if (order.type == order.SELL) {
					if (order.walletId != orders[order_count].walletId) {
						Wallet receiver_wallet;
						getWallet(orders[order_count].walletId, receiver_wallet);
						addTransaction(receiver_wallet, order_wallet, orders[order_count].fmiCoins);
						receiver_wallet.fiatMoney -= orders[order_count].fmiCoins * 375;
						order_wallet.fiatMoney += orders[order_count].fmiCoins * 375;
						if (!changeFiatMoney(receiver_wallet)) {
							delete[] orders;
							delete[] is_orders_processed;
							return;
						}
						if (!changeFiatMoney(order_wallet)) {
							delete[] orders;
							delete[] is_orders_processed;
							return;
						}
					}
				}
				order_count++;
			}
			else {
				is_processed = true;
				orders[order_count].fmiCoins -= order.fmiCoins;
				if (order.type == order.BUY) {
					if (order.walletId != orders[order_count].walletId) {
						Wallet sender_wallet;
						getWallet(orders[order_count].walletId, sender_wallet);
						addTransaction(order_wallet, sender_wallet, order.fmiCoins);
						sender_wallet.fiatMoney += order.fmiCoins * 375;
						order_wallet.fiatMoney -= order.fmiCoins * 375;
						if (!changeFiatMoney(sender_wallet)) {
							delete[] orders;
							delete[] is_orders_processed;
							return;
						}
						if (!changeFiatMoney(order_wallet)) {
							delete[] orders;
							delete[] is_orders_processed;
							return;
						}
					}
				}
				else if (order.type == order.SELL) {
					if (order.walletId != orders[order_count].walletId) {
						Wallet receiver_wallet;
						getWallet(orders[order_count].walletId, receiver_wallet);
						addTransaction(receiver_wallet, order_wallet, order.fmiCoins);
						receiver_wallet.fiatMoney -= order.fmiCoins * 375;
						order_wallet.fiatMoney += order.fmiCoins * 375;
						if (!changeFiatMoney(receiver_wallet)) {
							delete[] orders;
							delete[] is_orders_processed;
							return;
						}
						if (!changeFiatMoney(order_wallet)) {
							delete[] orders;
							delete[] is_orders_processed;
							return;
						}
					}
				}
				order.fmiCoins = 0;
			}
		}

	}
	std::string new_name;
	new_name += std::to_string(order.walletId);
	new_name += '_';
	new_name += std::to_string(time(0));
	new_name += ".txt";
	std::cout << new_name << std::endl;
	std::ofstream new_file(ORDER_PATH, std::ios::binary | std::ios::out);
	if (!new_file) {
		std::cerr << ORDER_PATH << ": error while opening." << std::endl;
		delete[] is_orders_processed;
		delete[] orders;
		return;
	}
	if (order.fmiCoins != 0) {
		new_file.write((char*)& order, sizeof(order));
		if (!new_file) {
			std::cerr << ORDER_PATH << ": error while writing." << std::endl;
		}
	}
	else if (order.fmiCoins == 0) {
		for (unsigned i = 0; i < number_orders; i++) {
			if (!is_orders_processed[i]) {
				new_file.write((char*)& orders[i], sizeof(orders[i]));
				if (!new_file) {
					std::cerr << ORDER_PATH << ": error while writing." << std::endl;
					break;
				}
			}
		}
	}
	new_file.close();
	delete[] is_orders_processed;
	delete[] orders;
}

void extendOrder(Order& order) {
	std::ofstream file(ORDER_PATH, std::ios::binary | std::ios::app);
	if (!file) {
		std::cerr << ORDER_PATH << ": error while opening." << std::endl;
		return;
	}
	file.write((char*)& order, sizeof(order));
	if (!file) {
		std::cerr << ORDER_PATH << ": error while writing." << std::endl;
		file.clear();
	}
	file.close();
}

double returnOrderCoins(unsigned id) {
	std::ifstream file(ORDER_PATH, std::ios::binary);
	if (!file) {
		std::cerr << ORDER_PATH << ": error while opening." << std::endl;
		return 0;
	}
	file.seekg(0, std::ios::end);
	std::streampos file_size = file.tellg();
	unsigned number_orders = file_size / sizeof(Order);
	Order* orders = new(std::nothrow) Order[number_orders];
	if (!orders) {
		std::cerr << "Error while allocating memory." << std::endl;
		file.close();
		return 0;
	}
	file.seekg(0, std::ios::beg);
	file.read((char*)orders, sizeof(Order) * number_orders);
	if (!file) {
		std::cerr << ORDER_PATH << ": error while reading." << std::endl;
		file.close();
		delete[] orders;
		return 0;
	}
	file.close();

	if (orders[0].type != 0) {
		delete[] orders;
		return 0;
	}
	double coins = 0;
	for (unsigned i = 0; i < number_orders; i++) {
		if (orders[i].walletId == id) {
			coins += orders[i].fmiCoins;
		}
	}

	delete[] orders;
	return coins;
}

void printOrders() {
	std::ifstream file(ORDER_PATH, std::ios::binary);
	if (!file) {
		std::cerr << ORDER_PATH << ": error while opening." << std::endl;
		return;
	}
	file.seekg(0, std::ios::end);
	std::streampos file_size = file.tellg();
	unsigned number_orders = file_size / sizeof(Order);
	Order* orders = new(std::nothrow) Order[number_orders];
	if (!orders) {
		std::cerr << "Error while allocation memory. " << ORDER_PATH << std::endl;
		file.close();
		return;
	}
	file.seekg(0, std::ios::beg);
	unsigned offset = sizeof(*orders) * number_orders;
	file.read((char*)orders, offset);
	if (!file) {
		std::cerr << ORDER_PATH << ": error while reading." << std::endl;
		file.close();
		delete[] orders;
		return;
	}
	file.close();

	for (unsigned i = 0; i < number_orders; i++) {
		printOrder(orders[i]);
	}
	delete[] orders;
}

void printOrder(Order& order) {
	std::cout << "Order type: " << order.type << std::endl;
	std::cout << "Order fmiCoins: " << order.fmiCoins << std::endl;
	std::cout << "Order walletId: " << order.walletId << std::endl;
}