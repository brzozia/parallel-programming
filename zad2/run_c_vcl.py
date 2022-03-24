import subprocess

points = []
for i in range(10, 11): #how long it takes
    points.append(10**i)

repets = 10
procesors = [x for x in range(1, 13, 1)]
result_file = "./results_2_vcl_test.txt"

compile_c = "mpicc -o pi paralel_pi_lab2.c"
# run_c = "mpiexec -machinefile ./allnodes -np " + str(proc) + " ./pi "

subprocess.Popen(compile_c.split(" "))
for i in range(0,repets):
    for points_no in points:
        for proc in procesors:
            run_c = "mpiexec -machinefile ./allnodes" +str(proc)+ " -np " + str(proc) + " ./pi " + str(points_no) + " " + str(result_file)
            proces = subprocess.Popen(run_c.split(" "))
            proces.wait()
            print(proces.stdout)
