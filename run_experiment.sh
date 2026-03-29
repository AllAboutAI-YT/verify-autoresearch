#!/bin/bash
# Usage: ./run_experiment.sh <waypoints.json>
# Runs game_auto and outputs just the accuracy number (e.g. "58.2")
if [ -z "$1" ]; then
    echo "Usage: $0 <waypoints.json>"
    exit 1
fi
./game_auto "$1" 2>/dev/null | grep "^Accuracy:" | awk '{print $2}' | tr -d '%'
