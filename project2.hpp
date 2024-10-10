#include <getopt.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <queue>

using namespace std;

struct Stock {
    char buyer_or_seller;
    int32_t small_dollars = 0;
    int32_t dollars = 0;
    int32_t profit_margin;
    int32_t shares;
    uint32_t stock_name;
    int32_t trader_name; 
    int32_t id = 0;
    int32_t timestamp; 
    int32_t timestamp2; 
};


struct BuyerComparator {
    bool operator()(const Stock &lhs, const Stock &rhs) {
        if (lhs.dollars == rhs.dollars) {
            return lhs.id > rhs.id;
        }
        return lhs.dollars < rhs.dollars;  // Higher price is prioritized
    }
};

struct SellerComparator {
    bool operator()(const Stock &lhs, const Stock &rhs) {
        if (lhs.dollars == rhs.dollars) {
            return lhs.id > rhs.id;
        }
        return lhs.dollars > rhs.dollars;  // Lower price is prioritized
    }
};

uint32_t median_find(vector<uint32_t> v) {
    sort(v.begin(), v.end());
    if (v.size() % 2 == 0){
        return (v[(v.size()/2 - 1)] + v[(v.size()/2)])/2;
    } else {
        return v[v.size()/2];  // Fixed the floating-point to integer conversion
    }
}



struct Stock_Sold {
    int32_t who_bought;
    int32_t seller; 
    uint32_t stock_got_sold;
    int32_t net_transfer;
    int32_t shares_bought;
    int32_t timestamp; 
};

class StockTrading {
private:
    string mode_for_stocks; 
    vector<priority_queue<Stock, vector<Stock>, BuyerComparator>> buyers;
    vector<priority_queue<Stock, vector<Stock>, SellerComparator>> sellers;
    vector<vector<Stock>> time_traveling; 
    vector<Stock_Sold> output;
    int32_t trades_completed; 
    uint32_t num_traders;
    uint32_t num_stocks;
    int32_t current_timestamp; 
    bool print_median; 

public:
    StockTrading(string modeinput, uint32_t numberofstock, uint32_t numoftrad) : mode_for_stocks(modeinput), trades_completed(0), num_traders(numoftrad), num_stocks(numberofstock), current_timestamp(0) {
        buyers.resize(num_stocks);   // Initialize buyers/sellers vectors
        sellers.resize(num_stocks);
    }

    void assortment() {
        for (uint32_t i = 0; i < sellers.size(); i++) {
            for (uint32_t j = 0; j < buyers.size(); j++) {
                if (i == j && !sellers[i].empty() && !buyers[j].empty()) {
                    if (sellers[i].top().trader_name != buyers[i].top().trader_name && sellers[i].top().dollars <= buyers[i].top().dollars) {
                        Stock predict;
                        predict.stock_name = sellers[i].top().stock_name;
                        predict.dollars = buyers[i].top().dollars;
                        predict.small_dollars = sellers[i].top().small_dollars;
                        predict.profit_margin = predict.dollars - predict.small_dollars;
                        predict.trader_name = sellers[i].top().trader_name;
                        predict.timestamp = sellers[i].top().timestamp;
                        predict.timestamp2 = buyers[i].top().timestamp;
                        time_traveling[predict.stock_name].push_back(predict);
                    }
                        if (sellers[i].top().shares > buyers[i].top().shares) {
                            Stock_Sold sold;
                            sold.net_transfer = sellers[i].top().dollars;
                            sold.shares_bought = buyers[i].top().shares;
                            sold.stock_got_sold = sellers[i].top().stock_name;
                            sold.who_bought = buyers[i].top().trader_name;
                            sold.seller = sellers[i].top().trader_name;
                            output.push_back(sold);
                            trades_completed++;
                            Stock replacement = sellers[i].top();
                            replacement.shares -= buyers[i].top().shares;
                            buyers[i].pop();
                            sellers[i].pop();
                            sellers[i].push(replacement);
                        } else {
                            Stock_Sold sold;
                            sold.net_transfer = sellers[i].top().dollars;
                            sold.shares_bought = sellers[i].top().shares;
                            sold.stock_got_sold = sellers[i].top().stock_name;
                            sold.who_bought = buyers[i].top().trader_name;
                            sold.seller = sellers[i].top().trader_name;
                            output.push_back(sold);
                            buyers[i].pop();
                            sellers[i].pop();
                            trades_completed++;
                        }
                    }
                }
            
        }
    }

    void statements() {
        if (mode_for_stocks == "i") {
            cout << "---Trader Info---" << '\n';
            for (uint32_t j = 0; j < num_traders; j++) {
                int sold_shares = 0, bought_shares = 0, net_price = 0;
                for (uint32_t i = 0; i < output.size(); i++) {
                    if (output[i].seller == static_cast<int32_t>(j)) {
                        sold_shares += output[i].shares_bought;
                        net_price += output[i].net_transfer;
                    }
                    if (output[i].who_bought == static_cast<int32_t>(j)) {
                        bought_shares += output[i].shares_bought;
                        net_price -= output[i].net_transfer;
                    }
                }
                cout << "Trader " << j << " bought " << bought_shares << " and sold " << sold_shares << " for a net transfer of " << net_price << '\n';
            }
        }
    }

    void reading_input() {
        int32_t time;
        string sorb;
        char junk;
        int32_t tradernum;
        uint32_t stocknum;
        int32_t price; 
        int32_t amtshares;

        cout << "Processing orders..." << '\n';
        while (!cin.eof()) {
            cin >> time >> sorb >> junk >> tradernum >> junk >> stocknum >> junk >> price >> junk >> amtshares;

            Stock inputted;
            inputted.timestamp = time;
            inputted.trader_name = tradernum;
            inputted.stock_name = static_cast<uint32_t>(stocknum);  // Fix signedness
            inputted.dollars = price;
            inputted.shares = amtshares;

            if (sorb == "SELL") {
                inputted.buyer_or_seller = 's';
                sellers[stocknum].push(inputted);
            } else if (sorb == "BUY") {
                inputted.buyer_or_seller = 'b';
                buyers[stocknum].push(inputted);
            } else {
                cerr << "Error: Invalid stock ID";
            }

            if (current_timestamp != inputted.timestamp) {
                print_median = true;
            }
            current_timestamp = inputted.timestamp;
            assortment();
        }

        statements();
    }
};
