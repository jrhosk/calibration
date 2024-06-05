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


    //std::complex <double> t_mean {0., 0.};
    //for(unsigned i = 0; i < 100; i++){
    //    t_mean += vis[i];
    //}

    //std::cout << t_mean/100. << std::endl;

    // Preprocessing
    const unsigned size = vis.size();

    boost::numeric::ublas::triangular_matrix<std::complex<double>, boost::numeric::ublas::upper> X (10, 10);
    for (unsigned i = 0; i < size; ++ i) {
        int m = ant_a[i];
        int n = ant_b[i];

        X (m, n) = X (m, n) + vis[i];
    }

    X /= 100.;


    //std::cout << X(0, 1) << std::endl;

    boost::numeric::ublas::scalar_matrix<std::complex<double>> s (3, 3, std::complex<double> {1., 1.});
    //std::cout << "Scalar matrix:\n" << s << std::endl;

    AntennaSolve solver = AntennaSolve(X);

    //solver.SetVis(mean_vis);
    //solver.Transform();

    solver.Fit(100, 0.1);

    //std::cout << "calculated: " << abs(solver.GetGains()[0]) << std::endl;

    //DataFile gainsfile = DataFile("/users/jhoskins/fornax/Development/c++/calibration/data/gains.csv", std::ios::in);
    //gainsfile.ReadCsv();

    //std::vector<std::complex<float>> gains = gainsfile.GetColumn("gains");
    //std::cout <<  "truth: " << abs(gains[0]) << std::endl;

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