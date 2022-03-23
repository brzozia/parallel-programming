import subprocess

loops = [10000]
# x for x in range(1000, 10**4, 1000)
msg_size = [x for x in range(1, 30000, 1000)]
result_file = "./results_fin_"

compile_c = "mpicc -o send_rec send_recv.c"
run_c = "mpiexec -machinefile ./allnodes -np 2 ./send_rec "

subprocess.Popen(compile_c.split(" "))
for ai in range(1,3):
    for loop in loops:
        for size in msg_size:
            proces = subprocess.Popen((run_c + str(loop) + " " + str(size) + " " + str(result_file)+str(ai)+"txt").split(" "))
            proces.wait()
            print(proces.stdout)
