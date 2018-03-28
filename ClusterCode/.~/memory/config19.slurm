#! /bin/bash 
 
#SBATCH --nodes=1
#SBATCH --output=RWS1.out
#SBATCH --wait-all-nodes=1
 
 echo $SLURM_JOB_NODELIST
 
./MyRAMBenchmark //exports/home/pgolegaonkar/memory/memory-RWS-1000000-1thread.dat //exports/home/pgolegaonkar/memory/memory-RWR-1-1thread.out.dat
