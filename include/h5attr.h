#ifndef h5attr_GUARD_H
#define h5attr_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <map>




class h5attr {
public:
    h5attr(std::string name, hid_t where, hid_t datatype,
            int drank, hsize_t* dims);
    ~h5attr();

private:
    std::string name;
    hid_t attr_id;
    herr_t status;
};

#endif
