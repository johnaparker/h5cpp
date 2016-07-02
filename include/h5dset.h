#ifndef h5dataset_GUARD_H
#define h5dataset_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>

#include "h5attr.h"


class h5dset {
public:
    h5dset(std::string name, hid_t where, hid_t datatype,
            int drank, hsize_t* dims);

    void create_attribute(std::string name, hid_t datatype, 
            int drank, hsize_t* dims);

    ~h5dset();

private:
    std::string name;
    hid_t dataset_id;
    herr_t status;

    std::map<std::string, h5attr> attrs;
};

#endif
