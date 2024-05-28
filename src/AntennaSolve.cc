//
// Created by mystletainn on 5/28/24.
//

#include <iostream>

#include "AntennaSolve.h"

void AntennaSolve::Transform() {
    for
};

void AntennaSolve::Fit(int n_batch, float step){
    std::complex<float> t_diff = {0.0, 0.0};
    std::complex<float> t_gain = {0.0, 0.0};
    std::complex<float> numerator = {0.0, 0.0};
    std::complex<float> denominator = {0.0, 0.0};

    float t_weight = 400;

    //for (size_t i = 0; i < this->pModelArray.size(); i++){
    for(size_t b = 0; b < n_batch; n_batch++) {
        for (size_t i = 0; i < 100; i++) {
            numerator += conj(this->pObservedArray[i]) * (this->pGainsArray[0]) * (this->pModelArray[i]) * t_weight;
            denominator += norm((this->pGainsArray[0])) * norm(this->pModelArray[i]) * t_weight;
        }

        t_gain = this->pGainsArray[0];
        this->pGainsArray[0] = t_gain + step*(numerator/denominator - t_gain);

        std::cout << "Batch: " << b << "\tgain: " << this->pGainsArray[0] << std::endl;
    }
};