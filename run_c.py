import os

loops = [1000, 10**4, 10**5, 10**6, 10**7, 10**8, 10**9, 10**10, 10**11]
msg_size = [x for x in range(1, 3000, 100)]
result_file = "results_1.txt"

compile_c = "mpicc -o send_rec send_recv.c"
run_c = "mpiexec -machinefile ./allnodes -np 2 ./send_rec "

os.system(compile_c)
for size in msg_size:
    for loop in loops:
        os.system(run_c + f"{loop} {size} {result_file}")