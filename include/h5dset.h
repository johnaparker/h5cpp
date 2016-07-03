#ifndef h5dataset_GUARD_H
#define h5dataset_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>

#include "h5attr.h"

using attr_iter = std::map<std::string, std::unique_ptr<h5attr>>::iterator;
using attr_pair = std::pair<std::string, std::unique_ptr<h5attr>>;

class h5dset {
public:
    h5dset(std::string name, hid_t where, hid_t datatype,
            int drank, hsize_t* dims);

    void create_attribute(std::string name, hid_t datatype, 
            int drank, hsize_t* dims);

    void write(const void* data, hid_t datatype);

private:
    std::string name;
    hid_t dset_id, dataspace_id;
    herr_t status;

    std::map<std::string, std::unique_ptr<h5attr>> attrs;
};

#endif
