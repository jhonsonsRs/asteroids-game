# Asteroids — SDL2

A simple arcade-style asteroids game written in C++ with SDL2. Shoot the asteroids before they hit you and rack up points!

## Controls

| Key | Action |
|-----|--------|
| `←` `→` `↑` `↓` | Move the ship |
| `Space` | Shoot |

## Dependencies

- SDL2, SDL2_ttf, SDL2_mixer

```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

## Compile & Run

```bash
g++ asteroids.cpp -o asteroids -lSDL2 -lSDL2_ttf -lSDL2_mixer -std=c++17
./asteroids
```

> Make sure `math-vector.h` and all asset files (`arial.ttf`, `*.wav`, `*.mp3`) are in the same directory.