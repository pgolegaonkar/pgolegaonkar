#!/bin/bash

for k in 19 20 21 22 23 24;
do
sbatch config$k.slurm
done


