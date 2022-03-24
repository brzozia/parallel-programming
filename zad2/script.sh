#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid-short
#SBATCH --account=plgmpr22

module add plgrid/tools/openmpi

mpicc -o pi paralel_pi_lab2.c

for points_no in 4e6 3e8 5e10
do
    for proc in {1..12}
    do
        mpiexec -np  $proc ./pi  $points_no  results_promet_v1.txt
    done
done