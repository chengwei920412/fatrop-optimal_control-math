#ifndef CASADICODEGENINCLUDED
#define CASADICODEGENINCLUDED
#include <vector>
#include <string>
#include <memory>
#include "FunctionEvaluation.hpp"
#include "aux/DynamicLib.hpp"

#ifdef ENABLE_MULTITHREADING
#include <omp.h>
#endif

/* Typedefs */
typedef long long int casadi_int;
typedef void (*signal_t)(void);
typedef casadi_int (*getint_t)(void);
typedef int (*work_t)(casadi_int *sz_arg, casadi_int *sz_res, casadi_int *sz_iw, casadi_int *sz_w);
typedef const casadi_int *(*sparsity_t)(casadi_int ind);
typedef int (*eval_t)(const double **arg, double **res, casadi_int *iw, double *w, int mem);
typedef int (*casadi_checkout_t)(void);
typedef void (*casadi_release_t)(int);
namespace fatrop
{
    class EvalCasGen : public EvalBase
    {
    public:
        EvalCasGen();
        /// constructor from file
        EvalCasGen(const std::shared_ptr<DLHandler> &handle, const std::string &function_name);
        /// pointer to result_buffer
        #ifndef ENABLE_MULTITHREADING
            double *output_buffer_p;
        #else  
            std::vector<double*> output_buffer_p = std::vector<double*>(omp_get_max_threads());
        #endif
        /// pointer to casadi codegen evalutation function
        eval_t eval; // !! multhithreading of this function not yet supported
        /// casadi int work vector
        casadi_int *iw;
        /// casadi double work vector
        double *w;
        /// increase reference counter
        signal_t incref;
        /// decrease reference counter
        signal_t decref;
        /// input size
        int *input_size;
        /// release casadi memory
        casadi_release_t release;
        /// thread local mem id
        int mem;
        /// double work vector
        std::vector<double> work_vector_d;
        /// int work vector
        std::vector<casadi_int> work_vector_i;
        /// evaluate function and save res in "ccs format with lda==out_m"
        int eval_buffer(const double **arg);
        /// for reference counting of handle pointer
        std::shared_ptr<DLHandler> handle;
        ~EvalCasGen();
    };
} // fatrop

#endif // CASADICODEGENINCLUDED