#ifndef h5dspace_GUARD_H
#define h5dspace_GUARD_H

#include "hdf5.h"
#include <vector>

namespace h5cpp {

extern hsize_t inf;
using h5ref = hobj_ref_t; 

struct dspace {
    explicit dspace(std::vector<hsize_t> dims, std::vector<hsize_t> max_dims={},
            std::vector<hsize_t> chunk_dims={}, bool compressed=false);
    explicit dspace(hid_t dspace_id, hid_t prop_id = H5P_DEFAULT);
    dspace() = default;

    hsize_t drank;
    std::vector<hsize_t> dims, max_dims, chunk_dims;
    bool extendable = false, unlimited = false;
    bool chunked = false, compressed = false;

private:
    void set_bools();
};

}

#endif
