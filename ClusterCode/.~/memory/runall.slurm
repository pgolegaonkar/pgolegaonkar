#!/bin/bash

for k in 1 2 3 4 5 6 7 8 9;
do
sbatch config$k.slurm
done

for k in  10 11 12 13 14 15 16 17 18 19;
do
sbatch config$k.slurm
done

for k in 20 21 22 23 24;
do
sbatch config$k.slurm
done
