//
// Created by jhoskins on 5/30/24.
//

#ifndef CALIBRATIONSOLVERBASE_H
#define CALIBRATIONSOLVERBASE_H

template <typename T, std::size_t M, std::size_t N>
class CalibrationSolverBase {
public:
    virtual ~CalibrationSolverBase() = 0;

    virtual void Step(T alpha) = 0;
    virtual void Transform() = 0;
    virtual void Loss() = 0;
    virtual void Fit(unsigned int, T) = 0;
};

#endif //CALIBRATIONSOLVERBASE_H
