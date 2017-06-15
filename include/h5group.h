#ifndef h5group_GUARD_H
#define h5group_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

#include "h5dset.h"
#include "h5attr.h"

namespace h5cpp {

class h5group {
public:
    h5group();
    h5group(const h5group&) = delete;
    h5group(h5group&&);
    h5group& operator=(h5group&) = delete;
    h5group& operator=(h5group&&);
    h5group(std::string name, hid_t where);
    explicit h5group(hid_t group_id);

    h5group create_group(std::string name) const;
    h5dset create_dataset(std::string name, dtype datatype, dspace dataspace = dspace()) const;
    h5dset create_dataset(std::string name, dtypeCompound datatype, dspace dataspace = dspace()) const;
    h5attr create_attribute(std::string name, dtype datatype, dspace dataspace = dspace()) const;

    h5group open_group(std::string name) const;
    h5group open_group(h5ref reference) const;
    h5dset open_dataset(std::string name) const;
    h5dset open_dataset(h5ref reference) const;
    h5attr open_attribute(std::string name) const;
    h5attr open_attribute(hsize_t id) const;

    hsize_t num_attrs() const;     // return number of attributes
    hsize_t num_objects() const;   // return number of groups + number of datasets
    std::string get_object_name(hsize_t idx) const;  // get object name by id
    hsize_t get_object_type(hsize_t idx) const;  // get object type by id

    h5group create_or_open_group(std::string name) const;
    h5dset create_or_open_dataset(std::string name, dtype datatype, dspace dataspace = dspace()) const;

    h5ref create_reference(std::string obj_name) const;

    bool object_exists(std::string name) const;

    std::string get_name() const {return name;}
    std::string get_path() const;

    void close();
    ~h5group();

private:
    std::string name;
    hid_t group_id;
};

}

#endif
