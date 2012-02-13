#set terminal png size 600, 600
#set output "LJ_PARALLEL_CODE.png"
set datafile separator ","
set size square 1.0, 1.0
set title "Trial results for 'LJ_PARALLEL_CODE'"
set xlabel "Worker Count" 0.0,0.0
set ylabel "Speedup" 0.0,0.0
set key left top box
set pointsize 1.8
plot [0:16] [0:16] x title "parallelism" lt 2, 1. notitle lt 2, "LJ_PARALLEL_CODE.csv" using 1:2 title "burdened speedup" with lines lt 1, "LJ_PARALLEL_CODE.csv" using 1:4 with points pt 1 title "trials"
