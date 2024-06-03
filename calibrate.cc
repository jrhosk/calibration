//
// Created by jhoskins on 5/22/24.
//
#include <iostream>
#include <complex>

#include <DataFile.h>
#include <AntennaSolve.h>
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

    std::cout << "Reading test csv" << std::endl;


    DataFile visfile = DataFile("/users/jhoskins/fornax/Development/c++/calibration/data/visibilities.csv", std::ios::in);
    visfile.ReadCsv();

    //std::vector<std::complex<float>> vis = visfile.GetColumn<float, std::complex>("vis");
    std::vector<std::complex<float>> vis = visfile.GetColumn("vis");

    AntennaSolve solver = AntennaSolve();

    solver.SetVis(vis);
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