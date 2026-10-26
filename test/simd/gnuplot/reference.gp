# sn.gp

if (!exists("title")) title = sprintf("sn (m=%.6f)", tanh(s*e))
if (!exists("label")) label = sprintf("m=%.6f", tanh(s*e))

set terminal svg size 1600,640
set output outfile

# set title title
set xlabel "ζ"
set ylabel "sn"
set grid
# set key autotitle columnhead

set xrange [0:360]
set xtics 30
set format x "%g°"

set yrange [-1:1]
set ytics 0.25

plot infile using 1:2 with lines linecolor rgb "#F00000" linewidth 1.5 title label
     
unset output