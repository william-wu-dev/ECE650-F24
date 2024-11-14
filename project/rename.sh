#!/bin/bash

for file in *.txt; do
    if [ -e "$file" ]; then
        newname=$(echo "$file" | sed 's/\.txt$/\.csv/')
        mv "$file" "$newname"
        echo "Renamed $file to $newname"
    fi
done
