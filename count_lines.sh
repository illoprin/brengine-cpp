#!/usr/bin/bash

# Count total number of lines in .cpp and .h files

path=$1

total_lines=0

for file in $(find $path -name '*.cpp' -o -name '*.h'); do
	lines=$(grep -c '^' "$file")
	echo "$file  - $lines lines"
	total_lines=$((total_lines + lines))
done

echo "Total number of lines: $total_lines"