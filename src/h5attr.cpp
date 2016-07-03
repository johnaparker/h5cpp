#include "h5attr.h"

using namespace std;


h5attr::h5attr(string name, hid_t where, hid_t datatype,
            vector<hsize_t> dims): name(name) {
    hsize_t drank = dims.size();
    dataspace_id = H5Screate_simple(drank, &dims[0], nullptr);
    attr_id = H5Acreate2(where, name.c_str(), datatype,
                    dataspace_id, H5P_DEFAULT,H5P_DEFAULT);
    //status = H5Aclose(attr_id);
    //status = H5Sclose(dataspace_id);
}
