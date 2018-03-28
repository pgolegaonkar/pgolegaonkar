#!/bin/bash

for k in 19 20 21 22 23 24 25 26 27;
do
sbatch disk$k.slurm
done


