#include <iostream>
#include "hdf5.h"
#include <memory>
#include "h5cpp.h"
#include <vector>
#include <eigen3/Eigen/Core>
#include "boost/multi_array.hpp"
#include <string>

using namespace std;
using namespace h5cpp;

int main() {
    //write data
    {
        //create a 3x3 matrix of ints
        vector<hsize_t> dims = {3,3};
        Eigen::Matrix<int,3,3,Eigen::RowMajor> A;
        A << 1,2,3,
             4,5,6,
             7,8,9;

        dspace ds(dims);

        h5file f("test.h5", io::w);

        // write matrix
        auto d1 = f.create_dataset("data", dtype::Int, ds);
        d1.write(A.data());

        // string message
        string message("hello there, how are you?");
        dspace ds_a(vector<hsize_t>{1});

        // write message
        auto a1 = f.create_attribute("message", dtype::String,  ds_a);
        a1.write(&message);

        // write seres of messages
        const int num = 4;
        const char* series[num] = {"hi","bye","coffee","tea"};
        dspace ds_a2(vector<hsize_t>{num});
        auto a2 = f.create_attribute("series", dtype::String,  ds_a2);
        a2.write(series);
    }


    //read data
    {
        h5file f("test.h5", io::r);
        auto d1 = f.open_dataset("data");
        auto dims = d1.get_dspace().dims;
        const int nx = dims[0];
        const int ny = dims[1];
        auto a1 = f.open_attribute("message");
        auto a2 = f.open_attribute("series");

        // read matrix into Eigen matrix
        Eigen::Matrix<int,3,3,Eigen::RowMajor> A;
        d1.read(A.data());
        cout << "Eigen matrix" << endl;
        cout << A << endl;


        // read matrix into Boost multi_array
        const int drank = 2;
        using array_type = boost::multi_array<int, drank>;
        array_type B(boost::extents[nx][ny]);
        d1.read(B.data());

        cout << endl;
        cout << "Boost matrix" << endl;
        for (int i = 0; i != nx; i++) {
            for (int j = 0; j != ny; j++) {
                cout << B[i][j] << " ";
            }
            cout << endl;
        }

        //read string message
        char* c_message[1];
        a1.read(c_message);
        string message(*c_message);
        cout << endl;
        cout << message << endl;

        //read string array
        dims = a2.get_dspace().dims;
        char** c_message_2 = new char*[dims[0]];
        a2.read(c_message_2);
        cout << endl;
        for (int i = 0; i != dims[0]; i++) {
            cout << c_message_2[i] << endl;
        }

        //alternatively, in a boost array
        boost::multi_array<char*,1> str_array(boost::extents[dims[0]]);
        a2.read(str_array.data()); 
        cout << endl;
        for (int i = 0; i != dims[0]; i++) {
            cout << string(str_array[i]) << endl;
        }

    }

}

