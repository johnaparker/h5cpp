#ifndef h5dataset_GUARD_H
#define h5dataset_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <vector>

#include "h5attr.h"
#include "h5dspace.h"


namespace h5cpp {

class h5dset {
    friend class h5file;
    friend class h5group;

public:
    h5dset();
    h5dset(std::string name, hid_t where, dtype datatype_, dspace dataspace);
    h5dset(std::string name, hid_t where, hid_t datatype_, dspace dataspace);
    h5dset(const h5dset&) = delete;
    h5dset(h5dset&&);
    h5dset& operator=(h5dset&) = delete;
    h5dset& operator=(h5dset&&);
    explicit h5dset(hid_t group_id);

    h5attr create_attribute(std::string name, dtype datatype_, dspace dataspace = dspace());

    void extend(std::vector<hsize_t> size);
    void write(const void* data);
    void select_write(const void* data, std::vector<hsize_t> offset, std::vector<hsize_t> count,
                std::vector<hsize_t> stride={}, std::vector<hsize_t> block={});
    void append(const void* data);

    void read(void* dest);
    void select_read(void* dest, std::vector<hsize_t> offset, std::vector<hsize_t> count,
                std::vector<hsize_t> stride={}, std::vector<hsize_t> block={});

    h5attr open_attribute(std::string name);
    h5attr open_attribute(hsize_t id);
    hsize_t num_attrs();

    const dspace get_dspace();
    hid_t get_dtype();
    const std::string get_name();

    void close();
    ~h5dset();

private:
    void select(std::vector<hsize_t> offset, std::vector<hsize_t> count,
                std::vector<hsize_t> stride={}, std::vector<hsize_t> block={});

private:
    std::string name;

    hid_t dset_id;
    hid_t dspace_id, filespace, memspace, prop;
    dspace dataspace;
    hid_t datatype;
    herr_t status;
};

}

#endif
