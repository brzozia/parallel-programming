import subprocess

points = [10**10, 10**20, 10*30]


repets = 20
procesors = [x for x in range(1, 13, 1)]
result_file = "./results_2_vcl_v1.txt"

compile_c = "mpicc -o pi paralel_pi_lab2.c"
# run_c = "mpiexec -machinefile ./allnodes -np " + str(proc) + " ./pi "

proces = subprocess.Popen(compile_c.split(" "))
proces.wait()
for i in range(0,repets):
    for points_no in points:
        for proc in procesors:
            run_c = "mpiexec -machinefile ./allnodes" +str(proc)+ " -np " + str(proc) + " ./pi " + str(points_no) + " " + str(result_file)
            proces = subprocess.Popen(run_c.split(" "))
            proces.wait()
            print(proces.stdout)
