#ifndef h5dataset_GUARD_H
#define h5dataset_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <vector>

#include "h5attr.h"
#include "h5dspace.h"


namespace h5cpp {

class h5dset {
public:
    h5dset(std::string name, hid_t where, dtype datatype_, dspace dataspace);
    h5dset(std::string name, hid_t where, hid_t datatype_, dspace dataspace);
    explicit h5dset(hid_t group_id);

    h5attr create_attribute(std::string name, dtype datatype_, dspace dataspace);

    void extend(std::vector<hsize_t> size);
    void write(const void* data);
    void write(const void* data, hid_t h5datatype);
    void select_write(const void* data, std::vector<hsize_t> offset, std::vector<hsize_t> count,
                std::vector<hsize_t> stride={}, std::vector<hsize_t> block={});
    void append(const void* data);

    h5attr open_attribute(std::string name);

    ~h5dset();

private:
    void select(std::vector<hsize_t> offset, std::vector<hsize_t> count,
                std::vector<hsize_t> stride={}, std::vector<hsize_t> block={});

private:
    std::string name;

    hid_t dset_id;
    hid_t dspace_id, filespace, memspace, prop;
    dspace dataspace;
    hid_t datatype;
    herr_t status;
};

}

#endif
