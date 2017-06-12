#ifndef eigen_GUARD_H
#define eigen_GUARD_H

#include <memory>
#include <string>
#include <vector>
#include "hdf5.h"

#include <eigen3/Eigen/Core>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>

#include "h5cpp.h"

#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#define DSET_TEMPLATE_CODE auto dspace = get_eigen_dspace(dims, a); \
        auto dset = group.create_dataset(name, type_map[typeid(T)], dspace); \
        dset.write(m.data()); \
        return dset;

namespace h5cpp {

    extern std::unordered_map<std::type_index, dtype> type_map;

    enum class append: bool {
        True = true,
        False = false
    };

    h5cpp::dspace get_eigen_dspace(std::vector<hsize_t> dims, append a);

    template<class T, class G>
    h5cpp::h5dset write_matrix(const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> &m, const G &group, std::string name, append a = append::False) {
        auto dims = std::vector<hsize_t>{m.rows(), m.cols()};
        DSET_TEMPLATE_CODE
    }

    template<class T, class G>
    h5cpp::h5dset write_vector(const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> &m, const G &group, std::string name, append a = append::False) {
        auto dims = std::vector<hsize_t>{m.size()};
        DSET_TEMPLATE_CODE
    }

    template<class T, class G>
    h5cpp::h5dset write_array(const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> &m, const G &group, std::string name, append a = append::False) {
        auto dims = std::vector<hsize_t>{m.size()};
        DSET_TEMPLATE_CODE
    }

    template<class T, int RANK, class G>
    h5cpp::h5dset write_tensor(const Eigen::Tensor<T,RANK,Eigen::RowMajor> &m, const G &group, std::string name, append a = append::False) {
        std::vector<hsize_t> dims;
        for (int i = 0; i < RANK; i++)
            dims.push_back(m.dimensions()[i]);
        DSET_TEMPLATE_CODE
    }

    template<class T, int RANK, class G>
    h5cpp::h5dset write_tensor(const Eigen::Tensor<T,RANK,Eigen::ColMajor> &m, const G &group, std::string name, append a = append::False) {
        Eigen::Matrix<int, Eigen::Dynamic,1> reverse;
        reverse.resize(RANK);
        for (int i = 0; i < RANK; i++)
            reverse[i] = RANK - 1 -i;

        // convert the col major into a row major for hdf5
        Eigen::Tensor<T,RANK,Eigen::RowMajor> row_m = m.swap_layout().shuffle(reverse);
        return write_tensor(row_m, group, name, a);
    }

    template<class T, class G>
    h5cpp::h5dset write_scalar(const T &scalar, const G &group, std::string name, append a = append::False) {
        auto ds = bool(a) ? get_eigen_dspace({1}, a) : dspace();
        auto dset = group.create_dataset(name, type_map[typeid(T)], ds);
        dset.write(&scalar);
        return dset;
    }

    // append functions (append_vector, append_matrix, etc.) (create or open)
    // also, pass in group = root file (should inherit from common)
}

#endif
