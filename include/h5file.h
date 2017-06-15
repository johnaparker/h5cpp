#ifndef h5file_GUARD_H
#define h5file_GUARD_H

#include "hdf5.h"
#include "mpi.h"
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
    wp,      ///<  create if non-existant; otherwise open
    r,       ///<  read only
    rw       ///<  read-write
};

class h5file {
public:
    h5file();
    h5file(const h5file&);
    h5file(h5file&&);
    h5file& operator=(const h5file&);
    h5file& operator=(h5file&&);
    h5file(std::string name, io flag, bool mpi = false);
    //h5file(const h5file& other);
    //h5file& operator=(const h5file& rhs); 

    h5group create_group(std::string name) const;
    h5dset create_dataset(std::string name, dtype datatype, dspace dataspace = dspace()) const;
    h5dset create_dataset(std::string name, hid_t datatype, dspace dataspace = dspace()) const;
    h5dset create_dataset(std::string name, dtypeCompound datatype, dspace dataspace = dspace()) const;
    h5attr create_attribute(std::string name, dtype datatype, dspace dataspace = dspace()) const;

    h5group open_group(std::string name) const;
    h5group open_group(h5ref reference) const;
    h5dset open_dataset(std::string name) const;
    h5dset open_dataset(h5ref reference) const;
    h5attr open_attribute(std::string name, std::string base="/") const;
    h5attr open_attribute(hsize_t id) const;

    hsize_t num_attrs() const;     // return number of attributes
    hsize_t num_objects() const;   // return number of groups + number of datasets
    std::string get_object_name(hsize_t idx) const;  // get object name by id
    hsize_t get_object_type(hsize_t idx) const;  // get object type by id

    h5group create_or_open_group(std::string name) const;
    h5dset create_or_open_dataset(std::string name, dtype datatype, dspace dataspace) const;
    
    h5ref create_reference(std::string obj_name) const;

    bool object_exists(std::string name) const;

    std::string get_name() const {return filename;}
    std::string get_path() const {return "/";}

    void close();
    ~h5file();

private:
    std::string filename;
    hid_t file_id;
    hid_t prop_id;
};

}

#endif
