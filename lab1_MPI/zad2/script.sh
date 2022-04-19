#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid-testing
#SBATCH --account=plgmpr22

module add plgrid/tools/openmpi

mpicc -o pi paralel_pi_lab2.c

for points_no in 4000000 300000000 5000000000
do
    for proc in {1..12}
    do
        mpiexec -np  $proc ./pi  $points_no  results_promet_v26.txt
    done
done
