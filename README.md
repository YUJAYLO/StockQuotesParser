# StockQuotesParser

StockQuotesParser is a C++ tool designed to parse TSE (Taiwan Stock Exchange) stock quote binary files. It reads binary data, processes it according to specific formats (e.g., Format1, Format6), and outputs the parsed information.

## Features

- **Binary Parsing**: Efficiently reads and processes binary stock quote data.
- **Format Support**: Handles multiple data formats including Format1 and Format6.
- **Robust Error Handling**: Includes validation and error reporting for malformed data.

## Requirements

- C++17 compatible compiler (e.g., g++, clang++)
- Make build system

## Build Instructions

To build the project, simply run `make` in the root directory:

```bash
make
```

This will compile the source code and generate the `StockQuotesParser.exe` executable.

To clean the build artifacts:

```bash
make clean
```

## Usage

After building, you can run the parser using the following command:

```bash
./StockQuotesParser.exe
```

By default, the parser expects the input file to be located at `./doc/Tse.bin`.

## Project Structure

```text
.
├── build/                  # Intermediate build files
├── data/                   # Output data directory
├── doc/                    # Documentation and input files
│   ├── Tse.bin
│   └── ...
├── include/                # Header files
│   ├── utils/
│   ├── dataAccess.h
│   ├── format.h
│   └── parser.h
├── src/                    # Source files
│   ├── dataAccess.cpp
│   ├── format.cpp
│   └── parser.cpp
├── main.cpp                # Main entry point
├── Makefile                # Build configuration
└── README.md               # Project documentation
```
