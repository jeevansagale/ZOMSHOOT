# ZOMSHOOT

A 2D zombie-survival shooter built in C++ with Raylib. Survive incoming enemies, aim and shoot, manage health, and keep your score alive for as long as possible.

## Gameplay Preview

<p align="center">
  <img src="Images/Screenshot%202026-07-03%20084905.png" width="32%" alt="ZOMSHOOT gameplay 1" />
  <img src="Images/Screenshot%202026-07-03%20084929.png" width="32%" alt="ZOMSHOOT gameplay 2" />
  <img src="Images/Screenshot%202026-07-03%20085016.png" width="32%" alt="ZOMSHOOT gameplay 3" />
</p>

## Features

- Top-down shooter gameplay.
- Player movement and shooting.
- Zombie enemies and collision detection.
- Health and score tracking.
- Sprite-based rendering.
- Menu and game-over screens.

## Controls

| Action | Input |
|---|---|
| Move | W, A, S, D |
| Aim | Mouse |
| Shoot | Left mouse button |
| Exit / menu | Esc |

## Technology

- C++
- Raylib
- OpenGL (through Raylib)

## Build and Run

The repository includes a Windows executable and runtime DLLs. It does not currently include a portable build configuration such as CMake or Make.

To build from source, install a C++ compiler and configure Raylib for your platform, then compile `main.cpp` and link the Raylib dependencies.

## Repository Layout

```text
ZOMSHOOT/
├── Assets/
├── Images/
├── main.cpp
└── README.md
```

## License

No license file is currently included. Unless a license is added, the repository should be treated as source available for viewing, not as granting general permission to reuse or redistribute the code.
