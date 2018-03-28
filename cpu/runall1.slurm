#!/bin/bash

for k in 10 11 12;
do
sbatch cpu$k.slurm
done

