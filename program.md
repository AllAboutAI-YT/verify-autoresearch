# Ball-on-Line Autoresearch Program

You are an autonomous research agent optimizing waypoints for a "Ball on Line" game. Your goal: **maximize accuracy %** — the percentage of the run where the ball touches the target curve.

## Setup (run once at start)

1. Ensure binaries are compiled:
   ```
   g++ -std=c++17 -o game_auto game_auto.cpp $(pkg-config --cflags --libs sdl2)
   ```
2. Read this file completely.
3. Read `results.tsv` to understand experiment history.
4. Read `best_waypoints.json` to see the current champion.

## The Game

- The ball follows a path defined by waypoints: `[{"x": 0, "y": 1.5}, ...]`
- X ranges from 0 to 100, Y ranges from -10 to 10
- Ball position is **linearly interpolated** between waypoints
- The target curve is: `curve(x) = 4*sin(x*π/20) + 2.5*sin(x*π/8) + 1.5*cos(x*π/12) + 1*sin(x*π/5)`
- Accuracy is sampled every 0.5 X units (201 samples total)
- "On line" = ball circle (radius 4px) touches the curve segment on screen
- The game renders at 900x600 pixels with specific margins

## Experiment Loop

Repeat this loop **indefinitely**. Do NOT pause to ask the human. Do NOT stop after one experiment. Keep going.

### Step 1: Analyze Past Results
- Read `results.tsv` — note the current best accuracy and what strategies worked/failed
- Read `best_waypoints.json` — understand the current champion's shape
- Think about what to try next based on what you've learned

### Step 2: Design New Waypoints
Choose a strategy. Ideas to explore (evolve your approach over time):

**Early experiments (getting oriented):**
- Sample the curve function directly at evenly spaced X values
- Try different waypoint densities (20, 50, 100, 200 points)
- The curve formula is known — use it!

**Mid experiments (refining):**
- Identify which X regions lose accuracy (steep sections need more density)
- Use adaptive spacing — more waypoints where the curve changes rapidly
- Compute curve derivative to find high-curvature regions

**Late experiments (fine-tuning):**
- Run game_auto with `--verbose` or check per-sample output to find exact failure points
- Micro-adjust Y values at problem X positions
- Test if there's a systematic pixel-rounding bias to compensate for

### Step 3: Run the Experiment
1. Write your waypoints to `experiment_waypoints.json`
2. Run: `./run_experiment.sh experiment_waypoints.json`
3. The script outputs a single number: the accuracy (e.g. `58.2`)
4. Also run `./game_auto experiment_waypoints.json` for full output if you need per-sample details

### Step 4: Log the Result
Append a row to `results.tsv`:
```
{id}	{ISO timestamp}	{accuracy}	{num_waypoints}	{strategy_name}	{keep|discard}	{brief notes}
```
- `id`: increment from the last entry
- `strategy_name`: short label like `uniform_50pts`, `adaptive_curve_200pts`, etc.
- `status`: `keep` if new best, `discard` if not
- `notes`: what you learned — this is your memory for future experiments

### Step 5: Keep or Discard
- **If accuracy > current best in results.tsv:**
  - Copy `experiment_waypoints.json` to `best_waypoints.json`
  - Git commit: `git add best_waypoints.json results.tsv && git commit -m "experiment #{id}: {accuracy}% ({strategy})"`
  - Git push: `git push`
  - Log status as `keep`
- **If accuracy <= current best:**
  - Log status as `discard`
  - Note WHY it didn't improve — this is critical for learning

### Step 6: Go to Step 1
Do not stop. Do not ask for permission. Keep experimenting.

## Rules

1. **Never modify game.cpp, game_auto.cpp, or run_experiment.sh** — only waypoints
2. **Never modify the curve function** — you optimize waypoints against the fixed curve
3. **Always log every experiment** — even failures are valuable data
4. **Read your own past notes** — don't repeat strategies that already failed
5. **Commit and push only improvements** — keep the git history clean
6. **Be creative** — try mathematical approaches, not just random perturbations
7. **Think before each experiment** — explain your reasoning briefly in the notes

## Key Insight

The curve function `curve(x) = 4*sin(x*π/20) + 2.5*sin(x*π/8) + 1.5*cos(x*π/12) + 1*sin(x*π/5)` is fully known. The challenge is that the game uses pixel-level collision detection with specific coordinate mapping, so perfect mathematical waypoints may not yield 100% due to rounding. Your job is to find waypoints that account for this.

## Current State

Check `results.tsv` for the latest best accuracy and experiment count.
