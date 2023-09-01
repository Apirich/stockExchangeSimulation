#pragma once

#include "OrderBookEntry.h"
#include "OrderBook.h"

#include <vector>
#include <string>


class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        void init();
    private: 
        void printMenu();
        void printHelp();
        void printHelpCmd(std::string cmdRequest);
        void printProd();
        void printMinMax(std::string minMaxRequest, std::string products, std::string askBidRequest);
        void printAvg(std::string products, std::string askBidRequest, int stepRequest);
        void printPredict(std::string minMaxRequest, std::string products, std::string askBidRequest);
        void printTradeHistory(std::string products);
        void printTime();
        void printStep();

        std::string getUserOption();
        void processUserOption(std::string userOption);

        OrderBook orderBook{"20200601.csv"};

        //store command keywords
        std::vector<std::string> commands = {"help", "prod", "min", "max", "avg", "predict", "trade", "time", "step"};

        std::string currentTime;
};
