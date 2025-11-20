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

    #pragma pack(push, 1)
    template<int N>
    struct RawRecord {
        // ESC (1)
        uint8_t esc;
        // HEADER (9)
        HEADER header;
        // BODY (N)
        uint8_t body[N];
        // checksum (1)
        uint8_t checksum;
        // terminal (2)
        uint16_t terminal;
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

        static constexpr size_t BODY_SIZE = 101;

        uint16_t _messageLength = 0x0114;
        uint8_t _businessType = 0x01;
        uint8_t _version = 0x09;
        BODY _body;

    public:
        Format1(const std::vector<uint8_t>& record);
        void process();
    };

} // namespace Format

#endif // FORMAT_H