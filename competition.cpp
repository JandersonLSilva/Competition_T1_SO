#include <iostream> // Declaração da biblioteca para I/O padrão (cin e cout)
#include <vector> // Declarando um contêiner da STL (Standard Templates Library)
#include <chrono> // Declarando uma biblioteca para medição tempo

#include <math.h> // Declarando a biblioteca para poder usar algumas operações matemáticas

#include <omp.h>
#include <tbb/tbb.h>

#include <fstream>

// using namespace std; // para evitar escrever  std::, para referênciar a classe padrão
using namespace tbb; // para evitar de escrever tbb::, para referênciar a classe Threading Building Blocks

float sequential_performance(const int LENGHT){

    std::vector<int> array(LENGHT); // Declarando uma collection array do tipo int com LENGTH elementos todos inicializados com 1

    for(int i = 0; i < LENGHT; i++){
        array[i] = i; // Inicializando o array com os valores de i
    }

    int sum = 0;
    auto start = std::chrono::high_resolution_clock::now(); // Gravando o tempo no inicio da soma

    for(int i = 0; i < LENGHT; i++){
        sum += array[i];
    }

    auto end = std::chrono::high_resolution_clock::now(); // Marcando o tempo no final da da soma
    std::chrono::duration<double> duration = end - start; // Armazenando na váriavel duration do tipo double, definido dentro da classe duration

    /// std::cout << "SOMA: " << sum << std::endl;
    std::cout << "Sequencial - Tempo de execução: " << duration.count() << " segundos." << std::endl; // Mostrando uma saída de dados especificando o intervalo de tempo entre as duas marcações

    return duration.count(); // Retornando também a duração
}

float parallel_performance_omp(const int LENGHT){
    std::vector<int> array(LENGHT);
    int sum = 0;

    for(int i = 0; i < LENGHT; i++){
        array[i] = i; // Inicializando o array com os valores de i
    }

    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for reduction(+:sum)
    /* Definindo que o próximo for será executada de forma paralela e que o soma será compartilhada entre as threads e as atualizações nessa váriavel será feita usando a operação soma */

    for(int i = 0; i < LENGHT; i++){
        sum += array[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // std::cout << "SOMA: " << sum << std::endl;
    std::cout << "Paralelo (openmp) - Tempo de execução: " << duration.count() << " segundos." << std::endl;

    return duration.count();
}

float parallel_performance_tbb(const int LENGHT){
    std::vector<int> array(LENGHT);
    std::vector<int> partial_sums(4, 0);
    int total_sum = 0;

    for(int i = 0; i < LENGHT; i++){
        array[i] = i; // Inicializando o array com os valores de i
    }

    blocked_range<int> range(0, LENGHT);

    auto start = std::chrono::high_resolution_clock::now();

    auto h = [&](const blocked_range<int>r) {
        int sum = 0;
        int tid = this_task_arena::current_thread_index();
        for (int i = r.begin(); i < r.end(); ++i) {
            sum += array[i];
        }
        partial_sums[tid] += sum;
    };

    parallel_for(range, h);

    for (auto partial_sum : partial_sums) {
        total_sum += partial_sum;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // std::cout << "SOMA: " << total_sum << std::endl;
    std::cout << "Paralelo (tbb) - Tempo de execução: " << duration.count() << " segundos." << std::endl;

    return duration.count();
}

float calc_avarage(std::vector<float> array, const int LENGHT){
    float sum = 0;
    for(int i = 0; i < LENGHT; i++){
        sum += array[i];

    }
    return (sum / LENGHT);
}

float calc_std_deviation(std::vector<float> array, float avg, const int LENGHT){
    float std_devtion = 0;
    for(int i = 0; i < LENGHT; i++){
        std_devtion += pow((array[i] - avg), 2);
    }
    return sqrt((std_devtion / (LENGHT - 1)));
}

int main(int argc, char** argv){

    std::vector<float> sequ_retr(30);
    std::vector<float> parll_omp_retr(30);
    std::vector<float> parll_tbb_retr(30);


    std::
    for(int i = 0; i < 30; i++){
        std::cout << "Execução nº" << i << ": " << std::endl;
        sequ_retr[i] = sequential_performance(std::pow(10, (atoi(argv[1]) || 8) ));

        parll_omp_retr[i] = parallel_performance_omp(std::pow(10, (atoi(argv[1]) || 4) ));

        parll_tbb_retr[i] = parallel_performance_tbb(std::pow(10, (atoi(argv[1]) || 4) ));
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Média do Sequencial: " << calc_avarage(sequ_retr, 30) << " segundos." << std::endl;
    std::cout << "Desvio Padrão do Sequencial: " << calc_std_deviation(sequ_retr, calc_avarage(sequ_retr, 30), 30) << " segundos." << std::endl << std::endl;

    std::cout << "Média do Paralelo (openmp): " << calc_avarage(parll_omp_retr, 30) << " segundos." << std::endl;
    std::cout << "Desvio Padrão do Paralelo (openmp): " << calc_std_deviation(parll_omp_retr, calc_avarage(parll_omp_retr, 30), 30) << " segundos." << std::endl << std::endl;

    std::cout << "Média do Paralelo (tbb): " << calc_avarage(parll_tbb_retr, 30) << " segundos." << std::endl;
    std::cout << "Desvio Padrão do Paralelo (tbb): " << calc_std_deviation(parll_tbb_retr, calc_avarage(parll_tbb_retr, 30), 30) << " segundos." << std::endl << std::endl;

    return 0;
}
