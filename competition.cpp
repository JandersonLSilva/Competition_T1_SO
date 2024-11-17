// Biblioteca para I/O padrão (cin e cout)
#include <iostream>
// Contêiner da STL (Standard Templates Library)
#include <vector>
// Biblioteca para medição tempo
#include <chrono>

// Declarando a biblioteca para poder usar algumas operações matemáticas
#include <math.h>

// Bibliotecas para criar bloco de execuções paralelas
#include <omp.h>
#include <tbb/tbb.h>

// Biblioteca para manipular arquivos
#include <fstream>

using namespace std; // para evitar escrever  std::, para referênciar a classe padrão
using namespace tbb; // para evitar de escrever tbb::, para referênciar a classe Threading Building Blocks

// Função que calcula o tempo de execução sequencial
float sequential_runtime(const int LENGTH, ofstream& file){
    int sum = 0;

    vector<int> array(LENGTH); // Declarando uma collection array do tipo int com LENGTH elementos todos inicializados com 1

    // Inicializando o array com os valores de i
    for(int i = 0; i < LENGTH; i++) array[i] = i;

    // Gravando o tempo no inicio da soma
    auto start = chrono::high_resolution_clock::now();

    // Somando os valores de cada posição do array
    for(int i = 0; i < LENGTH; i++) sum += array[i];

    // Marcando o tempo no final da da soma
    auto end = chrono::high_resolution_clock::now();

    // Armazenando na váriavel duration do tipo double, definido dentro da classe duration
    chrono::duration<double> duration = end - start;

    // Mostrando uma saída de dados especificando o intervalo de tempo entre as duas marcações
    cout << "Sequencial - Tempo de execução: " << duration.count() << " segundos." << endl;

    // Armazenando o tempo de execução dentro do arquivo
    file << "Sequencial - Tempo de execução: " << duration.count() << " segundos." << endl;

    // Retornando também a duração
    return duration.count();
}

// Função que calcula o tempo de execução paralela
float parallel_runtime_omp(const int LENGTH, ofstream& file){
    vector<int> array(LENGTH);
    int sum = 0;

    // Inicializando o array com os valores de i
    for(int i = 0; i < LENGTH; i++) array[i] = i;

    // Gravando o tempo no inicio da soma
    auto start = chrono::high_resolution_clock::now();

    /* Definindo que o próximo for será executada de forma paralela e que o soma será compartilhada
    entre as threads e que as atualizações nessa váriavel será feita usando a operação soma */
    #pragma omp parallel for reduction(+:sum)

    // Somando os valores de cada posição do array
    for(int i = 0; i < LENGTH; i++) sum += array[i];

    // Marcando o tempo no final da da soma
    auto end = chrono::high_resolution_clock::now();

    // Armazenando na váriavel duration do tipo double, definido dentro da classe duration
    chrono::duration<double> duration = end - start;

    // Mostrando uma saída de dados especificando o intervalo de tempo entre as duas marcações
    cout << "Paralelo (openmp) - Tempo de execução: " << duration.count() << " segundos." << endl;

    // Armazenando o tempo de execução dentro do arquivo
    file << "Paralelo (openmp) - Tempo de execução: " << duration.count() << " segundos." << endl;

    return duration.count();
}

// Função que calcula o tempo de execução paralela
float parallel_runtime_tbb(const int LENGTH, ofstream& file){
    vector<int> array(LENGTH);
    vector<int> partial_sums(4, 0);
    int total_sum = 0;

    // Inicializando o array com os valores de i
    for(int i = 0; i < LENGTH; i++) array[i] = i;

    // Definindo o intervalo de índices em que as threads vão trabalhar
    blocked_range<int> range(0, LENGTH);

    // Gravando o tempo no inicio da soma
    auto start = chrono::high_resolution_clock::now();

    // Função anônima lambda que define o bloco de execução que será executada de forma paralela pelas threads posteriormente
    auto h = [&](const blocked_range<int>r) {
        int sum = 0;
        // Atribuindo o índice da thread da tarefa atual ao tid
        int tid = this_task_arena::current_thread_index();

        // Somando o intervalo valores do array que a thread ficou responsável
        for (int i = r.begin(); i < r.end(); ++i) sum += array[i];

        // Somando a soma, feita pela thread atual, ao valor do índice tid da variável soma parcial
        partial_sums[tid] += sum;
    };


    parallel_for(range, h);

    // Somando os valores da soma parcial a variável soma total
    for (auto partial_sum : partial_sums) total_sum += partial_sum;

    // Marcando o tempo no final da da soma
    auto end = chrono::high_resolution_clock::now();

    // Armazenando na váriavel duration do tipo double, definido dentro da classe duration
    chrono::duration<double> duration = end - start;

    cout << "Paralelo (tbb) - Tempo de execução: " << duration.count() << " segundos." << endl;

    // Armazenando o tempo de execução dentro do arquivo
    file << "Paralelo (tbb) - Tempo de execução: " << duration.count() << " segundos." << endl;

    return duration.count();
}

// Função que calcula a média de um determinado vetor
float calc_average(vector<float> vector, const int LENGTH){
    float sum = 0;
    for(int i = 0; i < LENGTH; i++){
        sum += vector[i];

    }
    return (sum / LENGTH);
}

// Função que calcula o desvio padrão de um determinado vetor, com base na média
float calc_std_deviation(vector<float> vector, const int LENGTH){
    float std_devtion = 0;
    float avg = calc_average(vector, LENGTH);

    // Somando cada potência quadrada da diferença entre o valor e a média
    for(int i = 0; i < LENGTH; i++) std_devtion += pow((vector[i] - avg), 2);

    // Retornando o desvio padrão, sendo esse a divisão entre, a somatória dos quadrados da
    // diferença entre cada valor e a média e o tamanho da amostra menos 1
    return sqrt((std_devtion / (LENGTH - 1)));
}


void show_average_deviation(ofstream& file, const string name, vector<float> vector, const int LENGTH){

    // Mostrando a Média e inserindo no arquivo
    cout << "Média do " << name << ": " << calc_average(vector, LENGTH) << " segundos." << endl;
    file << "Média do " << name << ": " << calc_average(vector, LENGTH) << " segundos." << endl;

    // Mostrando a Desvio Padrão e inserindo no arquivo
    cout << "Desvio Padrão do " << name << ": " << calc_std_deviation(vector, LENGTH) << " segundos." << endl << endl;
    file << "Desvio Padrão do " << name << ": " << calc_std_deviation(vector, LENGTH) << " segundos." << endl << endl;
}

int main(int argc, char** argv){
    const int QUANT_RUNTIMES = 30;

    vector<float> sequ_retr(QUANT_RUNTIMES);
    vector<float> parll_omp_retr(QUANT_RUNTIMES);
    vector<float> parll_tbb_retr(QUANT_RUNTIMES);


    ofstream file("output/competition_output.txt");

    for(int i = 0; i < QUANT_RUNTIMES; i++){
        // Mostrando o número da execução atual e o armazenando dentro do arquivo
        cout << "Execução nº" << i+1 << ": " << endl;
        file << "Execução nº" << i+1 << ": " << endl;

        // Atribuindo cada tempo de execução ao seu índice no vetor
        sequ_retr[i] = sequential_runtime(pow(10, (argv[1] ? atoi(argv[1]) : 8 ) ), file);
        parll_omp_retr[i] = parallel_runtime_omp(pow(10, (argv[1] ? atoi(argv[1]) : 8 ) ), file);
        parll_tbb_retr[i] = parallel_runtime_tbb(pow(10, (argv[1] ? atoi(argv[1]) : 8 ) ), file);

        cout << endl;
        file << endl;
    }

    cout << endl;
    file << endl;

    // Mostrando a Média e o Desvio Padrão do Sequencial e inserindo no arquivo
    show_average_deviation(file, "Sequencial", sequ_retr, QUANT_RUNTIMES);

    // Mostrando a Média e o Desvio Padrão do Paralelo (openmp) e inserindo no arquivo
    show_average_deviation(file, "Paralelo (openmp)", parll_omp_retr, QUANT_RUNTIMES);

    // Mostrando a Média e o Desvio Padrão do Paralelo (tbb) e inserindo no arquivo
    show_average_deviation(file, "Paralelo (tbb)", parll_tbb_retr, QUANT_RUNTIMES);


    file.close(); // Fechando o arquivo aberto

    return 0;
}
