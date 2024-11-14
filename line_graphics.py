import re

class Runtimes:
   def __init__(self, sequential, parallel_tbb, parallel_omp):
       self.sequ = sequential
       self.tbb = parallel_tbb
       self.omp = parallel_omp

def get_runtimes(file):
    with open(file, 'r') as file:
        text = file.read()
        runtimes = Runtimes(
            re.findall(r"Sequencial - Tempo de execução: (\d+(\.*\d*)e*-*\d*) segundos", text),
            re.findall(r"Paralelo \(tbb\) - Tempo de execução: (\d+(\.*\d*)e*-*\d*) segundos", text),
            re.findall(r"Paralelo \(openmp\) - Tempo de execução: (\d+(\.*\d*)e*-*\d*) segundos", text)
        )
    return runtimes

runtimes = get_runtimes("./logs/log.txt")

runtimes.sequ = list(map(lambda v: float(v[0]), runtimes.sequ))
runtimes.tbb = list(map(lambda v: float(v[0]), runtimes.tbb))
runtimes.omp = list(map(lambda v: float(v[0]), runtimes.omp))

i = 0
for runtime in runtimes.sequ:
    i+=1
    print(f"{i}° - Runtime Sequential: {runtime}")

for runtime in runtimes.tbb:
    i+=1
    print(f"{i}° - Runtime Paralelo (tbb): {runtime}")

for runtime in runtimes.omp:
    i+=1
    print(f"{i}° - Runtime Parelelo (openmp): {runtime}")

# import matplotlib.pyplot as plt
# import numpy as np
#
# runs = np-arange(1, 31)
# runtimes_sequential = np.array()
