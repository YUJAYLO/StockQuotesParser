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

} // namespace Format

#endif // FORMAT_H