#! /bin/bash 
 
#SBATCH --nodes=1
#SBATCH --output=RR2100000.out
#SBATCH --wait-all-nodes=1
 
 echo $SLURM_JOB_NODELIST
 
./MyDiskBench //exports/home/pgolegaonkar/disk/disk-RR-100000-2thread.dat //exports/home/pgolegaonkar/disk/output/disk-RR-1thread.out.dat
