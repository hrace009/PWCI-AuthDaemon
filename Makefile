CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pthread
LDLIBS := -lmariadb

SOURCES := main.cpp AuthServer.cpp AuthClient.cpp Settings.cpp Database.cpp Utils.cpp
OBJECTS := $(SOURCES:.cpp=.o)
TARGET := gauthd

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
