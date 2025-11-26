#ifndef DATAACCESS_H
#define DATAACCESS_H

#include "format.h"

class DataAccess {
public:
    static void saveFormat1(const Format::Format1::ParsedRecord& record);
    static void saveFormat6(const Format::Format6::ParsedRecord& record);
};

#endif // DATAACCESS_H
