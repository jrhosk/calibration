//
// Created by jhoskins on 5/22/24.
//
#include <iostream>
#include <complex>

#include <DataFile.h>
#include <AntennaSolve.h>

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <iomanip>


//
// Model rho_ij: std::vector <complex> model[N] = 1 + 0.0j
//

namespace data{
    /*
    template <typename T>
    class column {
    public:
        std::vector<T> data;

        T mean() {
            if(!data.empty()) {
                T N = 0;
                T mean = 0;
                for(auto & it : this->data) {
                    mean += it;
                    ++N;
                }
                return mean / N;
            }
            return 0.0;

        }
    };
*/
    void print(boost::numeric::ublas::matrix<std::complex<double>> M){
        const unsigned m = M.size1();
        const unsigned n = M.size2();

        std::cout << "Real Component:" << std::endl;

        for(unsigned i = 0; i < m; i++){
            std::cout << "| ";
            for(unsigned j = 0; j < n; j++) {
                if(i==j){
                    std::cout << std::right << std::setw(15) << "\x1B[31m" <<  M(i, j).imag() << "\x1B[0m" << "\t";
                }else {
                    std::cout << std::right << std::setw(15) << M(i, j).real() << "\t";
                }
            }
            std::cout << "  |" << std::endl;
        }

        std::cout << "\n\nImaginary Component:" << std::endl;
        for(unsigned i = 0; i < m; i++){
            std::cout << "| ";
            for(unsigned j = 0; j < n; j++) {
                if(i==j){
                    std::cout << std::right << std::setw(15) << "\x1B[31m" <<  M(i, j).imag() << "\x1B[0m" << "\t";
                }else {
                    std::cout << std::right << std::setw(15) << M(i, j).imag() << "\t";
                }
            }
            std::cout << "  |" << std::endl;
        }

}



    template <typename T, template <typename> class complex>
    class column {
    public:
        std::vector<std::complex<T>> data;

        std::complex<T> mean() {
            if(!data.empty()) {
                std::complex<T> N = this->data.size();
                std::complex<T> mean = {0., 0.};
                for(auto & it : this->data) {
                    mean += it;
                    //++N;
                }
                return mean / N;
            }
            return std::complex<T> {0., 0.};

        }

        std::complex<T> stdev() {
            if(!data.empty()) {
                std::complex<T> std = {0., 0.};
                std::complex<T> mean = this->mean();
                std::complex<T> N = this->data.size();

                for(auto & it : this->data) {
                    std += pow(it - mean, 2);
                    //++N;
                }
                return sqrt(std / N);
            }
            return std::complex<T> {0., 0.};

        }
    };
};

int main(int argc, char *argv[]){

    std::cout << "Reading test csv" << std::endl;


    DataFile visfile = DataFile("/home/mystletainn/Development/c++/calibration/data/visibilities.csv", std::ios::in);
    visfile.ReadCsv();

    std::vector<std::complex<double>> vis = visfile.GetColumn<double, std::complex>("vis");
    std::vector<int> ant_a = visfile.GetColumn<int>("ant_a");
    std::vector<int> ant_b = visfile.GetColumn<int>("ant_b");


    // Preprocessing
    const unsigned size = vis.size();

    boost::numeric::ublas::matrix<std::complex<double>> X (10, 10);

    // Initialize zero
    for(unsigned i = 0; i < 10; i++) {
        for (unsigned j = 0; j < 10; j++) {
            X(i, j) = std::complex<double> {0., 0.};
        }
    }

    for (unsigned i = 0; i < size; ++ i) {
        int m = ant_a[i];
        int n = ant_b[i];


        X (m, n) = X (m, n) + vis[i];
        X (n, m) = conj(X (m, n));
    }

    X /= 100.;

    AntennaSolve solver = AntennaSolve(X);

    solver.Fit(100, 0.1);

    data::print(X);

    DataFile gainsfile = DataFile("/home/mystletainn/Development/c++/calibration/data/gains.csv", std::ios::in);
    gainsfile.ReadCsv();

    //std::vector<std::complex<double>> gains = gainsfile.GetColumn("gains");
    std::vector<std::complex<double>> gains = gainsfile.GetColumn<double, std::complex>("gains");

    for( unsigned i = 0; i < 10; i++) std::cout << std::left << std::setw(15) << "[calculated, truth]: " << abs(solver.GetGains()[i]) <<  "\t" << abs(gains[i]) << std::endl;

}