#include "project2.hpp"
#include <getopt.h>


int main(int argc, char* argv[]){
    int option_index = 0;
    int verbose_flag = 0, median_flag = 0, trader_info_flag = 0, time_travelers_flag = 0;

    // Define the long options
    static struct option long_options[] = {
        {"verbose", no_argument, &verbose_flag, 1},
        {"median", no_argument, &median_flag, 1},
        {"trader_info", no_argument, &trader_info_flag, 1},
        {"time_travelers", no_argument, &time_travelers_flag, 1},
        {0, 0, 0, 0} // End of options
    };

    // Parse options using getopt_long
    int c;
    while ((c = getopt_long(argc, argv, "vmit", long_options, &option_index)) != -1) {
        switch (c) {
            case 0:
                // A flag was set, no need to do anything here
                break;
            case 'v':
                verbose_flag = 1;
                break;
            case 'm':
                median_flag = 1;
                break;
            case 'i':
                trader_info_flag = 1;
                break;
            case 't':
                time_travelers_flag = 1;
                break;
            case '?':
                // Invalid option, show usage
                return EXIT_FAILURE;
            default:
                break;
        }
    }

    string what_mode;

    // After parsing, you can check the flags and run the corresponding logic
    if (verbose_flag) {
        std::cout << "Verbose output enabled.\n";
        what_mode = "v";
    }
    if (median_flag) {
        std::cout << "Median output enabled.\n";
        what_mode = "m";
    }
    if (trader_info_flag) {
        std::cout << "Trader info output enabled.\n";
        what_mode = "i";
    }
    if (time_travelers_flag) {
        std::cout << "Time travelers output enabled.\n";
        what_mode = "tl";
    }



    string comment; 
    string mode_label;
    string mode; 
    string num_traders_label;
    uint32_t num_traders;
    string num_stockers_label;
    uint32_t num_stocks; 



    getline(cin, comment);
    cin >> mode_label;
    cin >> mode;
    cin >> num_traders_label;
    cin >> num_traders;
    cin >> num_stockers_label;
    cin >> num_stocks;
    
    StockTrading classi(what_mode, num_stocks, num_traders);
    classi.reading_input();
    
    





}