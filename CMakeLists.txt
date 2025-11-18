cmake_minimum_required(VERSION 3.10)
project(StockQuotesParser)

set(CMAKE_CXX_STANDARD 17)

add_executable(StockQuotesParser
    main.cpp
    src/parser.cpp
    src/format.cpp
    src/dataAccess.cpp
    src/converter.cpp
)

target_include_directories(StockQuotesParser PRIVATE include)