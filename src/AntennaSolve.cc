//
// Created by mystletainn on 5/28/24.
//

#include <iostream>
#include <utility>

#include "AntennaSolve.h"

AntennaSolve::AntennaSolve(boost::numeric::ublas::matrix<std::complex<double>> & vis) {
    this->pObservedArray = boost::numeric::ublas::matrix<std::complex<double>>(vis);

    const unsigned size = this->pObservedArray.size1();

    this->pGainsArray = boost::numeric::ublas::vector<std::complex<double>>(size);
    for (unsigned i = 0; i < 10; i++) this->pGainsArray(i) = std::complex<double>{0.1, 0.1};

    //this->pModelArray = boost::numeric::ublas::triangular_matrix<std::complex<double>, boost::numeric::ublas::upper> (size, size);
    //this->pModelArray = boost::numeric::ublas::scalar_matrix<std::complex<double>> (10, 10, std::complex<double> {1., 0.});

    this->pModelArray = boost::numeric::ublas::matrix<std::complex<double>>(10, 10);
    for (unsigned i = 0; i < 10; i++) {
        for (unsigned j = 0; j < 10; j++) {
            this->pModelArray(i, j) = std::complex<double>{1., 0.};
        }
    }

}

AntennaSolve::~AntennaSolve() = default;

boost::numeric::ublas::vector<std::complex<double>> AntennaSolve::GetGains(){
    return this->pGainsArray;
}

void AntennaSolve::SetVis(boost::numeric::ublas::matrix<std::complex<double>> & vis){
    this->pObservedArray = vis;
}

//void AntennaSolve::SetGains(std::vector<std::complex<float>> gains){
//    this->pGainsArray = std::move(gains);
//}

void AntennaSolve::Step(double alpha) {
    for(unsigned i = 0; i < this->pGainsArray.size(); i++){
        this->pGainsArray(i) = this->pGainsArray(i) + alpha*this->loss(i);
    }
}

void AntennaSolve::Transform() {
    std::cout << "transform: AntennaSolve()" << std::endl;
    /**
    for (size_t i = 0; i < 900; i++){
        this->pModelArray.push_back(std::complex<float> {1., 0.});
    }

    for (size_t i = 0; i < 10; i++) {
        this->pGainsArray.push_back(std::complex<float>{1.0, 0.});
    }
     **/
}

boost::numeric::ublas::vector<std::complex<double>> AntennaSolve::Loss() {
    std::complex<double> numerator;
    std::complex<double> denominator;

    unsigned m = 0;
    unsigned n = 0;

    unsigned k = 0;


    //
    // constexpr float t_weight = 1/(0.05*0.05);
    auto t_weight = boost::numeric::ublas::scalar_matrix<std::complex<double>> (10, 10, std::complex<double> {400., 400.});
    this->loss = boost::numeric::ublas::vector<std::complex<double>> (10);

    //for(unsigned i = 0; i < this->pObservedArray.size1(); i++) {
    for(unsigned i = 0; i < 10; i++) {
        numerator = {0.0, 0.0};
        denominator = {0.0, 0.0};

        //for(unsigned j = 0; j < this->pObservedArray.size2(); j++) {
        for(unsigned j = 0; j < 10; j++) {

            if (i==j) continue;

            //std::cout << "(i, j): " << i << " " << j << "\t(m, n): " << m << " " << n << std::endl;

            numerator += this->pObservedArray(i, j)*this->pGainsArray(j) * conj(this->pModelArray(i, j)) * t_weight(i, j);
            denominator += conj(this->pModelArray(i, j)) * (this->pModelArray(i, j)) * conj(this->pGainsArray(j)) * (this->pGainsArray(j))  * t_weight(i, j);

            //std::cout  << "(i, j): " << i  << "\t" << j << ": " <<  (numerator/denominator) << std::endl;
        }

        this->loss(i) = (numerator/denominator) - this->pGainsArray(i);


        /**
            numerator += conj(this->pObservedArray(m, n)), (this->pGainsArray(n)) * conj(this->pModelArray(m, n)) *
                                                           t_weight(m, n);
            denominator +=
                    norm(this->pGainsArray(n)) * norm(this->pModelArray(m, n)) * this->pModelArray(m, n) *
                    conj(this->pModelArray(m, n)) * t_weight(m, n);
        }
        this->loss(i) = (numerator/denominator) - this->pGainsArray(i);
        **/
    }

    return loss;
}

void AntennaSolve::Fit(int n_batch, double alpha){
    std::complex<float> t_diff_0 = {0.0, 0.0};
    std::complex<float> t_diff_1 = {0.0, 0.0};

    std::complex<float> t_gain_0 = {0.0, 0.0};
    std::complex<float> t_gain_1 = {0.0, 0.0};

    std::complex<float> t_vis = {0.0, 0.0};

    //std::vector<std::complex<float>> vis_mean;


    std::cout << "fit: AntennaSolve()" << std::endl;
    //float N = 100.0;

    // Calculate average values
    //for (size_t i = 0; i < 2; i++) {
    //    t_vis = {0.0, 0.0};
    //    for( size_t j = 0; j < 100; j++){
    //        t_vis += this->pObservedArray[100*i + j];
    //    }

    //    t_vis /= N;
    //    vis_mean.push_back(t_vis);
    //}

    //for( size_t k = 0; k < 10; k++) std::cout << vis_mean[k] << std::endl;
    //for (size_t i = 0; i < this->pModelArray.size(); i++){
    for(size_t b = 0; b < n_batch; b++) {

        this->Loss();
        this->Step(alpha);


        //if((n_batch % 100) == 0) std::cout << "Gains:\t" << norm(this->pGainsArray(0)) << "\t" << norm(this->pGainsArray(1)) << std::endl;

        //std::cout << "Batch: " << b << "\tgain[0]: " << norm(this->pGainsArray[0]) << "\tdiff: " << t_diff_0 << std::endl;
        //std::cout << "Batch: " << b << "\tgain[1]: " << norm(this->pGainsArray[1]) << "\tdiff: " << t_diff_1 << std::endl;

        //if(abs(t_diff_0) < 0.01) break;
    }
}
