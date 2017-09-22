##!/usr/bin/gnuplot
#
# Creates a version of a plot, which looks nice for inclusion on web pages
#
# AUTHOR: Hagen Wierstorf

reset


set terminal pngcairo size 800,600 enhanced font 'Verdana,11'
set output 'vazaodsss1.png'
set title "Vazão - Dsss 1 Mbps"
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
set style line 1 lc rgb '#8b1a0e' pt 2 ps 1 lt 1 lw 2 # --- red
set style line 2 lc rgb '#5e9c36' pt 10 ps 1 lt 1 lw 2 # --- green 
set style line 3 lc rgb '#365e9c' pt 7 ps 1 lt 1 lw 2 # --- blue
set style line 4 lc rgb '#f49b16' pt 1 ps 1 lt 1 lw 2 # --- orange

# google colors 
# set style line 1 lc rgb '#EA4335' pt 2 ps 1 lt 1 lw 2 # --- red
# set style line 2 lc rgb '#34A853' pt 10 ps 1 lt 1 lw 2 # --- green 
# set style line 3 lc rgb '#4285F4' pt 7 ps 1 lt 1 lw 2 # --- blue
# set style line 4 lc rgb '#FBBC05' pt 1 ps 1 lt 1 lw 2 # --- orange

set key box bottom right # muda a posição da descrição das linhas

set xlabel 'Tempo (s)'
set ylabel 'Vazão (Mbps)'
#set xrange [0:100]
#set yrange [-14:5]

plot 'todosvazao.txt' u 1:2 t 'Static Routing' w lp ls 1, \
     '' u 1:3 t 'AODV' w lp ls 2, \
     '' u 1:4 t 'OLSR' w lp ls 3, \
     '' u 1:5 t 'DSDV' w lp ls 4