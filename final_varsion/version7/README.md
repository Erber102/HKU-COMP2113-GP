# HKU-COMP2113-GP

## Description

In this game, you will be a magic bard who tries to survive in the end times. All you have is a flute that can play beautiful music and a backpack. During the day phase, you can explore and scavenge. During the night phase, you can trade and transact.

---
## Team members

Lang Ruifan

Luo Jian

Peng Yicheng

Sun Jiawei

Tian Congxi

Zhang Wenyi

---
## Features implemented

- **Random Events:** (Event system) Trigger random events during exploration including finding resources, encountering enemies, special discoveries to make each playthrough unpredictable.
- **Data Structures:** Using structs to store player data, items, customers, and market trends; vectors for inventory management and list operations.
- **Dynamic Memory Management:** (in Player.cpp and itemdatabase.cpp) Create and delete item objects using **`new`** and **`delete`** when adding/removing items from inventory.
- **File Input/Output:** (in SaveSystem.cpp) Save and load game progress including player status, inventory, market trends, and current day to **`savegame.dat`**.
- **Multiple Files:** Modular design with separate header and implementation files for each system (Game, Player, Item, Combat, Market, etc.).
- **Multiple Difficulty Levels:** Three difficulty modes (**Easy**, **Normal**, **Hard**) affecting starting resources, survival costs, and combat difficulty.

---

## How to play

1. **Start the Game:**
   - Launch the game and choose your difficulty level (**Easy**, **Normal**, or **Hard**).
   - The game starts at Day 1, with each day divided into day and night phases.

2. **Control of Combat System:**

Use the number keys 1-7 to play notes. Different notes have different effects. Certain combinations of notes can form runes that have much more powerful effects!

---

## For developer

For the combat system, include `"combat.h"` and `"utils.h"`,

use 

```c++
auto combat=make_unique<Combat>("");
```

to recreate the player for combat, and 

```c++
combat->fightEnemy(enemyIndex);
```

to start a fight with enemy (currently 0~3).

---

## **Compilation and Execution**

### **Environment**
- **Operating System:** Linux/Unix systems
- **Compiler:** Any C++ compiler that supports C++11 or higher (e.g., `g++`)
- **Dependencies:** None (fully implemented with standard C++ libraries)

### **Steps to Compile**



---

# **Project Structure**

Here is the directory structure of the project files:

```powershell
.
├── src/
│   ├── core/
│   │   ├── main.cpp
│   │   ├── Game.cpp
│   │   ├── InputStream.cpp
│   │   ├── UISystem.cpp
│   │   └── panel.cpp
│   ├── Player/
│   │   └── Player.cpp
│   ├── Item/
│   │   ├── item.cpp
│   │   └── itemdatabase.cpp
│   ├── Save/
│   │   └── SaveSystem.cpp
│   ├── Map/
│   │   ├── DayPhase.cpp
│   │   ├── Event.cpp
│   │   └── Map.cpp
│   ├── Night/
│   │   ├── Customer.cpp
│   │   ├── Market.cpp
│   │   └── NightPhase.cpp
│   └── Combat/
│       ├── CombatPlayer.cpp
│       ├── combat.cpp
│       ├── enemy.cpp
│       ├── character.cpp
│       └── utils.cpp
├── headers/ (corresponding .h files for each module)
└── Makefile
```

---
