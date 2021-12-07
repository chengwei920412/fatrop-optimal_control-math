#include "Fatrop.hpp"
#include "FatropDebugTools.hpp"
#include "FatropSparse.hpp"
#include "vector"
#include "string"
#include <iostream>
#include "FatropVector.hpp"
using namespace std;
using namespace fatrop;

int main()
{
    OCP_dims dims;
    dims.K = 10;
    int nu = 5;
    int nx = 9;
    int ng = 1;
    dims.nx = vector<int>(dims.K, nx);
    dims.nu = vector<int>(dims.K, nu);
    dims.ng = vector<int>(dims.K, ng);
    // memory allocation
    fatrop_memory_allocator fma;
    OCP_KKT KKT(dims, fma);
    fma.allocate();
    random_OCP(KKT, dims, 0);
    // Sparse_OCP(dims, KKT).print("matrix");
    blasfeo_dmat *RSQrq = (blasfeo_dmat *)KKT.RSQrqt[0];
    blasfeo_dmat *BAbt = (blasfeo_dmat *)KKT.BAbt[0];
    blasfeo_timer timer;
    blasfeo_tic(&timer);
    for (unsigned long int iter = 0; iter < 1e0; ++iter)
    {
        for (int i = 0; i < dims.K; ++i)
        {
            // blasfeo_dgese(nu + nx + 1, nu + nx, (double) i, RSQrq + i, 0, 0);
            // blasfeo_dgese(nu + nx + 1, nx, (double) i, BAbt +i , 0, 0);
            blasfeo_dgese(nu + nx + 1, nu + nx, (double)i, RSQrq, 0, 0);
            blasfeo_dgese(nu + nx + 1, nx, (double)i, BAbt, 0, 0);
        }
    }
    double el = blasfeo_toc(&timer);
    cout << "el time " << el << " seconds \n";
    return 0;
}