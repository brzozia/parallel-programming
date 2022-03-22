import os
import subprocess

loops = [x for x in range(1000, 10**4, 1000) ]
msg_size = [x for x in range(1, 3000, 100)]
result_file = "./results_1_2n.txt"

compile_c = "mpicc -o send_rec send_recv.c"
run_c = "mpiexec -machinefile ./allnodes -np 2 ./send_rec "

subprocess.Popen(compile_c)
for size in msg_size:
    for loop in loops:
        proces = subprocess.Popen(run_c + str(loop) + " " + str(size) + " " + str(result_file))
        proces.wait()
	print(proces.stdout)


