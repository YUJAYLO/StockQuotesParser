CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./include
LDFLAGS = 

BUILD_DIR = build

SRCS = main.cpp src/dataAccess.cpp src/format.cpp src/parser.cpp
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
TARGET = StockQuotesParser

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.cpp
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	if exist $(TARGET).exe del $(TARGET).exe
