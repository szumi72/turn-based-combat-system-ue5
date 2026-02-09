Santa vs Elves – UE Project

This project is a transformation of a simple terminal-based game created during university programming labs
into a fully interactive Unreal Engine game with UI, animations, and real-time systems.

The original version of the game was text-based and focused purely on logic and turn handling.
This Unreal Engine version expands that concept into a visual and interactive experience
while preserving the core mechanics.

(Optionally: link to the original terminal version can be found here)
[LINK TO TERMINAL GAME]


--- CORE CONCEPT ---

The game is a turn-based battle system between two factions:
- Santa’s Army
- Elves’ Army

The player chooses a faction and manages units, gold, and progression through stages.
Battles are resolved using animations and a Quick Time Event (QTE) system.


--- MAIN FEATURES ---

• Turn-based combat system driven by animations and delegates  
• QTE-based player attacks (timing-based damage modifier)  
• Enemy counterattacks resolved automatically  
• Army management (recruiting units, losing units on death)  
• Dynamic battle arena camera  
• Log Board displaying combat and system messages  
• Health, damage, and death handling via animation notifies  
• End Game screen (win / lose conditions)


--- QUEST SYSTEM ---

The game includes a lightweight quest system:

• Units can be sent on quests  
• A unit on a quest is temporarily unavailable for combat  
• After completing a quest, the unit returns with:
  - Gold
  - OR an item that increases the unit’s damage
• Quest progress is handled automatically
• Returned rewards directly affect future battles

This system introduces strategic choices:
Should a unit fight now, or be sent on a quest for long-term benefits?


--- BATTLE FLOW ---

1. Battle starts if both armies have units
2. Active units are selected from each army
3. Player attack:
   - QTE appears
   - Success determines damage strength
4. Enemy attack:
   - Automatic, no QTE
5. Death handling:
   - Units are removed from armies
6. Battle continues until one army is defeated


--- CONTROLS ---

• Jump / Space – Confirm QTE
• Mouse – UI interaction


--- TECHNICAL NOTES ---

• Unreal Engine C++
• Heavy use of:
  - Delegates
  - Animation Notifies
  - Widgets (UMG)
• Project focuses on logic, systems, and gameplay flow rather than visuals


--- PROJECT GOAL ---

The main goal of this project was not to create a polished commercial game,
but to demonstrate how a simple terminal game concept can be expanded into
a full Unreal Engine project with:
- UI
- Animation-driven gameplay
- Player interaction
- Scalable systems

This project serves as a learning and portfolio piece.
