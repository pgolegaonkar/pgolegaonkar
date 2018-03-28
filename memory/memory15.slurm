#! /bin/bash 
 
#SBATCH --nodes=1
#SBATCH --output=RWS41.out
#SBATCH --wait-all-nodes=1
 
 echo $SLURM_JOB_NODELIST
 
./MyRAMBench //exports/home/pgolegaonkar/memory/memory-RWS-1-4thread.dat //exports/home/pgolegaonkar/memory/output/memory-RWR-1-1thread.out.dat
