#include "h5dspace.h"
#include <memory>
#include <iostream>

using namespace std;

namespace h5cpp {

hsize_t inf = H5S_UNLIMITED;

dspace::dspace(vector<hsize_t> dims, vector<hsize_t> max_dims_, 
                     vector<hsize_t> chunk_dims, bool compressed):
                dims(dims), max_dims(max_dims_), chunk_dims(chunk_dims),
                compressed(compressed) {
    
    drank = dims.size(); 
    if (max_dims.empty())
        max_dims = dims;
    set_bools();
}

dspace::dspace(hid_t dspace_id, hid_t prop_id) {
    drank = H5Sget_simple_extent_ndims(dspace_id);
    auto p_dims = unique_ptr<hsize_t[]>(new hsize_t[drank]);
    auto p_max_dims = unique_ptr<hsize_t[]>(new hsize_t[drank]);
    H5Sget_simple_extent_dims(dspace_id, p_dims.get(), p_max_dims.get());
    dims.assign(p_dims.get(), p_dims.get() + drank);
    max_dims.assign(p_max_dims.get(), p_max_dims.get() + drank);

    if (prop_id != H5P_DEFAULT && H5Pget_layout(prop_id) == H5D_CHUNKED) {
        auto p_chunk_dims = unique_ptr<hsize_t[]>(new hsize_t[drank]);
        H5Pget_chunk(prop_id, drank, p_chunk_dims.get());
        chunk_dims.assign(p_chunk_dims.get(), p_chunk_dims.get() + drank);
    }
    else {
        chunk_dims = {};
    }
    set_bools();
}


void dspace::set_bools() {
    for (hsize_t i = 0; i != drank; i++) {
        if (max_dims[i] > dims[i])
            extendable = true;
        if (max_dims[i] == H5S_UNLIMITED) {
            unlimited = true;
            extendable = true;
        }
    }
    if (!chunk_dims.empty())
        chunked = true;
}

bool dspace::isScalar() {
    if (dims.size() == 0)
        return true;
    return false;
}

}
