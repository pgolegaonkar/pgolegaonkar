#!/bin/bash

for k in 28 29 30; 
do
sbatch memory$k.slurm
done


