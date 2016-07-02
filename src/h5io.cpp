#include "h5io.h"

using namespace std;

h5io::h5io(string name, unsigned flags): filename(name) {
    file_id = H5Fcreate(name.c_str(), flags, H5P_DEFAULT,
            H5P_DEFAULT);
}

void h5io::create_dataset(string name, hid_t where, hid_t datatype,
            int drank, hsize_t* dims) {
    dataspace_id = H5Screate_simple(drank, dims, nullptr);
    dataset_id = H5Dcreate2(where, name.c_str(), datatype,
                    dataspace_id, H5P_DEFAULT,H5P_DEFAULT,
                    H5P_DEFAULT);
    status = H5Dclose(dataset_id);
    status = H5Sclose(dataspace_id);

}

void h5io::create_attribute(string name, hid_t where, hid_t datatype,
            int drank, hsize_t* dims) {
    dataspace_id = H5Screate_simple(drank, dims, nullptr);
    attribute_id = H5Acreate2(where, name.c_str(), datatype,
                    dataspace_id, H5P_DEFAULT,H5P_DEFAULT);
    status = H5Aclose(attribute_id);
    status = H5Sclose(dataspace_id);
}

h5io::~h5io() {
    H5Fclose(file_id);
}
