#include "h5dspace.h"
#include <iostream>

using namespace std;

h5dspace::h5dspace(vector<hsize_t> dims, vector<hsize_t> max_dims):
             dims(dims), max_dims(max_dims) {

    drank = dims.size();
    hsize_t *p_max_dims = nullptr;
    if (max_dims.size() == dims.size())
        p_max_dims = &max_dims[0];

    dspace_id = H5Screate_simple(drank, &dims[0], p_max_dims);

    for (hsize_t i = 0; i != drank; i++) {
        if (max_dims[i] > dims[i])
            extendable = true;
        if (max_dims[i] == H5S_UNLIMITED)
            unlimited = true;
    }
}

h5dspace::h5dspace(hid_t dspace_id): dspace_id(dspace_id) {
    drank = H5Sget_simple_extent_ndims(dspace_id);
    auto p_dims = make_unique<hsize_t[]>(drank);
    auto p_max_dims = make_unique<hsize_t[]>(drank);
    H5Sget_simple_extent_dims(dspace_id, p_dims.get(), p_max_dims.get());

    dims.assign(p_dims.get(), p_dims.get()+drank);
    max_dims.assign(p_max_dims.get(), p_max_dims.get()+drank);
}

h5dspace::h5dspace(const h5dspace& rhs) {
    drank = rhs.drank;
    dims = rhs.dims;
    max_dims = rhs.max_dims;
    chunk_dims = rhs.chunk_dims;

    extendable = rhs.extendable;
    unlimited = rhs.unlimited;

    dspace_id = H5Scopy(rhs.id());
}

hid_t h5dspace::id() const {
    return dspace_id;
}

bool h5dspace::isExtendable() const {
    return (extendable || unlimited);
}

int h5dspace::rank() const {
    return drank;
}


h5dspace::~h5dspace() {
    H5Sclose(dspace_id);
}
