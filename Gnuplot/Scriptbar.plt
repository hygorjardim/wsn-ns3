#!/usr/bin/gnuplot
#
# Plot statistics for three groups under two different conditions and indicate a
# significant difference between the groups (p < 0.01)
#
# AUTHOR: Hagen Wierstorf

reset

# wxt
#set terminal wxt size 350,262 enhanced font 'Verdana,10' persist
# png
set terminal pngcairo size 480,350 enhanced font 'Verdana,9'
set output 'Pacotes.png'
set title "Pacotes - Cenario 1, 2"
# svg
#set terminal svg size 350,262 fname 'Verdana, Helvetica, Arial, sans-serif' \
#fsize '10'
#set output 'statistics.svg'

set style line 2 lc rgb 'black' lt 1 lw 1
set style data histogram
set style histogram cluster gap 1
set style fill pattern border -1
set boxwidth 0.9
set xtics format ""
set grid ytics
set ylabel 'Pacotes'

plot "dados-pacote.dat" using 2:xtic(1) title "Enviados" ls 2, \
            '' using 3 title "Recebidos" ls 2, \
            '' using 4 title "Perdidos" ls 2, \