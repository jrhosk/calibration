//
// Created by Josh Hoskins on 5/28/24.
//

#include <iostream>
#include <cmath>

#include "AntennaSolve.h"
#include <mdspan.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-builtins"



/**
template<typename T, class Extents>
Kokkos::mdspan <std::complex<T>, Extents> AntennaSolve<T, Extents>::GetGains(){
    return this->pGainsArray;
}

//template <typename T, std::size_t M, std::size_t N>
//void AntennaSolve<T, M, N>::SetVis(Kokkos::mdspan<std::complex<T>, M> vis){
//    this->pObservedArray = std::move(vis);
//}

template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Step(T alpha) {
    for(unsigned i = 0; i < this->pGainsArray.size(); i++){
        this->pGainsArray[i] = this->pGainsArray[i] + alpha*this->loss[i];
    }
}

template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Transform() {
    std::cout << "transform: AntennaSolve()" << std::endl;
}


template <typename T, std::size_t M, std::size_t N>
void AntennaSolve<T, M, N>::Loss() {
    std::complex<T> numerator;
    std::complex<T> denominator;

    // Create Kokos::mdspan instances of matrices. Naming according to
    // private array: pModelArray, mds instance: sModelArray
    auto sModelArray = Kokkos::mdspan(this->pModelArray, N, N);
    auto sObservedArray = Kokkos::mdspan(this->pObservedArray, N, N);

    // The weight here is set to be the same as in the example
    auto weight_vector = std::array<std::complex<T>, N*N> (sModelArray.extent(0)*sModelArray.extent(1), std::complex<T> {400., 400.});

    // Create Kokkos::mdspan instance of weights.
    auto weight_mdspan = Kokkos::mdspan(weight_vector, sModelArray.extent(0), sModelArray.extent(1));

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
 **/