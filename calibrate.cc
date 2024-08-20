//
// Created by jhoskins on 5/22/24.
//
#include <iomanip>
#include <iostream>
#include <complex>

#include <DataFile.h>
#include <AntennaSolve.h>

#include <mdspan.hpp>


//
// Model rho_ij: std::vector <complex> model[N] = 1 + 0.0j
//

namespace data{
    template <typename T, class Extents>
    void print(Kokkos::mdspan<T, Extents> M) {
        const unsigned m = M.extent(0);
        const unsigned n = M.extent(1);

        std::cout << "Real Component:" << std::endl;

        for(unsigned i = 0; i < m; i++){
            std::cout << "| ";
            for(unsigned j = 0; j < n; j++) {
                if(i==j){
                    std::cout << std::right << std::setw(15) << "\x1B[31m" <<  M[i, j].imag() << "\x1B[0m" << "\t";
                }else {
                    std::cout << std::right << std::setw(15) << M[i, j].real() << "\t";
                }
            }
            std::cout << "  |" << std::endl;
        }

        std::cout << "\n\nImaginary Component:" << std::endl;
        for(unsigned i = 0; i < m; i++){
            std::cout << "| ";
            for(unsigned j = 0; j < n; j++) {
                if(i==j){
                    std::cout << std::right << std::setw(15) << "\x1B[31m" <<  M[i, j].imag() << "\x1B[0m" << "\t";
                }else {
                    std::cout << std::right << std::setw(15) << M[i, j].imag() << "\t";
                }
            }
            std::cout << "  |" << std::endl;
        }
    }
};

int main(int argc, char *argv[]){

    std::cout << "Reading test csv" << std::endl;

    // Pull data from csv, add the directory to visibilities.csv
    DataFile visfile = DataFile("<path-to-data>/visibilities.csv", std::ios::in);
    visfile.ReadCsv();

    // Data can be extracted and cast into most types
    std::vector<std::complex<double>> vis = visfile.GetColumn<double, std::complex>("vis");
    std::vector<int> ant_a = visfile.GetColumn<int>("ant_a");
    std::vector<int> ant_b = visfile.GetColumn<int>("ant_b");

    // Preprocessing
    const unsigned size = vis.size();

    // Length of unrolled matrix
    const int m_size = 100;

    // Length of array
    const int a_size = 10;

    std::array<std::complex <double>, m_size> array;
    array.fill((std::complex<double>{0., 0.}));

    Kokkos::mdspan X = Kokkos::mdspan<std::complex<double>, Kokkos::extents<unsigned int, a_size, a_size>>(array.data(), a_size, a_size);

    for (unsigned i = 0; i < size; ++ i) {
        double count = 1.;

        int m = ant_a[i];
        int n = ant_b[i];

        X[m, n] = X[m, n]*(count - 1) + vis[i];
        X[m, n] = X[m, n]/count;

        X[n, m] = conj(X[m, n]);

        count++;
    }

    auto solver = AntennaSolve<double, m_size, a_size>(array);

    solver.Fit(100, 0.1);

    data::print(X);

    // Get the solutions to compare against. Add path to gains.csv if needed.
    DataFile gainsfile = DataFile("<path-to-data>/gains.csv", std::ios::in);
    gainsfile.ReadCsv();

    std::vector<std::complex<double>> gains = gainsfile.GetColumn<double, std::complex>("gains");

    for( unsigned i = 0; i < 10; i++) std::cout << std::left << std::setw(15) << "[calculated, truth]: " << abs(solver.get_gains()[i]) <<  "\t" << abs(gains[i]) << std::endl;

}