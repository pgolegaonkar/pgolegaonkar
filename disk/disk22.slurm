#! /bin/bash 
 
#SBATCH --nodes=1
#SBATCH --output=WR11000000.out
#SBATCH --wait-all-nodes=1
 
 echo $SLURM_JOB_NODELIST
 
./MyDiskBench //exports/home/pgolegaonkar/disk/disk-WR-1000000-1thread.dat //exports/home/pgolegaonkar/disk/output/disk-RR-1thread.out.dat
