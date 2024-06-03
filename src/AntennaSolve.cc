//
// Created by mystletainn on 5/28/24.
//

#include <iostream>
#include <utility>

#include "AntennaSolve.h"

AntennaSolve::~AntennaSolve() = default;

std::vector<std::complex<float>> AntennaSolve::GetGains(){
    return this->pGainsArray;
}

void AntennaSolve::SetVis(std::vector<std::complex<float>> vis){
    this->pObservedArray = std::move(vis);
}

void AntennaSolve::SetGains(std::vector<std::complex<float>> gains){
    this->pGainsArray = std::move(gains);
}

void AntennaSolve::Step() {
    return;
}

void AntennaSolve::Transform() {
    std::cout << "transform: AntennaSolve()" << std::endl;

    for (size_t i = 0; i < 900; i++){
        this->pModelArray.push_back(std::complex<float> {1., 0.});
    }

    for (size_t i = 0; i < 10; i++) {
        this->pGainsArray.push_back(std::complex<float>{1.0, 0.});
    }
}

std::complex<float> AntennaSolve::Loss(const std::complex<float> & vis, const std::complex<float> & gain) {
    std::complex<float> numerator = {0.0, 0.0};
    std::complex<float> denominator = {0.0, 0.0};

    std::complex<float> frac = {0.0, 0.0};

    constexpr float t_weight = 1/(0.05*0.05);

    numerator += conj(vis) * (gain) * conj(this->pModelArray[0]) * t_weight;
    denominator += norm(gain) * norm(this->pModelArray[0]) * this->pModelArray[0] * conj(this->pModelArray[0]) * t_weight;
    // norm(this->pModelArray[0])

    //for (size_t i = 1; i < 2; i++) {
    //    numerator += vis_mean[i-1] * (this->pGainsArray[i]) * t_weight;
    //    denominator += norm((this->pGainsArray[i])) * t_weight; /** norm(this->pModelArray[0])**/
    //}

    frac = (numerator/denominator);
    std::cout << ">>\tmodel: " << this->pModelArray[0] << std::endl;
    std::cout << ">>\tmean vis: " << vis << std::endl;
    std::cout << ">>\tgain: " << gain << std::endl;
    std::cout << ">>\tfraction: " << frac << std::endl;

    return frac - gain;
}

void AntennaSolve::Fit(int n_batch, float step){
    std::complex<float> t_diff_0 = {0.0, 0.0};
    std::complex<float> t_diff_1 = {0.0, 0.0};

    std::complex<float> t_gain_0 = {0.0, 0.0};
    std::complex<float> t_gain_1 = {0.0, 0.0};

    std::complex<float> t_vis = {0.0, 0.0};

    std::vector<std::complex<float>> vis_mean;


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

        t_diff_0 = this->Loss(vis_mean[0], this->pGainsArray[1]);
        t_gain_0 = this->pGainsArray[0];


        t_diff_1 = this->Loss(vis_mean[0], this->pGainsArray[0]);
        t_gain_1 = this->pGainsArray[1];

        this->pGainsArray[0] = t_gain_0 + step*t_diff_0;
        this->pGainsArray[1] = t_gain_1 + step*t_diff_1;

        std::cout << "Batch: " << b << "\tgain[0]: " << norm(this->pGainsArray[0]) << "\tdiff: " << t_diff_0 << std::endl;
        std::cout << "Batch: " << b << "\tgain[1]: " << norm(this->pGainsArray[1]) << "\tdiff: " << t_diff_1 << std::endl;

        if(abs(t_diff_0) < 0.01) break;
    }
}