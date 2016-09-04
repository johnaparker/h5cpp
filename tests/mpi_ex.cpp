#include <mpi.h>
#include <stdio.h>
#include "h5cpp.h"
#include <vector>

using namespace h5cpp;
using namespace std;

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    {

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d"
           " out of %d processors\n",
           processor_name, world_rank, world_size);
    
    h5file f("new.h5", io::w, true);
    dspace ds(vector<hsize_t>{10,10});
    auto dset = f.create_dataset("data", dtype::Double, ds);
    auto dset2 = f.create_dataset("data2", dtype::Double, ds);

    double* data = new double[50];
    for (int i = 0; i != 50; i++)
        data[i] = i + 50*world_rank;

    if (world_rank == 0) {
        dset.select_write(data, vector<hsize_t>{5,0}, vector<hsize_t>{5,10});
    }
    if (world_rank == 1) {
        dset.select_write(data,vector<hsize_t>{0,0}, vector<hsize_t>{5,10});
    }
    delete[] data;

    // Finalize the MPI environment.
    }
    MPI_Finalize();
    //write an array, where p1 has the first half and p2 has the second half
    //write close in h5file, set to -1
}
