#ifndef eigen_GUARD_H
#define eigen_GUARD_H

#include <memory>
#include <string>
#include <vector>
#include "hdf5.h"

#include <eigen3/Eigen/Core>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>

#include "h5attr.h"
#include "h5dspace.h"

#include <typeindex>
#include <typeinfo>
#include <unordered_map>


namespace h5cpp {

    std::unordered_map<std::type_index, dtype> tmap {
        {typeid(double), dtype::Double},
        {typeid(int), dtype::Int},
    };

    template<class T, class G>
    h5cpp::h5dset write_matrix(const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> &m, const G &group, std::string name) {

        //auto shape = std::vector<hsize_t>{static_cast<hsize_t>(m.rows())}
        auto shape = std::vector<hsize_t>{m.rows(), m.cols()};

        auto dspace = h5cpp::dspace(shape);
        auto dset = group.create_dataset(name, tmap[typeid(T)], dspace);
        dset.write(m.data());
        return dset;
    }

    template<class T, h5cpp::dtype M>
    h5cpp::h5dset write_vector(const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> &m, const h5cpp::h5group &group, std::string name) {
        auto shape = std::vector<hsize_t>{m.size()};

        auto dspace = h5cpp::dspace(shape);
        auto dset = group.create_dataset(name, M, dspace);
        dset.write(m.data());
        return dset;
    }


    // write_tensor
    // append functions (append_vector, append_matrix, etc.) (create or open)
    // also, pass in group = root file (should inherit from common)
}

#endif
