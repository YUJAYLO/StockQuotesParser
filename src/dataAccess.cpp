#include "..\include\dataAccess.h"
#include <fstream>
#include <vector>
#include <string>
#include "..\include\utils\json.hpp"

using json = nlohmann::ordered_json;
const std::string dataPath = "data/";

void DataAccess::saveFormat1(const Format::Format1::ParsedRecord& record) {
    json jsonRecord = {
        {"stock_id", record.stockID},
        {"stock_name", record.stockName},
        {"ref_price", record.refPrice},
        {"up_limit", record.upLimit},
        {"down_limit", record.downLimit}
    };

    std::ofstream outFile(dataPath + "format1.json", std::ios::app);
    if (outFile) {
        outFile << jsonRecord.dump() << std::endl;
    }
}

void DataAccess::saveFormat6(const Format::Format6::ParsedRecord& record) {
    std::string filename = dataPath + "format6.csv";
    bool fileExists = std::ifstream(filename).good();
    std::ofstream outFile(filename, std::ios::app);

    if (outFile) {
        if (!fileExists) {
            outFile << "StockID,Time,TransactionPrice,TransactionVol";
            for (int i = 1; i <= 5; ++i) outFile << ",BuyPrice" << i << ",BuyVol" << i;
            for (int i = 1; i <= 5; ++i) outFile << ",SellPrice" << i << ",SellVol" << i;
            outFile << "\n";
        }

        outFile << record.stockID << "," << record.matchingTime << "," 
                << record.transactionPrice << "," << record.transactionVol;
        
        // Write 5 levels of Buy Quotes
        for (size_t i = 0; i < 5; ++i) {
            if (i < record.buyQuotes.size()) {
                outFile << "," << record.buyQuotes[i].first << "," << record.buyQuotes[i].second;
            } else {
                outFile << ",,";
            }
        }

        // Write 5 levels of Sell Quotes
        for (size_t i = 0; i < 5; ++i) {
            if (i < record.sellQuotes.size()) {
                outFile << "," << record.sellQuotes[i].first << "," << record.sellQuotes[i].second;
            } else {
                outFile << ",,";
            }
        }
        outFile << "\n";
    }
}