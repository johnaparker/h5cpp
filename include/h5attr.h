#ifndef h5attr_GUARD_H
#define h5attr_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <map>
#include <vector>




class h5attr {
public:
    h5attr(std::string name, hid_t where, hid_t datatype,
            std::vector<hsize_t> dims);
    ~h5attr() {
        H5Aclose(attr_id);
        H5Sclose(dataspace_id);
    }

private:
    std::string name;
    hid_t attr_id, dataspace_id;
    herr_t status;
};

#endif
