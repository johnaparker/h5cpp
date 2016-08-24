#ifndef GUARD_h5err_h
#define GUARD_h5err_h

#include "hdf5.h"

class error_lock {
public:
    error_lock();
    ~error_lock();
    void release();


private:
    H5E_auto2_t func;
    void* client_data;
};

#endif
