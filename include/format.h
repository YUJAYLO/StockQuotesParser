#ifndef FORMAT_H
#define FORMAT_H

#include <cstdint>
#include <vector>
#include <string>

namespace Format {

    const uint8_t ESC_CODE = 0x1B;
    const uint8_t TERMINAL_CODE[2] = {0x0D, 0x0A};

    #pragma pack(push, 1)
    struct HEADER {
        uint16_t messageLength;
        uint8_t businessType;
        uint8_t formatCode;
        uint8_t version;
        char reserved[5];
    };
    #pragma pack(pop)

    class Format1 {
    public:
        struct ParsedRecord {
            std::string symbolCode;
            std::string symbolShort;
            double refPrice;
            double upLimit;
            double downLimit;
            std::string timestamp;
        };

        void process(const std::vector<uint8_t>& record);
    };

} // namespace Format

#endif // FORMAT_H