import re
import pandas as pd # É preciso ter o pandas instalado (pip install pandas)
import tabulate as tab # É necessário ter o tabulate instalado (pip install tabulate)
import matplotlib.pyplot as plt
import numpy as np


# Classe de Execuções
class Runtimes:
   def __init__(self, sequential, parallel_tbb, parallel_omp):
       # Mapeando os valores (vetores) do vetor recebido e substituindo sobre cada vetor a primeira posição do vetor, convertido para o tipo float, usando a função anônima lambda e depois atribuindo o vetor resultante ao próprio atributo da classe

       self.sequ = list(map(lambda v: float(v[0]), sequential))
       self.tbb = list(map(lambda v: float(v[0]), parallel_tbb))
       self.omp = list(map(lambda v: float(v[0]), parallel_omp))


# Função que extrai os dados do arquivo
def get_runtimes(file):
    with open(file, 'r') as file:
        text = file.read()
        runtimes = Runtimes( # Passando os dados retornados para cada atributo da classe Runtimes
            re.findall(r"Sequencial - Tempo de execução: (\d+(\.*\d*)e*-*\d*) segundos", text),
            re.findall(r"Paralelo \(tbb\) - Tempo de execução: (\d+(\.*\d*)e*-*\d*) segundos", text),
            re.findall(r"Paralelo \(openmp\) - Tempo de execução: (\d+(\.*\d*)e*-*\d*) segundos", text)
        )
    return runtimes

def create_file(path, data):
    with open(path, 'w') as file:
        file.write(data)


# Função que mostra os dados no formato de tabela
def show_table(data):
    # Transformando o objeto em um DataFrame
    data_frame = pd.DataFrame(data)

    # Formatando a saida da tabela (DataFrame)
    table = tab.tabulate(data_frame, headers='keys', tablefmt='grid', showindex=False)
    print(table)

    # Criando um arquivo e armazenando nele a tabela
    create_file('./output/runtimes_table.txt', table)



# Atribuindo o retorno da função get_runtimes a variável runtimes
runtimes = get_runtimes("./output/competition_output.txt")
LENGTH = len(runtimes.sequ)

# Criando um array com os números de execuções
runs = list( map(lambda r: f"nº{str(r)}:", (range(1, LENGTH+1)) ) )

# Criando um objeto com os dados
data = {
    'Run': runs,
    'Sequential': runtimes.sequ,
    'Parallel (tbb)': runtimes.tbb,
    'Parallel (openmp)': runtimes.omp
}
# Mostrando a tabela
show_table(data)


# Criando uma sequencia de números que será plotados nos gráficos sendo esses as execuções
runs = np.arange(1, LENGTH+1)

# Transformando os arrays comuns em arrays do NumPy que será necessário dentro do plot facilitando e permitindo os cáculos posteriormente
runtimes_sequ = np.array(runtimes.sequ)
runtimes_tbb = np.array(runtimes.tbb)
runtimes_omp = np.array(runtimes.omp)


# Criando gráficos de linha, passandos os argumentos a serem plotados e a legenda de cada linha do gráfico
plt.plot(runs, runtimes_sequ, label='Sequential')
plt.plot(runs, runtimes_tbb, label='Parallel (tbb)')
plt.plot(runs, runtimes_omp, label='Parallel (openmp)')

# Definindo os rótulos para linhas e colunas do gráfico
plt.xlabel('Executions')
plt.ylabel('Times (seconds)')

# Definindo o titulo do gráfico
plt.title("Runtimes in 30 executions")

# Adicionando uma legenda ao gráfico
plt.legend()

# Salvando uma imagem do gráfico de execuções
plt.savefig('./output/runtimes_graphic.png')

# Finalmente mostrando o gráfico de execuções
plt.show()
