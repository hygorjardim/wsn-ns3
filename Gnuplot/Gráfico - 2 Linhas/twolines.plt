##!/usr/bin/gnuplot
#
# Creates a version of a plot, which looks nice for inclusion on web pages
#
# AUTHOR: Hagen Wierstorf

reset


set terminal pngcairo size 800,600 enhanced font 'Verdana,9'
set output 'nice_web_plot.png'
set title "Vazão - Cenario 1, 2"
# svg
#set terminal svg size 410,250 fname 'Verdana, Helvetica, Arial, sans-serif' \
#fsize '9' rounded dashed
#set output 'nice_web_plot.svg'

# define axis
# remove border on top and right and set color to gray
set style line 11 lc rgb '#808080' lt 1
set border 3 back ls 11
set tics nomirror
# define grid
set style line 12 lc rgb '#808080' lt 0 lw 1
set grid back ls 12

# color definitions
set style line 1 lc rgb '#8b1a0e' pt 1 ps 1 lt 1 lw 2 # --- red
set style line 2 lc rgb '#5e9c36' pt 6 ps 1 lt 1 lw 2 # --- green

set key bottom right

set xlabel 'Tempo (s)'
set ylabel 'Vazão (Mbps)'
#set xrange [0:100]
#set yrange [0:1]

plot 'modelo-org-dados.txt' u 1:2 t 'WSN - 10m' w lp ls 1, \
     '' u 1:3 t 'WSN - 15m' w lp ls 2
