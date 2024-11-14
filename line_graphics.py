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
            re.findall(r"Sequencial - Tempo de execução: (\d+(\.\d+)?e-\d+)", text),
            re.findall(r"Paralelo (openmp) - Tempo de execução: (\d+(\.\d+)?e-\d+)", text),
            re.findall(r"Paralelo (tbb) - Tempo de execução: (\d+(\.\d+)?e-\d+)", text)
        )
    return runtimes

for runtime in get_runtimes('./logs/log.txt').sequ:
    print(f"Runtime Sequential: {runtime[0]}")
    print('-'*80)

# import matplotlib.pyplot as plt
# import numpy as np
#
# runs = np-arange(1, 31)
# runtimes_sequential = np.array()
