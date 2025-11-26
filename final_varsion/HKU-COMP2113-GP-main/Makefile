# compiler setting
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++11 -Isrc/Core -Isrc/Player -Isrc/Map -Isrc/Combat -Isrc/Night -Isrc/Item -Isrc/Save
TARGET := bin/game

# list all source files
SOURCES := src/Core/main.cpp \
           src/Core/Game.cpp \
           src/Player/Player.cpp \
           src/Core/InputSystem.cpp \
           src/Core/UISystem.cpp \
           src/Item/item.cpp \
           src/Item/itemdatabase.cpp \
           src/Save/SaveSystem.cpp \
           src/Core/panel.cpp \
           src/Map/DayPhase.cpp \
           src/Map/Event.cpp \
           src/Map/Map.cpp \
           src/Night/Customer.cpp \
           src/Night/Market.cpp \
           src/Night/NightPhase.cpp \
           src/Combat/CombatPlayer.cpp \
           src/Combat/combat.cpp \
           src/Combat/enemy.cpp \
           src/Combat/character.cpp \
           src/Combat/utils.cpp

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
