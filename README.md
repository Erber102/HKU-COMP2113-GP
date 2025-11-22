# HKU-COMP2113-GP

## Team members

Lang Ruifan

Luo Jian

Peng Yicheng

Sun Jiawei

Tian Congxi

Zhang Wenyi

## How to play

Combat system:

Use the number keys 1-7 to play notes. Different notes have different effects. Certain combinations of notes can form runes that have much more powerful effects!

## Features implemented

## Description

In this game, you will be a magic bard who tries to survive in the end times. All you have is a flute that can play beautiful music and a backpack. During the day phase, you can explore and scavenge. During the night phase, you can trade and transact.

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