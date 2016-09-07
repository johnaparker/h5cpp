#ifndef h5group_GUARD_H
#define h5group_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

#include "h5dset.h"
#include "h5attr.h"

namespace h5cpp {

class h5group {
public:
    h5group(std::string name, hid_t where);
    explicit h5group(hid_t group_id);

    h5group create_group(std::string name);
    h5dset create_dataset(std::string name, dtype datatype, dspace dataspace);
    h5dset create_dataset(std::string name, dtypeCompound datatype, dspace dataspace);
    h5attr create_attribute(std::string name, dtype datatype, dspace dataspace = dspace());

    h5group open_group(std::string name);
    h5dset open_dataset(std::string name);
    h5attr open_attribute(std::string name);
    h5attr open_attribute(hsize_t id);

    hsize_t num_attrs();

    h5group create_or_open_group(std::string name);
    h5dset create_or_open_dataset(std::string name, dtype datatype, dspace dataspace);

    bool object_exists(std::string name);

    ~h5group();

private:
    std::string name;
    hid_t group_id;
    herr_t status;
};

}

#endif
