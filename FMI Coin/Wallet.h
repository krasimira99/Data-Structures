#ifndef WALLET_H_INCLUDED
#define WALLET_H_INCLUDED
#include<iostream>
#include<fstream>
struct Wallet {
	char owner[256];
	unsigned id;
	double fiatMoney;
};
bool addWallet();
void returnWalletId(Wallet& wallet);
bool getWallet(unsigned id, Wallet& wallet);
bool search(std::ifstream& file, Wallet& wallet, unsigned id);
bool isWalletContained(unsigned id);
double walletFiatMoney(unsigned id);
bool changeFiatMoney(Wallet& wallet);
void topWealthiest();
void change(Wallet& old_wallet, Wallet& new_wallet);
void walletInfo(unsigned id);
bool printLastAddedWallet();
void printAllWallets();
void printWallet(Wallet& wallet);
#endif // WALLET_H_INCLUDED


