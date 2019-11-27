#ifndef ORDER_H_INCLUDED
#define ORDER_H_INCLUDED
#include<iostream>
#include<fstream>

struct Order {
	enum Type { SELL, BUY } type;
	unsigned walletId;
	double fmiCoins;
};

void makeOrder();
void processOrder(Order& order);
void extendOrder(Order& order);
double returnOrderCoins(unsigned id);
void printOrders();
void printOrder(Order& order);



#endif // ORDER_H_INCLUDED
