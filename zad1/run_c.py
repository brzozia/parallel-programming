import subprocess


loops = [10000]
# x for x in range(1000, 10**4, 1000)
msg_size =[1 for x in range(0,10)]+ [x for x in range(0, 3100, 100)]
result_file = "./results_fin2a_v2.txt"

compile_c = "mpicc -o send_rec send_recv.c"
run_c = "mpiexec -machinefile ./allnodes2 -np 2 ./send_rec "

subprocess.Popen(compile_c.split(" "))
# for ai in range(1,3):
for loop in loops:
    for size in msg_size:
        proces = subprocess.Popen((run_c + str(loop) + " " + str(size) + " " + str(result_file)+" aa").split(" "))
        proces.wait()
        print(proces.stdout)

