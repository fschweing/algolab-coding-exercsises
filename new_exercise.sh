#!/bin/bash

# chmod +x new_exercise.sh

if [ -z "$1" ]; then
    echo "❌ Usage: ./new_exercise.sh <exercise_name>"
    exit 1
fi

EXERCISE=$1
TEMPLATE_DIR="./template_dir"
DEST_DIR="./$EXERCISE"

if [ -d "$DEST_DIR" ]; then
    echo "❌ Directory '$EXERCISE' already exists"
    exit 1
fi

cp -r "$TEMPLATE_DIR" "$DEST_DIR"
echo "✅ Created exercise '$EXERCISE' from template"