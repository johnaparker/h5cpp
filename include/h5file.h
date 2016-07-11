#ifndef h5file_GUARD_H
#define h5file_GUARD_H

#include "hdf5.h"
#include <memory>
#include "boost/multi_array.hpp"
#include <string>
#include <vector>
#include <map>

#include "h5group.h"
#include "h5dset.h"
#include "h5attr.h"


namespace h5cpp {

enum class io {
    w,       ///<  over-write
    wn,      ///<  write new; fail if file already exists
    r,       ///<  read only
    rw       ///<  read-write
};

class h5file {
public:
    h5file(std::string name, io flag);

    std::unique_ptr<h5group> create_group(std::string name);
    std::unique_ptr<h5dset> create_dataset(std::string name, dtype datatype, dataspace dspace);
    std::unique_ptr<h5attr> create_attribute(std::string name, dtype datatype, dataspace dspace);

    std::unique_ptr<h5group> open_group(std::string name);
    std::unique_ptr<h5dset> open_dataset(std::string name);
    std::unique_ptr<h5attr> open_attribute(std::string name);
    std::unique_ptr<h5attr> open_attribute_by_name(std::string attr_name,
            std::string name);

    bool object_exists(std::string name);

    ~h5file();

private:
    std::string filename;
    hid_t file_id;
    herr_t status;
};

}

#endif
