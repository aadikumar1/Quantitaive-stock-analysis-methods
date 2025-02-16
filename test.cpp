#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
using namespace std::chrono;

struct Order {
    double price;
    int quantity;
    bool isBid; // true = bid, false = ask
};

class OrderBook {
private:
    vector<Order> bids, asks;

public:
    void addOrder(double price, int quantity, bool isBid) {
        Order order = {price, quantity, isBid};
        if (isBid) bids.push_back(order);
        else asks.push_back(order);

        sortOrders();
    }

    void sortOrders() {
        sort(bids.begin(), bids.end(), [](Order a, Order b) { return a.price > b.price; });
        sort(asks.begin(), asks.end(), [](Order a, Order b) { return a.price < b.price; });
    }

    void matchOrders() {
        while (!bids.empty() && !asks.empty() && bids[0].price >= asks[0].price) {
            int tradeSize = min(bids[0].quantity, asks[0].quantity);
            cout << "Trade Executed: " << tradeSize << " @ " << asks[0].price << endl;

            bids[0].quantity -= tradeSize;
            asks[0].quantity -= tradeSize;

            if (bids[0].quantity == 0) bids.erase(bids.begin());
            if (asks[0].quantity == 0) asks.erase(asks.begin());
        }
    }

    void printBook() {
        cout << "\nORDER BOOK (TOP 3 LEVELS):\n";
        cout << "BIDS:\n";
        for (size_t i = 0; i < min(bids.size(), size_t(3)); i++)
            cout << bids[i].quantity << " @ " << bids[i].price << endl;
        cout << "ASKS:\n";
        for (size_t i = 0; i < min(asks.size(), size_t(3)); i++)
            cout << asks[i].quantity << " @ " << asks[i].price << endl;
    }
};

int main() {
    OrderBook orderBook;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> priceDist(99.5, 100.5);
    uniform_int_distribution<> qtyDist(1, 10);

    auto start = high_resolution_clock::now();
    while (duration_cast<seconds>(high_resolution_clock::now() - start).count() < 5) {  // Run for 5 seconds
        double price = priceDist(gen);
        int qty = qtyDist(gen);
        bool isBid = rand() % 2;

        orderBook.addOrder(price, qty, isBid);
        orderBook.matchOrders();
        orderBook.printBook();

        this_thread::sleep_for(milliseconds(100));  // Simulating high-speed order flow
    }

    return 0;
}
