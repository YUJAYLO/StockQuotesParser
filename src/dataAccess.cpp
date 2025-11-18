#include "dataAccess.h"
#include <fstream>
#include <../include/utils/json.hpp>

void DataAccess::saveFormat1(const Format::Format1::ParsedRecord& record) {
    nlohmann::json jsonRecord = {
        {"symbol_short", record.symbolShort},
        {"symbol_code", record.symbolCode},
        {"ref_price", record.refPrice},
        {"up_limit", record.upLimit},
        {"down_limit", record.downLimit},
        {"timestamp", record.timestamp}
    };

    std::ofstream outFile("format1.json", std::ios::app);
    if (outFile) {
        outFile << jsonRecord.dump() << std::endl;
    }
}