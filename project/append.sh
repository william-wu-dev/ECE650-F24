#!/bin/bash

for file in *.csv; do
    if [ -f "$file" ]; then
        averages="=AVERAGE(A1:A10),=AVERAGE(B1:B10),=AVERAGE(C1:C10),=AVERAGE(D1:D10),=AVERAGE(E1:E10),=AVERAGE(F1:F10),"
        echo "$averages" >> "$file"
        echo "Appended averages to $file"
    fi
done
