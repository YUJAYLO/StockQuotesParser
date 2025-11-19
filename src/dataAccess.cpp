#include "dataAccess.h"
#include <fstream>
#include <../include/utils/json.hpp>

using json = nlohmann::json;
const std::string dataPath = "build/";

void DataAccess::saveFormat1(const Format::Format1::ParsedRecord& record) {
    json jsonRecord = {
        {"symbol_code", record.symbolCode},
        {"symbol_short", record.symbolShort},
        {"ref_price", record.refPrice},
        {"up_limit", record.upLimit},
        {"down_limit", record.downLimit},
        {"timestamp", record.timestamp}
    };

    std::ofstream outFile(dataPath + "format1.json", std::ios::app);
    if (outFile) {
        outFile << jsonRecord.dump() << std::endl;
    }
}