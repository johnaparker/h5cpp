#ifndef h5dataset_GUARD_H
#define h5dataset_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <vector>

#include "h5attr.h"

class h5dset {
public:
    h5dset(std::string name, hid_t where, hid_t datatype,
            std::vector<hsize_t> dims);
    explicit h5dset(hid_t group_id);

    std::unique_ptr<h5attr> create_attribute(std::string name, hid_t datatype, std::vector<hsize_t> dims);

    void write(const void* data);

    ~h5dset();

private:
    std::string name;
    int drank;
    std::vector<hsize_t> dims;
    std::vector<hsize_t> max_dims;

    hid_t dset_id, dataspace_id;
    hid_t datatype;
    herr_t status;
};

#endif
