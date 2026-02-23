# 🚜🛡️ Unreal AI – Farmer & Turrets

![Unreal Engine](https://img.shields.io/badge/Engine-Unreal%20Engine%205-blue)
![AI](https://img.shields.io/badge/System-AI%20Controller-green)
![Behavior Tree](https://img.shields.io/badge/AI-Behavior%20Tree-purple)
![EQS](https://img.shields.io/badge/AI-EQS-orange)
![Blueprint](https://img.shields.io/badge/Language-Blueprint-yellow)
![Status](https://img.shields.io/badge/Project-Final%20Assignment-success)

Final individual project for the **Unreal AI** course.

This project implements an AI-driven ecosystem composed of:

* 🚜 **Farmer NPC** – Production logic
* 🛡️ **Turret NPC** – Defensive logic

Both systems are fully event-driven and built using Unreal Engine AI tools and custom Blueprint nodes.

---

# 📚 Table of Contents

* [Project Requirements](#-project-requirements)
* [Farmer NPC – Work Cycle](#-farmer-npc--work-cycle)

  * [States](#states)
  * [Logic & Tools](#logic--tools)
* [Turret NPC – Defense System](#-turret-npc--defense-system)

  * [States](#states-1)
  * [Senses & Logic](#senses--logic)
* [Architecture Overview](#-architecture-overview)

---

# 🏗️ Project Requirements

All Pawns:

* ✅ Controlled by an **AI Controller**
* ✅ Use **Behavior Tree & Blackboard**
* ✅ Use **AI Perception Component**
* ✅ Use **EQS (Environment Query System)**
* ✅ Include custom:

  * `BTTask`
  * `BTDecorator`
  * `BTService`

The project must be fully event-driven (no unnecessary Tick usage).

---

# 🚜 Farmer NPC – Work Cycle

The Farmer manages a complete agricultural production loop and changes mesh color depending on the current state.

---

## States

| Color     | State         | Description                                                |
| --------- | ------------- | ---------------------------------------------------------- |
| 🔴 Red    | Collect Seeds | No seeds in inventory. Searches for Seed Collection Point. |
| 🟣 Purple | Plant Seeds   | Has seeds. Searches for free soil.                         |
| 🔵 Blue   | Harvest       | Looks for mature plants (Green state).                     |
| 🟡 Yellow | Deposit       | Inventory full. Goes to storage.                           |

Mesh color changes provide immediate visual feedback of internal AI state.

---

## Logic & Tools

### 🌱 Plant Actor

* Internal growth timer
* Brown → Growing
* Green → Harvestable after X seconds

### 🔍 EQS Query

Used to:

* Find nearest Seed Point
* Find nearest valid Plant
* Filter by:

  * Distance
  * State (e.g., only Green plants)

*(Optional fallback: custom Task instead of EQS if needed)*

### 🧠 Custom Tasks

* `BTTask_Plant` → Spawns plant actor
* `BTTask_Deposit` → Empties inventory

### 🎛 Decorators

Used to validate Blackboard variables:

* `HasSeeds`
* `InventoryFull`
* `HasTargetPlant`

---

# 🛡️ Turret NPC – Defense System

The Turret protects the area and manages engagement priorities using perception.

---

## States

| Color        | State    | Description                                  |
| ------------ | -------- | -------------------------------------------- |
| ⚪ Cyan/White | Alert    | No enemy detected. Performs patrol rotation. |
| 🔴 Red       | Active   | Enemy detected. Stops patrolling and fires.  |
| ⚫ Grey       | Disabled | No logic executed.                           |

---

## Senses & Logic

### 👁 AI Perception

* Uses **Sight** sense
* Reacts only to Actors with `"Enemy"` and `Neutral` tag
* Event-driven via:

  * `OnTargetPerceptionUpdated`

⚠ No Tick usage allowed.

---

### 🎯 Blackboard Key

* `TargetActor` is updated inside AI Controller using perception events

---

### 🔄 Custom Service

`BTService_RotateToTarget`

* Active only when in "Active" state
* Smooth rotation using `RInterp To`
* Stops when target is lost

---

### 🧠 Memory System

If enemy leaves sight:

* Turret remains Active for 3 seconds
* Then returns to Alert state
* Managed via:

  * Timer
  * Decorator condition

---

# 🧩 Architecture Overview

```
AI Controller
 ├── AI Perception (Sight)
 ├── Blackboard
 └── Behavior Tree
        ├── Selector
        │     ├── Sequence (Work / Attack)
        │     └── Fallback
        ├── Custom Tasks
        ├── Custom Decorators
        └── Custom Services
```

Design principles:

* ✔ Clean Blackboard naming
* ✔ Proper Selector/Sequence usage
* ✔ Fully modular
* ✔ Event-driven
* ✔ No unnecessary Tick

---

# ⚙️ .gitignore Reminder

Ensure Unreal temporary folders are excluded:

```
Binaries/
Build/
Intermediate/
Saved/
DerivedDataCache/
```

---

# 🎓 Final Notes

This project demonstrates:

* Advanced Unreal AI architecture
* Event-driven design
* Custom Blueprint node development
* EQS integration
* AI Perception usage without Tick
* State-driven visual feedback

