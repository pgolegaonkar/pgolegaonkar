#! /bin/bash 
 
#SBATCH --nodes=1
#SBATCH --output=HP1.out
#SBATCH --wait-all-nodes=1
 
 echo $SLURM_JOB_NODELIST
 
./MyCPUBench //exports/home/pgolegaonkar/cpu/cpu_HP_1thread.dat //exports/home/pgolegaonkar/cpu/output/cpu_SP_1thread.out.dat
