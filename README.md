# 🏓 Ping Pong Game

A simple two-player Pong style game built in C++.  
This project showcases game loop logic, basic collision detection, paddle/ball movement, and score tracking.

---

## ✨ Features
- Single player or Two-player local gameplay
- Smooth paddle + ball movement
- Paddle/ball/wall collision with bounce
- Scoring and round reset
- Adjustable speed, paddle size, and winning score via constants

---

## 🎮 Controls

| Player | Up Key | Down Key |
|-------:|:------:|:--------:|
|   Left |   `W`  |   `S`    |
|  Right |  `↑`   |   `↓`    |

> You can change keys in `main.cpp`.

---

## 🧪 How It Works (quick overview)
- **Game loop:** process input → update world (positions, collisions, scoring) → render
- **Collision:** AABB checks for ball vs. paddles/walls; reflect velocity on hit
- **Scoring:** ball crosses left/right bounds → increment score → center ball → serve
- **Timing:** use frame delta time to keep speeds consistent across machines

---

## 📸 Screenshot 

![Screenshot 1](Screenshot%202025-11-03%20001919.png)
![Screenshot 2](Screenshot%202025-11-03%20001938.png)

---

## 🗺️ Roadmap
- [ ] Sound effects
- [ ] Single-player mode (simple AI)
- [ ] Power-ups
- [ ] Gamepad support
- [ ] Settings menu + persistent config

