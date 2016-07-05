#ifndef h5dspace_GUARD_H
#define h5dspace_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <vector>

#include "h5attr.h"

class h5dspace {
public:
    explicit h5dspace(std::vector<hsize_t> dims, 
               std::vector<hsize_t> max_dims = {});
    explicit h5dspace(hid_t dspace_id);

    h5dspace(const h5dspace&);

    bool isExtendable() const;

    int rank() const;
    
    hid_t id() const;

    ~h5dspace();

private:
    int drank;
    std::vector<hsize_t> dims;
    std::vector<hsize_t> max_dims;
    std::vector<hsize_t> chunk_dims;

    bool extendable = false, unlimited = false;

    hid_t dspace_id;
    herr_t status;
};

#endif
