import subprocess

sizes = [x for x in range(100000000,16451788000, 10000)]

compile_c = "gcc -Wall ./check_mem.c -o check"


proces = subprocess.Popen(compile_c.split(" "))
proces.wait()

for size in sizes:
    proces = subprocess.Popen(["bash", "-c", "./rand" + " " + str(size)])
    proces.wait()
