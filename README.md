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
```
<p align="center">
  <img src="Screenshot%202025-11-03%20001919.png" width="600" alt="Ping Pong Game Screenshot 1">
</p>

<p align="center">
  <img src="Screenshot%202025-11-03%20001938.png" width="600" alt="Ping Pong Game Screenshot 2">
</p>

---

## 🗺️ Roadmap
- [ ] Sound effects
- [ ] Single-player mode (simple AI)
- [ ] Power-ups
- [ ] Gamepad support
- [ ] Settings menu + persistent config


## 🙌 Acknowledgements
Inspired by the original **Pong** (Atari, 1972). Thanks to SFML for a simple C++ multimedia API.



