set terminal png size 600, 600
set output "mergesort_plots.png"
set datafile separator ","
set size square 1.0, 1.0
set title "Trial results for 'mergesort_plots'"
set xlabel "Worker Count" 
set ylabel "Speedup" 
set key left top box
set pointsize 1.8
plot [0:16] [0:16] x title "parallelism" lt 2, 7.735163 notitle lt 2, "mergesort_plots.csv" using 1:2 title "burdened speedup" with lines lt 1, "mergesort_plots.csv" using 1:4 with points pt 1 title "trials"