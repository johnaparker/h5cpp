#ifndef h5file_GUARD_H
#define h5file_GUARD_H

#include "hdf5.h"
#include <memory>
#include "boost/multi_array.hpp"
#include <string>
#include <map>

#include "h5group.h"
#include "h5dset.h"
#include "h5attr.h"


class h5file {
public:
    h5file(std::string name, unsigned flags = H5F_ACC_TRUNC);
    void create_group(std::string name);
    void create_dataset(std::string name, std::string where, 
            hid_t datatype, int drank, hsize_t* dims);
    void create_attribute(std::string name, std::string where, 
            hid_t datatype, int drank, hsize_t* dims);

    ~h5file();


private:
    std::string filename;
    hid_t file_id;
    herr_t status;

    std::map<std::string, h5group> groups;
    std::map<std::string, h5dset> dsets;
    std::map<std::string, h5attr> attrs;
};






#endif
