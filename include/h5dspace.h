#ifndef h5dspace_GUARD_H
#define h5dspace_GUARD_H

#include "hdf5.h"
#include <vector>

struct dataspace {
    explicit dataspace(std::vector<hsize_t> dims, std::vector<hsize_t> max_dims={},
            std::vector<hsize_t> chunk_dims={}, bool compressed=false);
    explicit dataspace(hid_t dspace_id, hid_t prop_id = H5P_DEFAULT);
    dataspace() = default;

    int drank;
    std::vector<hsize_t> dims, max_dims, chunk_dims;
    bool extendable = false, unlimited = false;
    bool chunked = false, compressed = false;

private:
    void set_bools();
};

#endif
