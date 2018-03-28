#! /bin/bash 
 
#SBATCH --nodes=1
#SBATCH --output=RWR2.out
#SBATCH --wait-all-nodes=1
 
 echo $SLURM_JOB_NODELIST
 
./MyRAMBenchmark //exports/home/pgolegaonkar/memory/memory-RWR-1000000-2thread.dat //exports/home/pgolegaonkar/memory/memory-RWR-1-1thread.out.dat
