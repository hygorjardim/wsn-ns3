##!/usr/bin/gnuplot
#
# Creates a version of a plot, which looks nice for inclusion on web pages
#
# AUTHOR: Hagen Wierstorf

reset


set terminal pngcairo size 800,600 enhanced font 'Verdana,9'
set output 'jitteraodv.png'
set title "Jitter - AODV"
# svg
#set terminal svg size 800,600 fname 'Verdana' fsize 10
#set output 'introduction.svg'

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
set style line 2 lc rgb '#5e9c36' pt 2 ps 1 lt 1 lw 2 # --- green 
set style line 3 lc rgb '#365e9c' pt 10 ps 1 lt 1 lw 2 # --- blue
set style line 4 lc rgb '#f49b16' pt 1 ps 1 lt 1 lw 2 # --- orange
set style line 5 lc rgb '#602A8D' pt 4 ps 1 lt 1 lw 2 # --- orange
set style line 6 lc rgb '#5EC8D8' pt 6 ps 1 lt 1 lw 2 # --- orange

set key box bottom right # muda a posição da descrição das linhas

set xlabel 'Tempo (s)'
set ylabel 'Jitter (s)'
#set xrange [0:100]
#set yrange [-14:5]

plot 'todosjitter.txt' u 1:2 t 'Standard 802.11b - Dsss 2Mbps' w lp ls 1, \
     '' u 1:3 t 'Standard 802.11b - Dsss 5.5Mbps' w lp ls 2, \
     '' u 1:4 t 'Standard 802.11b - Dsss 11Mbps' w lp ls 3, \
     '' u 1:5 t 'Standard 802.11a - Ofdm 6Mbps' w lp ls 4, \
     '' u 1:6 t 'Standard 802.11a - Ofdm 9Mbps' w lp ls 5, \
     '' u 1:7 t 'Standard 802.11a - Ofdm 12Mbps' w lp ls 6