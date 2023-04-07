//
// Created by arnav on 31/3/23.
//
#include <hpx/local/algorithm.hpp>
#include <hpx/local/init.hpp>

#include <iostream>
#include <chrono>
#include <random>
#include <vector>

std::mt19937 gen;

int digits(const int n) {
    if (n == 0) return 1;
    if (n < 0) return digits(-n) + 1;
    return digits(n / 10) + 1;
};

template<typename T>
class Matrix {
public:
    const std::size_t m, n;
    std::vector<T> matrix;

    Matrix(const std::size_t m, const std::size_t n) : n(n), m(m), matrix(n * m, 0) {
    }

    ~Matrix() = default;

    T &entry(unsigned int &i, unsigned int &j) {
        assert(i < m and j < n);
        return matrix[i * n + j];
    }

    Matrix<T> operator*(Matrix<T> &otherMatrix) {
        assert(otherMatrix.m == n);
        Matrix<T> returnMatrix(m, otherMatrix.n);

        hpx::experimental::for_loop(hpx::execution::par, 0, m, [&](unsigned int i){
            hpx::experimental::for_loop(hpx::execution::par, 0, n, [&](unsigned int j){
                hpx::experimental::for_loop(hpx::execution::par, 0, otherMatrix.n, [&](unsigned int k){
                    returnMatrix.entry(i, k) += matrix[i * n + j] * otherMatrix.entry(j, k);
                });
            });
        });

        return returnMatrix;
    }


    void pretty_print() {
        int maxW = 0;

        for (auto entry: matrix)
            maxW = std::max(maxW, digits(entry));

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++)
                std::cout << '|' << std::setw(maxW) << matrix[i * n + j];
            std::cout << "|\n\n";
        }
    }
};

int hpx_main(hpx::program_options::variables_map &vm) {
    std::size_t n = vm["n"].as<std::size_t>();
    std::size_t k = vm["k"].as<std::size_t>();
    std::size_t m = vm["m"].as<std::size_t>();
    int l = vm["l"].as<int>();
    int u = vm["u"].as<int>();

    assert(l <= u);

    unsigned int seed = std::random_device{}();
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    gen.seed(seed);
    std::cout << "using seed: " << seed << std::endl;

    // Matrices have random values in the range [lower, upper]
    std::uniform_int_distribution<int> dis(l, u);
    auto generator = std::bind(dis, gen);

    // Timer to keep track of time elapsed.
    auto start = std::chrono::high_resolution_clock::now();

    Matrix<int> firstMatrix(m, n);
    hpx::ranges::generate(firstMatrix.matrix, generator);
    Matrix<int> secondMatrix(n, k);
    hpx::ranges::generate(secondMatrix.matrix, generator);

    Matrix<int> product = firstMatrix * secondMatrix;

    auto timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

    std::cout << "First Matrix : \n";
    firstMatrix.pretty_print();
    std::cout << "Second Matrix : \n";
    secondMatrix.pretty_print();
    std::cout << "Product Matrix : \n";
    product.pretty_print();

    std::cout << "Time taken : " << static_cast<double>(timeTaken.count()) * 1e-9 << " seconds" << std::endl;

    return hpx::local::finalize();
}

int main(int argc, char *argv[]) {
    using namespace hpx::program_options;
    options_description cmdline("usage: " HPX_APPLICATION_STRING " [options]");
    // clang-format off
    cmdline.add_options()
            ("n",
             hpx::program_options::value<std::size_t>()->default_value(2),
             "Number of rows of first matrix")
            ("m",
             hpx::program_options::value<std::size_t>()->default_value(3),
             "Number of columns of first matrix (equal to the number of rows of "
             "second matrix)")
            ("k",
             hpx::program_options::value<std::size_t>()->default_value(2),
             "Number of columns of second matrix")
            ("l",
             hpx::program_options::value<int>()->default_value(0),
             "Lower limit of range of values")
            ("u",
             hpx::program_options::value<int>()->default_value(10),
             "Upper limit of range of values");
    // clang-format on
    hpx::local::init_params init_args;
    init_args.desc_cmdline = cmdline;

    return hpx::local::init(hpx_main, argc, argv, init_args);
}