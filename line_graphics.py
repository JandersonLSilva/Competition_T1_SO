import re

class Runtimes:
   def __init__(self, sequential, parallel_tbb, parallel_omp):
       # Mapeando os valores (vetores) do vetor recebido e substituindo sobre cada vetor a primeira posição do vetor, convertido para o tipo float, usando a função anônima lambda e depois atribuindo o vetor resultante ao próprio atributo da classe

       self.sequ = list(map(lambda v: float(v[0]), sequential))
       self.tbb = list(map(lambda v: float(v[0]), parallel_tbb))
       self.omp = list(map(lambda v: float(v[0]), parallel_omp))

def get_runtimes(file):
    with open(file, 'r') as file:
        text = file.read()
        runtimes = Runtimes( # Passando os dados retornados para cada atributo da classe Runtimes
            re.findall(r"Sequencial - Tempo de execução: (\d+(\.*\d*)e*-*\d*) segundos", text),
            re.findall(r"Paralelo \(tbb\) - Tempo de execução: (\d+(\.*\d*)e*-*\d*) segundos", text),
            re.findall(r"Paralelo \(openmp\) - Tempo de execução: (\d+(\.*\d*)e*-*\d*) segundos", text)
        )
    return runtimes





# Atribuindo o retorno da função get_runtimes a variável runtimes
runtimes = get_runtimes("./logs/log.txt")
LENGTH = len(runtimes.sequ)




import pandas as pd # É preciso ter o pandas instalado (pip install pandas)
import tabulate as tab # É necessário ter o tabulate instalado (pip install tabulate)

# Criando um array com os números de execuções
runs = list( map(lambda r: f"nº{str(r)}:", (range(1, LENGTH+1)) ) )

# Criando um objeto com os dados
data = {
    'Run': runs,
    'Sequential': runtimes.sequ,
    'Parallel (tbb)': runtimes.tbb,
    'Parallel (openmp)': runtimes.omp
}
# Transformando o objeto em um DataFrame
data_frame = pd.DataFrame(data)

# Formatando a saida da tabela (DataFrame)
print(tab.tabulate(data_frame, headers='keys', tablefmt='grid', showindex=False))




# import matplotlib.pyplot as plt
# import numpy as np
#
# # Criando uma sequencia de números que será plotados no gráficos
# runs = np.arange(1, 31)
# runtimes_sequential = np.array()























#
