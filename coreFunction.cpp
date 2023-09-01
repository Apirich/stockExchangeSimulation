//**For CHECKING FOR PLAGIARISM ONLY**//


// print welcome
void MerkelMain::printMenu()
{
    std::cout << "Please enter a command, or help for a list of commands." << std::endl;
}


// print help
void MerkelMain::printHelp()
{
    std::cout << "The available commands are help, help <cmd>, prod, min/max, avg, predict, trade, time, step." << std::endl;
}


//print help <cmd>
void MerkelMain::printHelpCmd(std::string cmdRequest)
{
    if(cmdRequest == commands[1]) //print help prod
    {
        std::cout << "prod -> List of available products." << std::endl;
    }
    else if(cmdRequest == commands[2] ||
            cmdRequest == commands[3]) //print help min/max
    {
        std::cout << "min/max ETH/BTC bid/ask -> Min/max ETH/BTC bid/ask of current time step." << std::endl;
    }
    else if(cmdRequest == commands[4]) //print help avg
    {
        std::cout << "avg ETH/BTC bid/ask 3 -> Average ETH/BTC bid/ask over the last 3 time steps." << std::endl;
    }
    else if(cmdRequest == commands[5]) //print help predict
    {
        std::cout << "predict min/max ETH/BTC bid/ask -> Preditc min/max ETH/BTC bid/ask of the next time step." << std::endl;
    }
    else if(cmdRequest == commands[6]) //print help trade history
    {
        std::cout << "trade history ETH/BTC -> Trade history of ETH/BTC of current time step." << std::endl;
    }
    else if(cmdRequest == commands[7]) //print help time
    {
        std::cout << "time -> Current time step." << std::endl;
    }
    else if(cmdRequest == commands[8]) //print help step
    {
        std::cout << "step -> Move to the next time step." << std::endl;
    }
    else //print help invalid
    {
        std::cout << "Invalid command!" << std::endl;
    }   
}


//print prod
void MerkelMain::printProd()
{
    //all available products
    for(std::string const& p: orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
    }
}


//print min/max
void MerkelMain::printMinMax(std::string minMaxRequest, std::string products, std::string askBidRequest)
{
    //product name
    std::cout << "Product: " << products << std::endl;

    //min bid/ask
    if(minMaxRequest == "min")
    {
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(askBidRequest),
                                                                products,
                                                                currentTime);
        if(entries.size() != 0) //check if there is any ask or bid as requested in user command
        {
            std::cout << askBidRequest << " seen: " << entries.size() << std::endl;
            std::cout << "Min: " << askBidRequest << " " << OrderBook::getLowPrice(entries) << std::endl;
        }
        else //no bid/ask as requested in user command
        {
            std::cout << "There is no " << askBidRequest << " at this timestep" << std::endl;
        }
    }

    //max bid/ask
    if(minMaxRequest == "max")
    {
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(askBidRequest),
                                                                products,
                                                                currentTime);
        if(entries.size() != 0) //check if there is any ask or bid as requested in user command
        {
            std::cout << askBidRequest << " seen: " << entries.size() << std::endl;
            std::cout << "Max: " << askBidRequest << " " << OrderBook::getHighPrice(entries) << std::endl;
        }
        else //no bid/ask as requested in user command
        {
            std::cout << "There is no " << askBidRequest << " at this timestep" << std::endl;
        }
    }
}


//print avg
void MerkelMain::printAvg(std::string products, std::string askBidRequest, int stepRequest)
{   
    //passed time steps
    std::vector<std::string> paStep = orderBook.passedTimestamp();
    //sum average of passed time steps
    double sum = 0;
    //subtract step without bid/ask
    double stepWithData = static_cast<double>(stepRequest);

    if(0 < static_cast<double>(stepRequest) &&
       static_cast<double>(stepRequest) <= paStep.size() &&
       paStep.size() != 0) //check if number of passed time steps in user command is valid
    {
        //loop through all passed time steps
        for(int i = paStep.size() - 1; i >= paStep.size() - static_cast<double>(stepRequest); --i)
        {
            std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(askBidRequest),
                                                                    products,
                                                                    paStep[i]);

            std::cout << "Timestep: " << paStep[i] << std::endl;

            if(entries.size() != 0) //check if there is any ask or bid in each requested time step
            {
                std::cout << askBidRequest << " seen: " << entries.size() << std::endl;
                std::cout << "Avg " << askBidRequest << ": " << OrderBook::getAveragePrice(entries) << std::endl;
                sum+= OrderBook::getAveragePrice(entries);
            }
            else //no bid/ask in each requested time step
            {
                stepWithData--;
                std::cout << "There is no " << askBidRequest << " at this timestep" << std::endl;
            }
        }
        std::cout << "The average " << products << " " << askBidRequest << " price over the last " << stepRequest << " timestamp was " << sum/stepWithData << std::endl;
    }
    else //invalid requested time steps
    {
        std::cout << "Incorrect request passed Timestep: " << stepRequest << " Current pass timestep is " << paStep.size() << std::endl;
    }
}


//print predict
void MerkelMain::printPredict(std::string minMaxRequest, std::string products, std::string askBidRequest)
{
    //store predict value for each case
    double predictVal;

    std::vector<OrderBookEntry> entriesBid = orderBook.getOrders(OrderBookType::bid,
                                                                products,
                                                                currentTime);
    std::vector<OrderBookEntry> entriesAsk = orderBook.getOrders(OrderBookType::ask,
                                                                products,
                                                                currentTime);

    //predict min bid
    if(minMaxRequest == "min" && askBidRequest == "bid")
    {
        //when current time step has both asks and bids
        if(entriesBid.size() != 0 && entriesAsk.size() !=0)
        {
            std::cout << "Min bid is: " << OrderBook::getLowPrice(entriesBid) << std::endl;
            std::cout << "Avg bid is: " << OrderBook::getAveragePrice(entriesBid) << std::endl;

            std::cout << "Min ask is: " << OrderBook::getLowPrice(entriesAsk) << std::endl;

            predictVal = ((OrderBook::getLowPrice(entriesBid) + OrderBook::getLowPrice(entriesAsk))/2 + OrderBook::getAveragePrice(entriesBid))/2;
            std::cout << "The predict min " << askBidRequest << " is: " << predictVal << std::endl;
        }
        //when current time step has only bids
        else if(entriesBid.size() != 0 && entriesAsk.size() == 0)
        {
            std::cout << "Min bid is: " << OrderBook::getLowPrice(entriesBid) << std::endl;
            std::cout << "Avg bid is: " << OrderBook::getAveragePrice(entriesBid) << std::endl;
            std::cout << "Max bid is: " << OrderBook::getHighPrice(entriesBid) << std::endl;

            std::cout << "There is no ask for current timestep." << std::endl;

            predictVal = OrderBook::getHighPrice(entriesBid)*105/100;
            std::cout << "The predict min " << askBidRequest << " is: " << predictVal << std::endl;
        }
        //when current time step has only asks
        else if(entriesBid.size() == 0 && entriesAsk.size() !=0)
        {
            std::cout << "Min ask is: " << OrderBook::getLowPrice(entriesAsk) << std::endl;

            std::cout << "There is no bid for current timestep." << std::endl;

            predictVal = OrderBook::getLowPrice(entriesAsk)*80/100;
            std::cout << "The predict min " << askBidRequest << " is: " << predictVal << std::endl;
        }
        //when current time has neither asks nor bids of requested product
        else
        {
            std::cout << "There is no bid or ask for " << products << " at current timestep." << std::endl;
        }
    }
    //predict min ask
    if(minMaxRequest == "min" && askBidRequest == "ask")
    {
        //when current time step has both asks and bids
        if(entriesBid.size() != 0 && entriesAsk.size() !=0)
        {
            std::cout << "Min bid is: " << OrderBook::getLowPrice(entriesBid) << std::endl;

            std::cout << "Min ask is: " << OrderBook::getLowPrice(entriesAsk) << std::endl;
            std::cout << "Avg ask is: " << OrderBook::getAveragePrice(entriesAsk) << std::endl;

            predictVal = ((OrderBook::getLowPrice(entriesBid) + OrderBook::getLowPrice(entriesAsk))/2 + OrderBook::getAveragePrice(entriesAsk))/2;
            std::cout << "The predict min " << askBidRequest << " is: " << predictVal << std::endl;
        }
        //when current time step has only bids
        else if(entriesBid.size() != 0 && entriesAsk.size() == 0)
        {
            std::cout << "Min bid is: " << OrderBook::getLowPrice(entriesBid) << std::endl;
            std::cout << "Avg bid is: " << OrderBook::getAveragePrice(entriesBid) << std::endl;
            std::cout << "Max bid is: " << OrderBook::getHighPrice(entriesBid) << std::endl;

            std::cout << "There is no ask for current timestep." << std::endl;

            predictVal = OrderBook::getHighPrice(entriesBid)*120/100;
            std::cout << "The predict min " << askBidRequest << " is: " << predictVal << std::endl;
        }
        //when current time step has only asks
        else if(entriesBid.size() == 0 && entriesAsk.size() !=0)
        {
            std::cout << "Min ask is: " << OrderBook::getLowPrice(entriesAsk) << std::endl;

            std::cout << "There is no bid for current timestep." << std::endl;

            predictVal = OrderBook::getLowPrice(entriesAsk)*95/100;
            std::cout << "The predict min " << askBidRequest << " is: " << predictVal << std::endl;
        }
        //when current time has neither asks nor bids of requested product
        else
        {
            std::cout << "There is no bid or ask for " << products << " at current timestep." << std::endl;
        }
    }
    //predict max bid
    if(minMaxRequest == "max" && askBidRequest == "bid")
    {
        //when current time step has both asks and bids
        if(entriesBid.size() != 0 && entriesAsk.size() !=0)
        {
            std::cout << "Max bid is: " << OrderBook::getHighPrice(entriesBid) << std::endl;

            std::cout << "Max ask is: " << OrderBook::getHighPrice(entriesAsk) << std::endl;
            std::cout << "Avg bid is: " << OrderBook::getAveragePrice(entriesBid) << std::endl;

            predictVal = ((OrderBook::getHighPrice(entriesBid) + OrderBook::getHighPrice(entriesAsk))/2 + OrderBook::getAveragePrice(entriesBid))/2;
            std::cout << "The predict max " << askBidRequest << " is: " << predictVal << std::endl;
        }
        //when current time step has only bids
        else if(entriesBid.size() != 0 && entriesAsk.size() == 0)
        {
            std::cout << "Min bid is: " << OrderBook::getLowPrice(entriesBid) << std::endl;
            std::cout << "Avg bid is: " << OrderBook::getAveragePrice(entriesBid) << std::endl;
            std::cout << "Max bid is: " << OrderBook::getHighPrice(entriesBid) << std::endl;

            std::cout << "There is no ask for current timestep." << std::endl;

            predictVal = OrderBook::getHighPrice(entriesBid)*120/100;
            std::cout << "The predict max " << askBidRequest << " is: " << predictVal << std::endl;
        }
        //when current time step has only asks
        else if(entriesBid.size() == 0 && entriesAsk.size() !=0)
        {
            std::cout << "Min ask is: " << OrderBook::getLowPrice(entriesAsk) << std::endl;

            std::cout << "There is no bid for current timestep." << std::endl;

            predictVal = OrderBook::getLowPrice(entriesAsk)*95/100;
            std::cout << "The predict max " << askBidRequest << " is: " << predictVal << std::endl;
        }
        //when current time has neither asks nor bids of requested product
        else
        {
            std::cout << "There is no bid or ask for " << products << " at current timestep." << std::endl;
        }
    }
    //predict max ask
    if(minMaxRequest == "max" && askBidRequest == "ask")
    {
        //when current time step has both asks and bids
        if(entriesBid.size() != 0 && entriesAsk.size() !=0)
        {
            std::cout << "Max bid is: " << OrderBook::getHighPrice(entriesBid) << std::endl;

            std::cout << "Max ask is: " << OrderBook::getHighPrice(entriesAsk) << std::endl;
            std::cout << "Avg bid is: " << OrderBook::getAveragePrice(entriesBid) << std::endl;

            predictVal = ((OrderBook::getHighPrice(entriesBid) + OrderBook::getHighPrice(entriesAsk))/2 + OrderBook::getAveragePrice(entriesAsk))/2;
            std::cout << "The predict max " << askBidRequest << " is: " << predictVal << std::endl;
        }
        //when current time step has only bids
        else if(entriesBid.size() != 0 && entriesAsk.size() == 0)
        {
            std::cout << "Min bid is: " << OrderBook::getLowPrice(entriesBid) << std::endl;
            std::cout << "Avg bid is: " << OrderBook::getAveragePrice(entriesBid) << std::endl;
            std::cout << "Max bid is: " << OrderBook::getHighPrice(entriesBid) << std::endl;

            std::cout << "There is no ask for current timestep." << std::endl;

            predictVal = OrderBook::getHighPrice(entriesBid)*135/100;
            std::cout << "The predict max " << askBidRequest << " is: " << predictVal << std::endl;
        }
        //when current time step has only asks
        else if(entriesBid.size() == 0 && entriesAsk.size() !=0)
        {
            std::cout << "Min ask is: " << OrderBook::getLowPrice(entriesAsk) << std::endl;

            std::cout << "There is no bid for current timestep." << std::endl;

            predictVal = OrderBook::getHighPrice(entriesAsk)*95/100;
            std::cout << "The predict max " << askBidRequest << " is: " << predictVal << std::endl;
        }
        //when current time has neither asks nor bids of requested product
        else
        {
            std::cout << "There is no bid or ask for " << products << " at current timestep." << std::endl;
        }
    }
}


//print trade history of requested product of current time step
void MerkelMain::printTradeHistory(std::string products)
{
    std::vector<OrderBookEntry> printSales = orderBook.matchAsksToBids(products, currentTime);

    //total of trade
    std::cout << "There are " << printSales.size() << " trades in this timestep." << std::endl;
    //display trade list with price and amount
    for(OrderBookEntry& p: printSales)
    {
        std::cout << "Trade price: " << p.price << " Trade amount: " << p.amount << std::endl;
    }
}


//print current time
void MerkelMain::printTime()
{
    std::cout << "Current time is " << currentTime << std::endl;
}

//move to the next time step
void MerkelMain::printStep()
{
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "Successful moved to time step: " << currentTime << std::endl;
}


//get user command
std::string MerkelMain::getUserOption()
{
    std::string userOption = "";
    std::string line;
    std::cout << "Type a command or help" << std::endl;
    std::getline(std::cin, userOption);
    return userOption;
}


//process user command
void MerkelMain::processUserOption(std::string userOption)
{
    //tokenise user command
    std::vector<std::string> tokens = CSVReader::tokenise(userOption, ' ');

    //print help, prod, time, step
    if(tokens.size() == 1)
    {
        if(tokens[0] == "help") //print help
        {
            printHelp(); 
        }
        else if(tokens[0] == "prod") //print prod
        {
            printProd();
        }
        else if(tokens[0] == "time") //print time
        {
            printTime();
        }
        else if(tokens[0] == "step") //print step
        {
            printStep();
        }
        else
        {
            std::cout << "Invalid command!" << std::endl;
        }
    }
    //print help <cmd>
    else if(tokens.size() == 2) 
    {
        if(tokens[0] == "help")
        {
            printHelpCmd(tokens[1]);
        }
        else
        {
            std::cout << "Invalid command!" << std::endl;
        }
    }
    //print min, max, trade history
    else if(tokens.size() == 3) 
    {
        //check if product command is in products list
        bool checkVal = false;
        for(std::string const& p: orderBook.getKnownProducts())
        {
            if(tokens[1] == p ||
               tokens[2] == p)
            {
                checkVal = true;
            }
        } 
        
        if((tokens[0] == "min" || tokens[0] == "max") &&
            (tokens[2] == "ask" || tokens[2] == "bid")&&
            checkVal) //print min, max
        {
            printMinMax(tokens[0], tokens[1], tokens[2]);
        }
        else if(tokens[0] == "trade" &&
                tokens[1] == "history" &&
                checkVal) //print trade history
        {
            printTradeHistory(tokens[2]);
        }
        else
        {
            std::cout << "Invalid command!" << std::endl;
        }
    } 
    //print avg, predict
    else if(tokens.size() == 4) 
    {
        //check if product command is in products list
        bool checkVAl = false;
        for(std::string const& p: orderBook.getKnownProducts())
        {
            if(tokens[1] == p ||
               tokens[2] == p)
            {
                checkVAl = true;
            }
        }
    
        if((tokens[2] == "ask" || tokens[2] == "bid") &&
            tokens[0] == "avg" &&
            checkVAl) //print avg
        {
            try
            {
                printAvg(tokens[1], tokens[2], std::stoi(tokens[3]));
            }
            catch(const std::exception& e)
            {
                std::cout << "Number of timestep has to be an interger number!" << std::endl;
            }
        }
        else if((tokens[1] == "min" || tokens[1] == "max") &&
                (tokens[3] == "ask" || tokens[3] == "bid") &&
                tokens[0] == "predict" &&
                checkVAl) //print predict
        {
            printPredict(tokens[1], tokens[2], tokens[3]);
        }  
        else
        {
            std::cout << "Invalid command!" << std::endl;
        }
    }
    //invalid command
    else
    {
        std::cout << "Invalid command!" << std::endl;
    }
}


 //OPTIMISE EXCHANGE CODE -------- Check if max bid is lower than min ask --> if lower, no need to loop through the orders 
    if(bids[0].price < asks[0].price)
    {
        std::cout << " OrderBook::matchAsksToBids max bid is lower than min ask -> no sales in this time step." << std::endl;
        return sales;
    }