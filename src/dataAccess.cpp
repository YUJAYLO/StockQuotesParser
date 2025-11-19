#include "dataAccess.h"
#include <fstream>
#include <../include/utils/json.hpp>

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