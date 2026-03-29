# verify-autoresearch

Autonomous AI research agent that iteratively optimizes waypoints for a "Ball on Line" tracking game, inspired by [karpathy/autoresearch](https://github.com/karpathy/autoresearch).

## How It Works

1. A target curve is rendered on screen
2. A ball follows a path defined by waypoints (JSON)
3. Accuracy = % of the run where the ball touches the curve
4. An AI agent reads `program.md` and autonomously experiments with different waypoint strategies
5. Improvements are committed and pushed; failures are logged and discarded

## Quick Start

```bash
# Compile
g++ -std=c++17 -o game game.cpp $(pkg-config --cflags --libs sdl2)
g++ -std=c++17 -o game_auto game_auto.cpp $(pkg-config --cflags --libs sdl2)

# Play manually
./game

# Run automated with waypoints
./game_auto waypoints.json

# Run experiment (outputs accuracy number)
./run_experiment.sh waypoints.json

# Start the AI research loop
# Point Claude Code at this repo and say: "read program.md and start experimenting"
```

## Files

| File | Purpose |
|------|---------|
| `game.cpp` | Interactive SDL2 game |
| `game_auto.cpp` | Headless runner, renders MP4 to `runs/` |
| `program.md` | Agent instructions for the autonomous loop |
| `best_waypoints.json` | Current best waypoints |
| `results.tsv` | Full experiment log |
| `run_experiment.sh` | Helper to extract accuracy from a run |
