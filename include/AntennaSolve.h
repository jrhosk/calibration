//
// Created by Josh Hoskins on 5/28/24.
//

#ifndef CALIBRATION_ANTENNASOLVE_H
#define CALIBRATION_ANTENNASOLVE_H

#include <complex>
#include <vector>
#include <mdspan.hpp>

#include "CalibrationSolverBase.h"


//template <typename T, std::size_t M, std::size_t N>
//class AntennaSolve: public CalibrationSolverBase<T, M, N> {
template <typename T, std::size_t M, std::size_t N>
class AntennaSolve {
protected:

    //Kokkos::mdspan<std::complex<T>, Kokkos::extents<unsigned int, N, N>> pModelArray;
    std::array<std::complex<T>, M> pModelArray;

    //Kokkos::mdspan<std::complex<T>, Kokkos::extents<unsigned int, N, N>> pObservedArray;
    std::array<std::complex<T>, M> pObservedArray;

    std::array <std::complex<T>, N> pGainsArray;


    std::array<std::complex<T>, N> loss;

public:
    explicit AntennaSolve(std::array<std::complex<T>, M>);

    ~AntennaSolve();

    //template<T, Extents>
    //Kokkos::mdspan <std::complex<T>, Extents> GetGains();

    //template<T, Extents>
    //void SetVis(std::array<std::complex<T>, N>);

    void Loss();
    void Test();

    void Transform();

    void Step(T alpha);
    void Fit(unsigned n_batch, T alpha);

    //Kokkos::mdspan<std::complex<T>, Extents> GetGains();

    //void SetVis(Kokkos::mdspan<std::complex<T>, Extents> &vis);
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
void AntennaSolve<T, M, N>::Transform() {
    std::cout << "transform: AntennaSolve()" << std::endl;
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

#endif //CALIBRATION_ANTENNASOLVE_H
