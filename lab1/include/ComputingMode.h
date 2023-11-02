#ifndef COMPUTING_MODE_H
#define COMPUTING_MODE_H


namespace Matrix {
    enum class ComputingMode {
        SEQUENCE,
        OPENMP,
        PTHREADS,
        TBB
    };
}


#endif //COMPUTING_MODE_H
