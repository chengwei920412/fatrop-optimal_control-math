/**
 * @file FatropOCP.hpp
 * @author your name (you@domain.com)
 * @brief this file contains necessary the OCP specific code 
 * @version 0.1
 * @date 2021-11-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef FATROP_OCPDIMS_INCLUDED
#define FATROP_OCPDIMS_INCLUDED
#include "../AUX/FatropMemory.hpp"
#include "../AUX/FatropVector.hpp"
#include <vector>
using namespace std;
namespace fatrop
{
    /** \brief  this class contains the problem dimensions of a standard ocp*/
    struct OCPDims
    {
    public:
        /// horizon length
        int K;
        /// input vector size
        FatropVector<int> nu;
        /// state vector size
        FatropVector<int> nx;
        // number of stagewise equality constraints
        FatropVector<int> ng;
    };
} // namespace fatrop
#endif //FATROP_OCPDIMS_INCLUDED