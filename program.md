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
- The target curve is **unknown** — you cannot read the source code to find it
- Accuracy is sampled every 0.5 X units (201 samples total)
- "On line" = ball circle (radius 4px) touches the curve segment on screen
- The game renders at 900x600 pixels with specific margins
- You must discover the curve shape through experimentation

## Experiment Loop

Repeat this loop **indefinitely**. Do NOT pause to ask the human. Do NOT stop after one experiment. Keep going.

### Step 1: Analyze Past Results
- Read `results.tsv` — note the current best accuracy and what strategies worked/failed
- Read `best_waypoints.json` — understand the current champion's shape
- Think about what to try next based on what you've learned

### Step 2: Design New Waypoints
Choose a strategy. Ideas to explore (evolve your approach over time):

**Early experiments (probing the curve):**
- Try flat lines at different Y values to map out the curve's range
- Probe individual regions: set Y high/low at specific X to find where the curve is
- Use the per-sample output from `./game_auto` to see which X positions are ON vs OFF line

**Mid experiments (building a map):**
- Use the ON/OFF data from runs to narrow down the curve's Y value at each X
- Binary search: if Y=3 is off and Y=5 is on at X=20, try Y=4
- Increase waypoint density in regions you've mapped well
- Use per-sample details to identify steep vs flat sections

**Late experiments (fine-tuning):**
- Micro-adjust Y values at specific X positions that are still OFF
- Increase density in steep regions where linear interpolation causes misses
- Try small perturbations (+/- 0.1) around known good values

### Step 3: Run the Experiment
1. Write your waypoints to `experiment_waypoints.json`
2. Run: `./game_auto experiment_waypoints.json`
3. The output contains:
   - **Accuracy line**: `Accuracy: 4.5%`
   - **Per-sample table**: for every 0.5 X step from 0 to 100, shows `X`, `BallY`, and `YES`/`NO` (on line or not)
   - **best_runs.csv**: overwritten each run with columns `x,ball_y,on_line,accuracy` — your latest per-sample data
   - **MP4 video**: saved to `runs/{accuracy}%_{timestamp}.mp4`
4. **The per-sample YES/NO data is your primary feedback signal.** Use it to learn where the curve is:
   - If `BallY=3.0` at `X=20` is `YES` → the curve passes near Y=3.0 at X=20
   - If `BallY=3.0` at `X=20` is `NO` → the curve is NOT near Y=3.0 at X=20
   - By probing different Y values at each X, you can map out the curve

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

1. **Never read game.cpp or game_auto.cpp** — the curve is unknown, you must discover it
2. **Never modify game.cpp, game_auto.cpp, or run_experiment.sh** — only waypoints
3. **Always log every experiment** — even failures are valuable data
4. **Read your own past notes** — don't repeat strategies that already failed
5. **Commit and push only improvements** — keep the git history clean
6. **Be scientific** — form hypotheses, test them, record learnings
7. **Think before each experiment** — explain your reasoning briefly in the notes
8. **Use the per-sample output** — `./game_auto experiment_waypoints.json` shows ON/OFF for each X sample, this is your primary feedback signal

## Current State

- Baseline: **4.5%** accuracy from random waypoints (21 points, random Y in -8 to 8)
- Check `results.tsv` for the latest best accuracy and full experiment history
- Check `best_runs.csv` for per-sample data from the latest run
