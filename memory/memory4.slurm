#! /bin/bash 
 
#SBATCH --nodes=1
#SBATCH --output=RWR11000.out
#SBATCH --wait-all-nodes=1
 
 echo $SLURM_JOB_NODELIST
 
./MyRAMBench //exports/home/pgolegaonkar/memory/memory-RWR-1000-1thread.dat //exports/home/pgolegaonkar/memory/output/memory-RWR-1-1thread.out.dat
