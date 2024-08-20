// ******************************************************************* //
//
// Created by Joshua Hoskins (jhoskins@nrao.edu) 5/22/24.
//
//   Style guide:
//             https://google.github.io/styleguide/cppguide.html
//
// ******************************************************************* //

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


    std::array<std::complex<T>, M> model_array_;
    std::array<std::complex<T>, M> observed_array_;

    std::array<std::complex<T>, N> loss_;
    std::array <std::complex<T>, N> gains_array_;

public:
    explicit AntennaSolve(std::array<std::complex<T>, M>);

    ~AntennaSolve();

    std::array <std::complex<T>, N> get_gains();

    void Normalize();
    //void Standardize();

    void Loss();
    void Transform(const char *mode="standardization");
    void Step(T alpha);
    void Fit(unsigned n_batch, T alpha);

};

template<typename T, std::size_t M, std::size_t N>
AntennaSolve<T, M, N>::~AntennaSolve() = default;

template <typename T, std::size_t M, std::size_t N>
AntennaSolve<T, M, N>::AntennaSolve(std::array<std::complex<T>, M> vis) {

    this->observed_array_ = std::move(vis);

    // Fill with arrays initial guesses
    this->gains_array_.fill(std::complex<T>{0.1, 0.1});
    this->model_array_.fill(std::complex<T>{1., 0.});
}

template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Step(T alpha) {
    for(std::size_t i = 0; i < this->gains_array_.size(); i++){
        this->gains_array_[i] = this->gains_array_[i] + alpha*this->loss_[i];
    }
}

template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Normalize() {
    // This isn't working yet
    T min_real = 0.;
    T max_real = 0.;

    T min_imag = 0.;
    T max_imag = 0.;

    // Normalize visibilities
    auto model_array = Kokkos::mdspan(this->model_array_.data(), N, N);

    // Find extrema
    for (std::size_t i = 0; i < model_array.extent(0); i++) {
        for (std::size_t j = 0; j < model_array.extent(1); j++) {
            if(model_array[i, j].real() < min_real) min_real = model_array[i, j].real();
            if(model_array[i, j].real() > max_real) max_real = model_array[i, j].real();

            if(model_array[i, j].imag() < min_real) min_imag = model_array[i, j].imag();
            if(model_array[i, j].imag() > max_real) max_imag = model_array[i, j].imag();
        }
    }

    // Scale data
    for (std::size_t i = 0; i < model_array.extent(0); i++) {
        for (std::size_t j = 0; j < model_array.extent(1); j++) {
            std::complex<T> new_value = std::complex<T>( {(model_array[i, j].real() - min_real)/(max_real - min_real), (model_array[i, j].imag() - min_imag)/(max_imag - min_imag)} );

            model_array[i, j] = new_value;
        }
    }
}

/**
template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Standardize() {
    T mean = 0.;
    T stdev = 0.;
    T variance = 0;
    T n;

    // Standardize visibilities
    auto model_array = Kokkos::mdspan(this->model_array_.data(), N, N);

    n = model_array.extent(0)*model_array.extent(1);

    // Calculate mean
    for (std::size_t i = 0; i < model_array.extent(0); i++) {
        for (std::size_t j = 0; j < model_array.extent(1); j++) {
            mean += model_array[i, j];
        }
    }

    mean = mean/(n);

    // Calculate square root.
    for (std::size_t i = 0; i < model_array.extent(0); i++) {
        for (std::size_t j = 0; j < model_array.extent(1); j++) {
            variance += power(model_array[i, j] - mean, 2);
        }
    }

    stdev = sqrt(variance/(n));

    // Scale data
    for (std::size_t i = 0; i < model_array.extent(0); i++) {
        for (std::size_t j = 0; j < model_array.extent(1); j++) {
            model_array[i, j] = (model_array[i, j] - mean)/stdev;
        }
    }
}
**/
template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Transform(const char *mode) {
    // This function still needs some work. Both scaling functions
    // need to be made to work with complex numbers.
    std::cout << "transform: AntennaSolve()" << std::endl;

    if(strcmp(mode, "normalization") == 0){
        this->Normalize();
    }
    else {
        //this->Standardize();
        return;
    }
}

template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Loss() {
    std::complex<T> numerator;
    std::complex<T> denominator;

    // Create Kokos::mdspan instances of matrices. Naming according to
    // private array: model_array_, mds instance: model_array
    auto model_array = Kokkos::mdspan(this->model_array_.data(), 10, 10);
    auto sObservedArray = Kokkos::mdspan(this->observed_array_.data(), 10, 10);

    // The weight here is set to be the same as in the example
    auto weight_vector = std::array<std::complex<T>, M> ();
    weight_vector.fill(std::complex<T> {400., 400.});

    // Create Kokkos::mdspan instance of weights.
    auto weight_mdspan = Kokkos::mdspan(weight_vector.data(), model_array.extent(0), model_array.extent(1));

    this->loss_ = std::array<std::complex<T>, N> ();

    for(std::size_t i = 0; i < sObservedArray.extent(0); i++) {

        numerator = {0.0, 0.0};
        denominator = {0.0, 0.0};

        for(std::size_t j = 0; j < sObservedArray.extent(1); j++) {


            if (i==j) continue;

            numerator += sObservedArray[i, j]*this->gains_array_[j] * conj(model_array[i, j]) * weight_mdspan[i, j];
            denominator += conj(model_array[i, j]) * (model_array[i, j]) * conj(this->gains_array_[j]) * (this->gains_array_[j])  * weight_mdspan[i, j];
        }

        this->loss_[i] = (numerator/denominator) - this->gains_array_[i];
    }
}

template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Fit(unsigned n_batch, T alpha){

    std::cout << "fit: AntennaSolve()" << std::endl;

    for(std::size_t b = 0; b < n_batch; b++) {

        // Compute the loss
        this->Loss();

        // Take a step
        this->Step(alpha);

    }
}

template<typename T, std::size_t M, std::size_t N>
std::array <std::complex<T>, N> AntennaSolve<T, M, N>::get_gains(){
    return this->gains_array_;
}

#endif //CALIBRATION_ANTENNASOLVE_H
