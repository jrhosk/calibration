//
// Created by Josh Hoskins on 5/28/24.
//

#include <iostream>

#include "AntennaSolve.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-builtins"
AntennaSolve::AntennaSolve(const boost::numeric::ublas::matrix<std::complex<double>> & vis) {
    this->pObservedArray = boost::numeric::ublas::matrix(vis);

    const unsigned size = this->pObservedArray.size1();

    this->pGainsArray = boost::numeric::ublas::vector<std::complex<double>>(size);
    for (unsigned i = 0; i < 10; i++) this->pGainsArray(i) = std::complex<double>{0.1, 0.1};

    this->pModelArray = boost::numeric::ublas::matrix<std::complex<double>>(10, 10);
    for (unsigned i = 0; i < 10; i++) {
        for (unsigned j = 0; j < 10; j++) {
            this->pModelArray(i, j) = std::complex<double>{1., 0.};
        }
    }

}
#pragma clang diagnostic pop

AntennaSolve::~AntennaSolve() = default;

boost::numeric::ublas::vector<std::complex<double>> AntennaSolve::GetGains(){
    return this->pGainsArray;
}

void AntennaSolve::SetVis(boost::numeric::ublas::matrix<std::complex<double>> & vis){
    this->pObservedArray = vis;
}

void AntennaSolve::Step(double alpha) {
    for(unsigned i = 0; i < this->pGainsArray.size(); i++){
        this->pGainsArray(i) = this->pGainsArray(i) + alpha*this->loss(i);
    }
}

void AntennaSolve::Transform() {
    std::cout << "transform: AntennaSolve()" << std::endl;
}

boost::numeric::ublas::vector<std::complex<double>> AntennaSolve::Loss() {
    std::complex<double> numerator;
    std::complex<double> denominator;

    auto t_weight = boost::numeric::ublas::scalar_matrix<std::complex<double>> (this->pModelArray.size1(), this->pModelArray.size2(), std::complex<double> {400., 400.});
    this->loss = boost::numeric::ublas::vector<std::complex<double>> (this->pGainsArray.size());

    for(unsigned i = 0; i < this->pObservedArray.size1(); i++) {

        numerator = {0.0, 0.0};
        denominator = {0.0, 0.0};

        for(unsigned j = 0; j < this->pObservedArray.size2(); j++) {


            if (i==j) continue;

            numerator += this->pObservedArray(i, j)*this->pGainsArray(j) * conj(this->pModelArray(i, j)) * t_weight(i, j);
            denominator += conj(this->pModelArray(i, j)) * (this->pModelArray(i, j)) * conj(this->pGainsArray(j)) * (this->pGainsArray(j))  * t_weight(i, j);
        }

        this->loss(i) = (numerator/denominator) - this->pGainsArray(i);

    }

    return loss;
}

void AntennaSolve::Fit(int n_batch, double alpha){

    std::cout << "fit: AntennaSolve()" << std::endl;

    for(size_t b = 0; b < n_batch; b++) {

        this->Loss();
        this->Step(alpha);

    }
}
