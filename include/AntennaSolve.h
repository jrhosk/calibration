//
// Created by Josh Hoskins on 5/28/24.
//

#ifndef CALIBRATION_ANTENNASOLVE_H
#define CALIBRATION_ANTENNASOLVE_H

#include <complex>
#include <vector>
#include <cstring>

#include <mdspan.hpp>

#include "CalibrationSolverBase.h"


//template <typename T, std::size_t M, std::size_t N>
//class AntennaSolve: public CalibrationSolverBase<T, M, N> {
template <typename T, std::size_t M, std::size_t N>
class AntennaSolve {
protected:


    std::array<std::complex<T>, M> pModelArray;
    std::array<std::complex<T>, M> pObservedArray;

    std::array<std::complex<T>, N> loss;
    std::array <std::complex<T>, N> pGainsArray;

public:
    explicit AntennaSolve(std::array<std::complex<T>, M>);

    ~AntennaSolve();

    std::array <std::complex<T>, N> GetGains();

    void Normalize();
    void Standardize();

    void Loss();
    void Transform(const char *mode="standardization");
    void Step(T alpha);
    void Fit(unsigned n_batch, T alpha);

};

template<typename T, std::size_t M, std::size_t N>
AntennaSolve<T, M, N>::~AntennaSolve() = default;

template <typename T, std::size_t M, std::size_t N>
AntennaSolve<T, M, N>::AntennaSolve(std::array<std::complex<T>, M> vis) {

    this->pObservedArray = std::move(vis);

    // Fill with arrays initial guesses
    this->pGainsArray.fill(std::complex<T>{0.1, 0.1});
    this->pModelArray.fill(std::complex<T>{1., 0.});
}

template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Step(T alpha) {
    for(unsigned i = 0; i < this->pGainsArray.size(); i++){
        this->pGainsArray[i] = this->pGainsArray[i] + alpha*this->loss[i];
    }
}

template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Normalize() {
    T min = 0.;
    T max = 0.;

    // Normalize visibilities
    auto sModelArray = Kokkos::mdspan(this->pModelArray.data(), N, N);

    // Find extrema
    for (std::size_t i = 0; i < sModelArray.extent(0); i++) {
        for (std::size_t j = 0; j < sModelArray.extent(1); j++) {
            if(sModelArray[i, j] < min) min = sModelArray[i, j];
            if(sModelArray[i, j] > max) max = sModelArray[i, j];
        }
    }

    // Scale data
    for (std::size_t i = 0; i < sModelArray.extent(0); i++) {
        for (std::size_t j = 0; j < sModelArray.extent(1); j++) {
            sModelArray[i, j] = (sModelArray[i, j] - min)/(max - min);
        }
    }
}

template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Standardize() {
    T mean = 0.;
    T stdev = 0.;
    T variance = 0;
    T n;

    // Standardize visibilities
    auto sModelArray = Kokkos::mdspan(this->pModelArray.data(), N, N);

    n = sModelArray.extent(0)*sModelArray.extent(1);

    // Calculate mean
    for (std::size_t i = 0; i < sModelArray.extent(0); i++) {
        for (std::size_t j = 0; j < sModelArray.extent(1); j++) {
            mean += sModelArray[i, j];
        }
    }

    mean = mean/(n);

    // Calculate square root.
    for (std::size_t i = 0; i < sModelArray.extent(0); i++) {
        for (std::size_t j = 0; j < sModelArray.extent(1); j++) {
            variance += power(sModelArray[i, j] - mean, 2);
        }
    }

    stdev = sqrt(variance/(n));

    // Scale data
    for (std::size_t i = 0; i < sModelArray.extent(0); i++) {
        for (std::size_t j = 0; j < sModelArray.extent(1); j++) {
            sModelArray[i, j] = (sModelArray[i, j] - mean)/stdev;
        }
    }
}

template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Transform(const char *mode) {
    // This function still needs some work. Both scaling functions
    // need to be made to work with complex numbers.
    std::cout << "transform: AntennaSolve()" << std::endl;

    if(strcmp(mode, "normalization") == 0){
        this->Normalize();
    }
    else {
        this->Standardize();
    }
}

template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Loss() {
    std::complex<T> numerator;
    std::complex<T> denominator;

    // Create Kokos::mdspan instances of matrices. Naming according to
    // private array: pModelArray, mds instance: sModelArray
    auto sModelArray = Kokkos::mdspan(this->pModelArray.data(), 10, 10);
    auto sObservedArray = Kokkos::mdspan(this->pObservedArray.data(), 10, 10);

    // The weight here is set to be the same as in the example
    auto weight_vector = std::array<std::complex<T>, M> ();
    weight_vector.fill(std::complex<T> {400., 400.});

    // Create Kokkos::mdspan instance of weights.
    auto weight_mdspan = Kokkos::mdspan(weight_vector.data(), sModelArray.extent(0), sModelArray.extent(1));

    this->loss = std::array<std::complex<T>, N> ();

    for(unsigned i = 0; i < sObservedArray.extent(0); i++) {

        numerator = {0.0, 0.0};
        denominator = {0.0, 0.0};

        for(unsigned j = 0; j < sObservedArray.extent(1); j++) {


            if (i==j) continue;

            numerator += sObservedArray[i, j]*this->pGainsArray[j] * conj(sModelArray[i, j]) * weight_mdspan[i, j];
            denominator += conj(sModelArray[i, j]) * (sModelArray[i, j]) * conj(this->pGainsArray[j]) * (this->pGainsArray[j])  * weight_mdspan[i, j];
        }

        this->loss[i] = (numerator/denominator) - this->pGainsArray[i];
    }
}

template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Fit(unsigned n_batch, T alpha){

    std::cout << "fit: AntennaSolve()" << std::endl;

    for(size_t b = 0; b < n_batch; b++) {

        // Compute the loss
        this->Loss();

        // Take a step
        this->Step(alpha);

    }
}

template<typename T, std::size_t M, std::size_t N>
std::array <std::complex<T>, N> AntennaSolve<T, M, N>::GetGains(){
    return this->pGainsArray;
}

#endif //CALIBRATION_ANTENNASOLVE_H
