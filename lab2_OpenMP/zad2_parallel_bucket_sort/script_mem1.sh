#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid-testing
#SBATCH --account=plgmpr22

python run_bucket_no1.py
