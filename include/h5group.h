#ifndef h5group_GUARD_H
#define h5group_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <map>

#include "h5dset.h"
#include "h5attr.h"



class h5group {
public:
    h5group(std::string name);

    void create_dataset(std::string name, hid_t datatype, 
              int drank, hsize_t* dims);

    void create_attribute(std::string name, hid_t datatype, 
              int drank, hsize_t* dims);

    ~h5group();

private:
    std::string name;
    hid_t group_id;
    herr_t status;
    
    std::map<std::string, h5dset> dsets;
    std::map<std::string, h5attr> attrs;
};

#endif
