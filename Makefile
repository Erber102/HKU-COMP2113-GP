# compiler setting
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++11 -I. -IMay_system -INight_market_Joey
TARGET := game

# list all source files
SOURCES := main.cpp \
           Game.cpp \
           Player.cpp \
           combat.cpp \
           monster.cpp \
           panel.cpp \
           May_system/DayPhase.cpp \
           May_system/Event.cpp \
           May_system/Map.cpp

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