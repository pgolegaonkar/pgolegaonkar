#!/bin/bash

for k in 10 11 12 13 14 15 16 17 18;
do
sbatch disk$k.slurm
done


