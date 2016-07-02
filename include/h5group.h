#ifndef h5group_GUARD_H
#define h5group_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <map>

#include "h5dset.h"
#include "h5attr.h"


using dset_iter = std::map<std::string, std::unique_ptr<h5dset>>::iterator;
using dset_pair = std::pair<std::string, std::unique_ptr<h5dset>>;

class h5group {
public:
    h5group(std::string name, hid_t where);

    void create_dataset(std::string name, hid_t datatype, 
              int drank, hsize_t* dims);

    void create_attribute(std::string name, hid_t datatype, 
              int drank, hsize_t* dims);

private:
    std::string name;
    hid_t group_id;
    herr_t status;
    
    std::map<std::string, std::unique_ptr<h5dset>> dsets;
    std::map<std::string, std::unique_ptr<h5attr>> attrs;
};

#endif
