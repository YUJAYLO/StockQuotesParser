#ifndef FORMAT_H
#define FORMAT_H

#include <cstdint>
#include <vector>
#include <string>

namespace Format {

    const uint8_t ESC_CODE = 0x1B;
    const uint16_t TERMINAL_CODE = 0x0D0A;

    #pragma pack(push, 1)
    struct HEADER {
        uint8_t messageLength[2];
        uint8_t businessType;
        uint8_t formatCode;
        uint8_t version;
        uint8_t serialNuum[4];
    };
    #pragma pack(pop)


    class Format1 {
    public:
        struct ParsedRecord {
            std::string stockID;
            std::string stockName;
            std::string refPrice;
            std::string upLimit;
            std::string downLimit;
        };

    private:
        #pragma pack(push, 1)
        struct BODY {
            char stockID[6];
            char stockName[16];
            uint8_t stockDataReserved1[8];
            uint8_t refPrice[5];
            uint8_t upLimit[5];
            uint8_t downLimit[5];
            uint8_t stockDataReserved2[9];
            uint8_t warrantDataReserved[39];
            uint8_t otherDataReserved[7];
            uint8_t routeMark;
        };
        #pragma pack(pop)

        const uint16_t _messageLength = 0x0114;
        const uint8_t _businessType = 0x01;
        const uint8_t _version = 0x09;

    public:
        Format1(const HEADER* header);
        void process(const std::vector<uint8_t>& body);
    };

    class Format6 {
    public:
        struct ParsedRecord {
            std::string stockID;
            std::string matchingTime;
            
            // Transaction Data
            std::string transactionPrice;
            std::string transactionVol;

            // Quotes (up to 5 levels)
            std::vector<std::pair<std::string, std::string>> buyQuotes;
            std::vector<std::pair<std::string, std::string>> sellQuotes;
        };

    private:
        #pragma pack(push, 1)
        struct BODY {
            char stockID[6];
            uint8_t matchingTime[6];
            uint8_t revealItemNote;
            uint8_t priceLimitNote;
            uint8_t stateNote;
            uint8_t totalTransactionVol[4];
            uint8_t realTimeQuotes[99];
        };
        #pragma pack(pop)

        const uint8_t _businessType = 0x01;
        const uint8_t _version = 0x04;

    public:
        Format6(const HEADER* header);
        void process(const std::vector<uint8_t>& body);
    };

} // namespace Format

#endif // FORMAT_H