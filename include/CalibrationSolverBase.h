//
// Created by jhoskins on 5/30/24.
//

#ifndef CALIBRATIONSOLVERBASE_H
#define CALIBRATIONSOLVERBASE_H

class CalibrationSolverBase {
public:
    //virtual ~CalibrationSolverBase() = 0;

    virtual void Step() = 0;
    virtual void Transform() = 0;
    virtual std::complex<float> Loss(const std::complex<float> &, const std::complex<float> &) = 0;
    virtual void Fit(int, float) = 0;
};

#endif //CALIBRATIONSOLVERBASE_H
