set datafile separator ","
set xlabel "Number of Verticies"
set ylabel "Effective Ratio" 
# set ylabel "Effective Ratio" 
set title "Effective Ratio Comparison"

# Set the range of the axes
set xrange [0:55]
set yrange [0.9:2]
set xtics 5
set ytics 0.1

# Define the style of the error regions
set style fill solid 0.5 border

# Set key left
set key left

# Plot the data from the CSV file with low and high y values
# plot "data.csv" using 1:2:3:4 with yerrorlines  title "CNF-SAT-VC"
set term png size 800,600
set output "ER.png"

plot 'Effective Ratio.csv' using 1:2:($3):($4) with yerrorlines title "APPROX-1-VC" dt 2 lc 2 lt 32, \
     '' using 1:5:($6):($7) with yerrorlines title "APPROX-2-VC" dt 3 lc 3 lt 32