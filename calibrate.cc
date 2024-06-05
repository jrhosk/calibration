//
// Created by jhoskins on 5/22/24.
//
#include <iostream>
#include <complex>

#include <DataFile.h>
#include <AntennaSolve.h>

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>


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
/**
    boost::numeric::ublas::vector<std::complex<double>> v (3);
    for (unsigned i = 0; i < v.size (); ++ i) {
        v (i) = std::complex<double>(2., 1.);
    }

    std::cout << v << std::endl;

    boost::numeric::ublas::vector<std::complex<double>> n (3);

    boost::numeric::ublas::triangular_matrix<std::complex<double>, boost::numeric::ublas::upper> mu (3, 3);
    for (unsigned i = 0; i < mu.size1 (); ++ i) {
        for (unsigned j = i; j < mu.size2 (); ++ j) {
            mu (i, j) = std::complex<double> (i + j, + 1.);
        }
    }

    std::cout << mu.size1() << " " << mu.size2() << " " << mu << std::endl;

    n = boost::numeric::ublas::prod(mu, v);
    std::cout << n << std::endl;
    **/


    std::cout << "Reading test csv" << std::endl;


    DataFile visfile = DataFile("/home/mystletainn/Development/c++/calibration/data/visibilities.csv", std::ios::in);
    visfile.ReadCsv();

    std::vector<std::complex<double>> vis = visfile.GetColumn<double, std::complex>("vis");
    std::vector<int> ant_a = visfile.GetColumn<int>("ant_a");
    std::vector<int> ant_b = visfile.GetColumn<int>("ant_b");

    const unsigned size = vis.size();

    boost::numeric::ublas::triangular_matrix<std::complex<double>, boost::numeric::ublas::upper> X (size, size);
    for (unsigned i = 0; i < X.size1 (); ++ i) {
        int m = ant_a[i];
        int n = ant_b[i];

        X (m, n) = X (m, n) + vis[i];
        //X (m, n) /=100.;

    }
    X /= 100.;

    std::cout << X(0, 1).real() << " " << X(0, 1).imag() << std::endl;


    //for( auto & it: vis){
    //    std::cout << it.real() << " " << it.imag() << std::endl;
    //}
    //std::vector<std::complex<float>> vis = visfile.GetColumn("vis");


    //std::complex<float> t_vis = {0.0, 0.0};
/**
    std::vector<std::complex<float>> mean_vis;

    const float N = 100.;

    // Calculate means
    for (size_t i = 0; i < 2; i++) {
        t_vis = {0.0, 0.0};
        for( size_t j = 0; j < 100; j++){
            t_vis += vis[100*i + j];
        }

        t_vis /= N;
        mean_vis.push_back(t_vis);
    }

    AntennaSolve solver = AntennaSolve();

    solver.SetVis(mean_vis);
    solver.Transform();
    solver.Fit(1000, 0.1);

    std::cout << "calculated: " << abs(solver.GetGains()[0]) << std::endl;

    DataFile gainsfile = DataFile("/users/jhoskins/fornax/Development/c++/calibration/data/gains.csv", std::ios::in);
    gainsfile.ReadCsv();

    std::vector<std::complex<float>> gains = gainsfile.GetColumn("gains");
    std::cout <<  "truth: " << abs(gains[0]) << std::endl;

    /**
    data::column<float, std::complex> d_column;
    d_column.data = std::vector <std::complex<float>> {
        std::complex<float>{3., 2.},
        std::complex<float>{2., 3.},
        std::complex<float>{1., 4.}
    };

    std::cout << "Statistics: " << d_column.mean() << " +- " << d_column.stdev() << std::endl;
    **/
}