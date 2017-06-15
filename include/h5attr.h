#ifndef h5attr_GUARD_H
#define h5attr_GUARD_H

#include "hdf5.h"
#include "h5type.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

#include "h5dspace.h"

namespace h5cpp {

extern const int MAX_NAME;

class h5attr {
public:
    h5attr(const h5attr&) = delete;
    h5attr(h5attr&&);
    h5attr& operator=(h5attr&) = delete;
    h5attr& operator=(h5attr&&);
    h5attr(std::string name, hid_t where, dtype datatype_, dspace dataspace = dspace());
    explicit h5attr(hid_t attr_id);

    void write(const void* data) const;

    void read(void* dest) const;

    const dspace get_dspace() const;
    hid_t get_dtype() const;
    std::string get_name() const {return name;}
    std::string get_path() const;

    void close();
    ~h5attr();

private:
    std::string name;

    hid_t attr_id;
    hid_t dspace_id;
    dspace dataspace;
    hid_t datatype;
};

}

#endif
