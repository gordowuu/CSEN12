#!/bin/bash

# Script to run timing tests for unique and parity programs
# Runs each test 3 times and calculates the average

# List of test files
files=(
    "GreenEggsAndHam.txt"
    "Macbeth.txt"
    "Genesis.txt"
    "HoundOfTheBaskervilles.txt"
    "TheWarOfTheWorlds.txt"
    "TreasureIsland.txt"
    "TheSecretGarden.txt"
    "TwentyThousandLeagues.txt"
    "TheCountOfMonteCristo.txt"
    "Bible.txt"
)

# Base directory for test files
base_dir="/scratch/coen12"

echo "Running timing tests for UNIQUE (hashing)..."
echo "=============================================="
echo ""
echo "unique - hashing"
echo "----------------"

# Array to store unique results
declare -a unique_results

for file in "${files[@]}"; do
    total=0
    for i in 1 2 3; do
        # Run the command and extract real time in seconds
        time_output=$( { time ./unique "$base_dir/$file" > /dev/null 2>&1; } 2>&1 )
        real_time=$(echo "$time_output" | grep real | awk '{print $2}')
        
        # Convert time to seconds (handle both mm:ss.sss and ss.sss formats)
        if [[ $real_time == *m* ]]; then
            minutes=$(echo "$real_time" | cut -d'm' -f1)
            seconds=$(echo "$real_time" | cut -d'm' -f2 | sed 's/s//')
            time_in_seconds=$(echo "$minutes * 60 + $seconds" | bc)
        else
            time_in_seconds=$(echo "$real_time" | sed 's/s//')
        fi
        
        total=$(echo "$total + $time_in_seconds" | bc)
        echo "  $file (run $i): $time_in_seconds seconds"
    done
    
    # Calculate average
    avg=$(echo "scale=4; $total / 3" | bc)
    unique_results+=("$file:$avg")
    echo "  $file AVERAGE: $avg seconds"
    echo ""
done

echo ""
echo "Running timing tests for PARITY (hashing)..."
echo "=============================================="
echo ""
echo "parity - hashing"
echo "----------------"

# Array to store parity results
declare -a parity_results

for file in "${files[@]}"; do
    total=0
    for i in 1 2 3; do
        # Run the command and extract real time in seconds
        time_output=$( { time ./parity "$base_dir/$file" > /dev/null 2>&1; } 2>&1 )
        real_time=$(echo "$time_output" | grep real | awk '{print $2}')
        
        # Convert time to seconds (handle both mm:ss.sss and ss.sss formats)
        if [[ $real_time == *m* ]]; then
            minutes=$(echo "$real_time" | cut -d'm' -f1)
            seconds=$(echo "$real_time" | cut -d'm' -f2 | sed 's/s//')
            time_in_seconds=$(echo "$minutes * 60 + $seconds" | bc)
        else
            time_in_seconds=$(echo "$real_time" | sed 's/s//')
        fi
        
        total=$(echo "$total + $time_in_seconds" | bc)
        echo "  $file (run $i): $time_in_seconds seconds"
    done
    
    # Calculate average
    avg=$(echo "scale=4; $total / 3" | bc)
    parity_results+=("$file:$avg")
    echo "  $file AVERAGE: $avg seconds"
    echo ""
done

echo ""
echo "=========================================="
echo "SUMMARY - Copy these values to report.txt"
echo "=========================================="
echo ""
echo "unique (hashing column):"
echo "------------------------"
for result in "${unique_results[@]}"; do
    filename=$(echo "$result" | cut -d':' -f1)
    avg=$(echo "$result" | cut -d':' -f2)
    printf "%-30s %s\n" "$filename" "$avg"
done

echo ""
echo "parity (hashing column):"
echo "------------------------"
for result in "${parity_results[@]}"; do
    filename=$(echo "$result" | cut -d':' -f1)
    avg=$(echo "$result" | cut -d':' -f2)
    printf "%-30s %s\n" "$filename" "$avg"
done
