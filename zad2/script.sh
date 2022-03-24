#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid-short
#SBATCH --account=plgmpr22

module add plgrid/tools/openmpi

mpicc -o pi paralel_pi_lab2.c

for i in 4e6 1e8 :
    for points_no in points:
        for proc in procesors:
            run_c = "mpiexec -np " + str(proc) + " ./pi " + i + " " + str(result_file)