#ifndef TRANSACTION_H_INCLUDED
#define TRANSACTION_H_INCLUDED
#include<iostream>
#include <fstream>
#include "Wallet.h"
struct Transaction {
	long long time;
	unsigned senderId;
	unsigned receiverId;
	double fmiCoins;
};
bool addTransaction(Wallet& wallet);
bool addTransaction(Wallet& receiver, Wallet& sender, double fmi_coins);
double walletFmiCoins(unsigned id);
unsigned countTransactions(unsigned id);
time_t timeTransaction(unsigned mode, unsigned id);
void printLastAddedTransaction();
void printTransaction(Transaction& transaction);

#endif // TRANSACTION_H_INCLUDED

