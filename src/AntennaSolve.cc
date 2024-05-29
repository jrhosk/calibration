//
// Created by mystletainn on 5/28/24.
//

#include <iostream>
#include <utility>

#include "AntennaSolve.h"

void AntennaSolve::Transform() {
    std::cout << "transform: AntennaSolve()" << std::endl;

    for (size_t i = 0; i < 900; i++){
        this->pModelArray.push_back(std::complex<float> {1., 0.});
    }

    for (size_t i = 0; i < 10; i++) {
        this->pGainsArray.push_back(std::complex<float>{001., 0.});
    }
}

void AntennaSolve::SetVis(std::vector<std::complex<float>> vis){
    this->pObservedArray = std::move(vis);
}

void AntennaSolve::SetGains(std::vector<std::complex<float>> gains){
    this->pGainsArray = std::move(gains);
}

void AntennaSolve::Fit(int n_batch, float step){
    std::complex<float> t_diff = {0.0, 0.0};
    std::complex<float> t_vis = {0.0, 0.0};
    std::complex<float> t_gain = {0.0, 0.0};

    std::complex<float> numerator;
    std::complex<float> denominator;

    std::vector<std::complex<float>> vis_mean;

    float t_weight = 1/(0.05*0.005);

    std::cout << "fit: AntennaSolve()" << std::endl;
    float N = 100.0;

    // Calculate average values
    for (size_t i = 0; i < 2; i++) {
        t_vis = {0.0, 0.0};
        for( size_t j = 0; j < 100; j++){
            t_vis += this->pObservedArray[100*i + j];
        }

        t_vis /= N;
        vis_mean.push_back(t_vis);
    }
    //for( size_t k = 0; k < 10; k++) std::cout << vis_mean[k] << std::endl;
    //for (size_t i = 0; i < this->pModelArray.size(); i++){
    for(size_t b = 0; b < n_batch; b++) {
        numerator = {0.0, 0.0};
        denominator = {0.0, 0.0};

        for (size_t i = 1; i < 2; i++) {
            numerator += vis_mean[i-1] * (this->pGainsArray[i]) * t_weight;
            denominator += norm((this->pGainsArray[i])) * t_weight; /** norm(this->pModelArray[0])*/
        }
        t_gain = this->pGainsArray[0];
        t_diff = numerator/denominator - t_gain;

        this->pGainsArray[0] = t_gain + step*t_diff;

        //if(b % 10 == 0) std::cout << "Batch: " << b << "\tgain: " << this->pGainsArray[0].real() << " + " << this->pGainsArray[0].imag() << "i" << std::endl;
        if(b % 10 == 0) std::cout << "Batch: " << b << "\tgain: " << this->pGainsArray[0].real() << " + " << this->pGainsArray[0].imag() << "i\t"  << t_diff << std::endl;

        //if(abs(t_gain) < 0.001) break;
    }

    std::cout << "Gain: " << this->pGainsArray[0] << std::endl;
}