# compiler setting
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++11 -I. -IMap_system -INight_market_Joey -ICombat_system
TARGET := game

# list all source files
SOURCES := main.cpp \
           Game.cpp \
           Player.cpp \
           item.cpp \
           itemdatabase.cpp \
           SaveSystem.cpp \
           Combat_system/combat.cpp \
           panel.cpp \
           Map_system/DayPhase.cpp \
           Map_system/Event.cpp \
           Map_system/Map.cpp \
           Night_market_Joey/Customer.cpp \
           Night_market_Joey/Market.cpp \
           Night_market_Joey/NightPhase.cpp

# object list
OBJECTS := $(addprefix build/,$(SOURCES:.cpp=.o))

# main rules
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# general compile rule
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean command
clean:
	rm -rf build $(TARGET)

# run command
run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
