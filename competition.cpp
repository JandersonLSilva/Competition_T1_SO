// Biblioteca para I/O padrão (cin e cout)
#include <iostream>
// Contêiner da STL (Standard Templates Library)
#include <vector>
// Biblioteca para medição tempo
#include <chrono>
// Biblioteca para manipular strings
#include <string.h>

// Declarando a biblioteca para poder usar algumas operações matemáticas
#include <math.h>

// Bibliotecas para criar bloco de execuções paralelas
#include <omp.h>
#include <tbb/tbb.h>

// Biblioteca para manipular arquivos
#include <fstream>

// using namespace std; // para evitar escrever  std::, para referênciar a classe padrão
using namespace tbb; // para evitar de escrever tbb::, para referênciar a classe Threading Building Blocks

// Função que calcula o tempo de execução sequencial
float sequential_runtime(const int LENGTH, std::ofstream& file){
    int sum = 0;

    std::vector<int> array(LENGTH); // Declarando uma collection array do tipo int com LENGTH elementos todos inicializados com 1

    // Inicializando o array com os valores de i
    for(int i = 0; i < LENGTH; i++) array[i] = i;

    // Gravando o tempo no inicio da soma
    auto start = std::chrono::high_resolution_clock::now();

    // Somando os valores de cada posição do array
    for(int i = 0; i < LENGTH; i++) sum += array[i];

    // Marcando o tempo no final da da soma
    auto end = std::chrono::high_resolution_clock::now();

    // Armazenando na váriavel duration do tipo double, definido dentro da classe duration
    std::chrono::duration<double> duration = end - start;

    // Mostrando uma saída de dados especificando o intervalo de tempo entre as duas marcações
    std::cout << "Sequencial - Tempo de execução: " << duration.count() << " segundos." << std::endl;

    // Armazenando o tempo de execução dentro do arquivo
    file << "Sequencial - Tempo de execução: " << duration.count() << " segundos." << std::endl;

    // Retornando também a duração
    return duration.count();
}

// Função que calcula o tempo de execução paralela
float parallel_runtime_omp(const int LENGTH, std::ofstream& file){
    std::vector<int> array(LENGTH);
    int sum = 0;

    // Inicializando o array com os valores de i
    for(int i = 0; i < LENGTH; i++) array[i] = i;

    // Gravando o tempo no inicio da soma
    auto start = std::chrono::high_resolution_clock::now();

    /* Definindo que o próximo for será executada de forma paralela e que o soma será compartilhada
    entre as threads e que as atualizações nessa váriavel será feita usando a operação soma */
    #pragma omp parallel for reduction(+:sum)

    // Somando os valores de cada posição do array
    for(int i = 0; i < LENGTH; i++) sum += array[i];

    // Marcando o tempo no final da da soma
    auto end = std::chrono::high_resolution_clock::now();

    // Armazenando na váriavel duration do tipo double, definido dentro da classe duration
    std::chrono::duration<double> duration = end - start;

    // Mostrando uma saída de dados especificando o intervalo de tempo entre as duas marcações
    std::cout << "Paralelo (openmp) - Tempo de execução: " << duration.count() << " segundos." << std::endl;

    // Armazenando o tempo de execução dentro do arquivo
    file << "Paralelo (openmp) - Tempo de execução: " << duration.count() << " segundos." << std::endl;

    return duration.count();
}

// Função que calcula o tempo de execução paralela
float parallel_runtime_tbb(const int LENGTH, std::ofstream& file){
    std::vector<int> array(LENGTH);
    std::vector<int> partial_sums(4, 0);
    int total_sum = 0;

    // Inicializando o array com os valores de i
    for(int i = 0; i < LENGTH; i++) array[i] = i;

    blocked_range<int> range(0, LENGTH);

    // Gravando o tempo no inicio da soma
    auto start = std::chrono::high_resolution_clock::now();

    // Função anônima lambda que divide o bloco de execução em threads para serem executada de forma paralela
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
    auto end = std::chrono::high_resolution_clock::now();

    // Armazenando na váriavel duration do tipo double, definido dentro da classe duration
    std::chrono::duration<double> duration = end - start;

    std::cout << "Paralelo (tbb) - Tempo de execução: " << duration.count() << " segundos." << std::endl;

    // Armazenando o tempo de execução dentro do arquivo
    file << "Paralelo (tbb) - Tempo de execução: " << duration.count() << " segundos." << std::endl;

    return duration.count();
}

// Função que calcula a média de um determinado vetor
float calc_average(std::vector<float> vector, const int LENGTH){
    float sum = 0;
    for(int i = 0; i < LENGTH; i++){
        sum += vector[i];

    }
    return (sum / LENGTH);
}

// Função que calcula o desvio padrão de um determinado vetor, com base na média
float calc_std_deviation(std::vector<float> vector, const int LENGTH){
    float std_devtion = 0;
    float avg = calc_average(vector, LENGTH);

    // Somando cada potência quadrada da diferença entre o valor e a média
    for(int i = 0; i < LENGTH; i++) std_devtion += pow((vector[i] - avg), 2);

    // Retornando o desvio padrão, sendo esse a divisão entre, a somatória dos quadrados da
    // diferença entre cada valor e a média e o tamanho da amostra menos 1
    return sqrt((std_devtion / (LENGTH - 1)));
}

int main(int argc, char** argv){

    std::vector<float> sequ_retr(30);
    std::vector<float> parll_omp_retr(30);
    std::vector<float> parll_tbb_retr(30);


    std::ofstream file("logs/log.txt");

    for(int i = 0; i < 30; i++){
        // Mostrando o número da execução atual
        std::cout << "Execução nº" << i+1 << ": " << std::endl;

        // Armazenando o número da execução atual dentro do arquivo
        file << "Execução nº" << i+1 << ": " << std::endl;

        // Atribuindo cada tempo de execução ao seu índice no vetor
        sequ_retr[i] = sequential_runtime(std::pow(10, (atoi(argv[1]) || 8) ), file);
        parll_omp_retr[i] = parallel_runtime_omp(std::pow(10, (atoi(argv[1]) || 4) ), file);
        parll_tbb_retr[i] = parallel_runtime_tbb(std::pow(10, (atoi(argv[1]) || 4) ), file);

        std::cout << std::endl;
        file << std::endl;
    }

    std::cout << std::endl;
    file << std::endl;

    // Mostrando a Média do Sequencial e inserindo no arquivo
    std::cout << "Média do Sequencial: " << calc_average(sequ_retr, 30) << " segundos." << std::endl;
    file << "Média do Sequencial: " << calc_average(sequ_retr, 30) << " segundos." << std::endl;

    // Mostrando a Desvio Padrão do Sequencial e inserindo no arquivo
    std::cout << "Desvio Padrão do Sequencial: " << calc_std_deviation(sequ_retr, 30) << " segundos." << std::endl << std::endl;
    file << "Desvio Padrão do Sequencial: " << calc_std_deviation(sequ_retr, 30) << " segundos." << std::endl << std::endl;

    // Mostrando a Média do Paralelo (openmp) e inserindo no arquivo
    std::cout << "Média do Paralelo (openmp): " << calc_average(parll_omp_retr, 30) << " segundos." << std::endl;
    file << "Média do Paralelo (openmp): " << calc_average(parll_omp_retr, 30) << " segundos." << std::endl;

    // Mostrando a Desvio Padrão do Paralelo (openmp) e inserindo no arquivo
    std::cout << "Desvio Padrão do Paralelo (openmp): " << calc_std_deviation(parll_omp_retr, 30) << " segundos." << std::endl << std::endl;
    file << "Desvio Padrão do Paralelo (openmp): " << calc_std_deviation(parll_omp_retr, 30) << " segundos." << std::endl << std::endl;

    // Mostrando a Média do Paralelo (tbb) e inserindo no arquivo
    std::cout << "Média do Paralelo (tbb): " << calc_average(parll_tbb_retr, 30) << " segundos." << std::endl;
    file << "Média do Paralelo (tbb): " << calc_average(parll_tbb_retr, 30) << " segundos." << std::endl;

    // Mostrando a Desvio Padrão do Paralelo (tbb) e inserindo no arquivo
    std::cout << "Desvio Padrão do Paralelo (tbb): " << calc_std_deviation(parll_tbb_retr, 30) << " segundos." << std::endl << std::endl;
    file << "Desvio Padrão do Paralelo (tbb): " << calc_std_deviation(parll_tbb_retr, 30) << " segundos." << std::endl << std::endl;

    file.close(); // Fechando o arquivo aberto

    return 0;
}
