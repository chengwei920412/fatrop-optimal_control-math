#ifndef FATROP_BLASFEO_INCLUDED
#define FATROP_BLASFEO_INCLUDED

// macros
#define MAT blasfeo_dmat
#define VEC blasfeo_dvec
#define MEMSIZE_MAT blasfeo_memsize_dmat
#define CREATE_MAT blasfeo_create_dmat
#define ROWPE blasfeo_drowpe
#define VECPE blasfeo_dvecpe
#define VECPEI blasfeo_dvecpei
#define ROWPEI blasfeo_drowpei
#define COLPE blasfeo_dcolpe
#define COLPEI blasfeo_dcolpei
#define CREATE_MAT blasfeo_create_dmat
#define MATEL BLASFEO_DMATEL
#define ROWSW blasfeo_drowsw
#define COLSW blasfeo_dcolsw
#define GEAD blasfeo_dgead
#define GEADTR fatrop_dgead_transposed
#define GECP blasfeo_dgecp
#define VECCP blasfeo_dveccp
#define VECCPSC blasfeo_dveccpsc
#define VECCPR fatrop_dveccp_reversed
#define TRSM_RLNN fatrop_dtrsm_rlnn_alt // TODO this is not implemented by blasfeo so we defined our own (naive) implementation
#define VECEL BLASFEO_DVECEL
#define MEMSIZE_VEC blasfeo_memsize_dvec
#define CREATE_VEC blasfeo_create_dvec
#define GEMM_NT blasfeo_dgemm_nt
#define GEAD blasfeo_dgead
#define SYRK_LN_MN blasfeo_dsyrk_ln_mn
#define GETR blasfeo_dgetr
#define TRTR_L blasfeo_dtrtr_l
#define POTRF_L_MN blasfeo_dpotrf_l_mn
#define ROWEX blasfeo_drowex
#define ROWIN blasfeo_drowin
#define ROWAD fatrop_drowad
#define TRSV_LTN blasfeo_dtrsv_ltn
#define TRSV_LNN blasfeo_dtrsv_lnn
#define TRSV_UNU fatrop_dtrsv_unu
#define GEMV_T blasfeo_dgemv_t
#define GEMV_N blasfeo_dgemv_n
#define VECSE blasfeo_dvecse
#define PACKMAT blasfeo_pack_dmat
#define PACKVEC blasfeo_pack_dvec
#define PMAT PermMat
#define AXPY blasfeo_daxpy
#define AXPBY blasfeo_daxpby
#define DOT blasfeo_ddot
#define GESE blasfeo_dgese
#define DIARE blasfeo_ddiare
#define COLSC blasfeo_dcolsc

#define MAX(a, b)                   \
    (                               \
        {                           \
            __typeof__(a) _a = (a); \
            __typeof__(b) _b = (b); \
            _a > _b ? _a : _b;      \
        })
#define MIN(a, b)                   \
    (                               \
        {                           \
            __typeof__(a) _a = (a); \
            __typeof__(b) _b = (b); \
            _a < _b ? _a : _b;      \
        })
#include <iostream>
extern "C"
{
#include <blasfeo.h>
}
#include "aux/LinearAlgebra.hpp"
#include "aux/FatropVector.hpp"
#include "aux/Common.hpp"
#if DEBUG
#include <assert.h>
#endif
using namespace std;
namespace fatrop
{
    void fatrop_dcolsc(int kmax, double alpha, struct blasfeo_dmat *sA, int ai, int aj);
    // copy elements from sx to sy but in reversed order to avoid aliasing issues in recursion
    void fatrop_dveccp_reversed(int m, struct blasfeo_dvec *sx, int xi, struct blasfeo_dvec *sy, int yi);
    // for debugging purposes
    // void fatrop_potrf_l_mn(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj);
    void test();
    /** \brief D <= alpha * B * A^{-1} , with A lower triangular employing explicit inverse of diagonal, fatrop uses its own (naive) implementation since it  not implemented yet in blasfeo */
    void fatrop_dtrsm_rlnn(int m, int n, double alpha, MAT *sA, int offs_ai, int offs_aj, MAT *sB, int offs_bi, int offs_bj, MAT *sD, int offs_di, int offs_dj);
    /** \brief D <= alpha * B * A^{-1} , with A lower triangular employing explicit inverse of diagonal, fatrop uses its own (naive) implementation since it  not implemented yet in blasfeo */
    void fatrop_dtrsm_rlnn_alt(int m, int n, double alpha, MAT *sA, int offs_ai, int offs_aj, MAT *sB, int offs_bi, int offs_bj, MAT *sD, int offs_di, int offs_dj);
    /** \brief B <= B + alpha*A^T (B is mxn) */
    void fatrop_dgead_transposed(int m, int n, double alpha, struct blasfeo_dmat *sA, int offs_ai, int offs_aj, struct blasfeo_dmat *sB, int offs_bi, int offs_bj);
    void fatrop_identity(const int m, MAT *sA, const int ai, const int aj);
    void fatrop_drowad(int kmax, double alpha, struct blasfeo_dvec *sx, int xi, struct blasfeo_dmat *sA, int ai, int aj);

    // // permute the rows of a matrix struct
    // void fatrop_drowpe(int size, int kmax, int *ipiv, struct blasfeo_dmat *sA, const int ai, const int aj)
    // {

    //     // invalidate stored inverse diagonal
    //     sA->use_dA = 0;

    //     int ii;
    //     for (ii = 0; ii < kmax; ii++)
    //     {
    //         if (ipiv[ii] != ii)
    //             blasfeo_drowsw(size, sA, ai + ii, aj, sA, ai + ipiv[ii], aj);
    //     }
    //     return;
    // }

    // // inverse permute the rows of a matrix struct
    // void fatrop_drowpei(const int size, int kmax, int *ipiv, struct blasfeo_dmat *sA, const int ai, const int aj)
    // {

    //     // invalidate stored inverse diagonal
    //     sA->use_dA = 0;

    //     int ii;
    //     for (ii = kmax - 1; ii >= 0; ii--)
    //     {
    //         if (ipiv[ii] != ii)
    //             blasfeo_drowsw(size, sA, ai + ii, aj, sA, ai + ipiv[ii], aj);
    //     }
    //     return;
    // }

    // // permute the cols of a matrix struct
    // void fatrop_dcolpe(const int size, int kmax, int *ipiv, struct blasfeo_dmat *sA, const int ai, const int aj)
    // {

    //     // invalidate stored inverse diagonal
    //     sA->use_dA = 0;

    //     int ii;
    //     for (ii = 0; ii < kmax; ii++)
    //     {
    //         if (ipiv[ii] != ii)
    //             blasfeo_dcolsw(size, sA, ai, aj + ii, sA, ai, aj + ipiv[ii]);
    //     }
    //     return;
    // }

    // // inverse permute the cols of a matrix struct
    // void fatrop_dcolpei(const int size, int kmax, int *ipiv, struct blasfeo_dmat *sA, const int ai, const int aj)
    // {

    //     // invalidate stored inverse diagonal
    //     sA->use_dA = 0;

    //     int ii;
    //     for (ii = kmax - 1; ii >= 0; ii--)
    //     {
    //         if (ipiv[ii] != ii)
    //             blasfeo_dcolsw(size, sA, ai, aj + ii, sA, ai, aj + ipiv[ii]);
    //     }
    //     return;
    // }

    /** \brief this class is used for blasfeo matrices*/
    class FatropMatBF : public FatropMat
    {
    public:
        /** \brief constructor memory still has to be allocated*/
        FatropMatBF(const int nrows, const int ncols, const int row_offset, const int col_offset) : row_offset_(row_offset), col_offset_(col_offset), nrows_(nrows), ncols_(ncols) {}
        /** \brief constructor memory already allocated*/
        FatropMatBF(const int nrows, const int ncols, const int row_offset, const int col_offset, MAT *matbf) : mat_(matbf), row_offset_(row_offset), col_offset_(col_offset), nrows_(nrows), ncols_(ncols) {}
        /** \brief constructor memory already allocated*/
        FatropMatBF(MAT *matbf) : mat_(matbf), row_offset_(0), col_offset_(0), nrows_(matbf->m), ncols_(matbf->n) {}
        /** \brief type conversion to blasfeo matrix pointer*/
        inline explicit operator MAT *() const
        {
            return this->mat_;
        }
        /** \brief acces to element of matrix */
        inline double &at(const int ai, const int aj) const
        {
#if DEBUG
            assert(ai < nrows_);
            assert(aj < ncols_);
#endif
            return MATEL(mat_, ai + row_offset_, aj + col_offset_);
        };
        /** \brief get element of matrix */
        inline double get_el(const int ai, const int aj) const { return this->at(ai, aj); };
        /** \brief get number of rows */
        inline int nrows() const { return nrows_; };
        /** \brief get number of cols */
        inline int ncols() const { return ncols_; };
        /** \brief copies all elements from a given fatrop_matrix to this matrix*/
        void operator=(const FatropMat &fm)
        {
            for (int ai = 0; ai < fm.nrows(); ai++)
            // for (int ai = 0; ai < nrows_; ai++)
            {
                for (int aj = 0; aj < fm.ncols(); aj++)
                // for (int aj = 0; aj < ncols_; aj++)
                {
                    this->at(ai, aj) = fm.get_el(ai, aj);
                }
            }
        }
        /** \brief set data pointer*/
        void set_datap(MAT *matbf)
        {
            mat_ = matbf;
        }
        /** \brief take a block of size (p,q), starting at (i,j)*/
        FatropMatBF block(const int i, const int j, const int p, const int q) const
        {
            return FatropMatBF(p, q, row_offset_ + i, col_offset_ + j, this->mat_);
        }

    private:
        MAT *mat_ = NULL;
        const int row_offset_;
        const int col_offset_;
        const int nrows_;
        const int ncols_;
    };

    /** \brief this class is used for the allocation of a blasfeo matrix, the dimsensions are set from a vector */
    class FatropMemoryMatBF
    {
    public:
        /** \brief constuction for allocation on fatrop_memory_allocator*/
        FatropMemoryMatBF(const FatropVector<int> &nrows, const FatropVector<int> &ncols, int N) : N_(N), nrows_(nrows), ncols_(ncols)
        // TODO: if rvalue-reference is used -> unecessary copy, use move sementics instead.
        {
            set_up();
        }
        FatropMemoryMatBF(const int nrows, const int ncols, int N) : N_(N), nrows_(vector<int>(N, nrows)), ncols_(vector<int>(N, ncols))
        {
            set_up();
        }
        /** \brief calculate memory size*/
        int memory_size() const
        {
            int result = 0;
            // size to store structs
            result += N_ * sizeof(MAT);
            // sufficient space for cache alignment
            result = (result + LEVEL1_DCACHE_LINE_SIZE - 1) / LEVEL1_DCACHE_LINE_SIZE * LEVEL1_DCACHE_LINE_SIZE + LEVEL1_DCACHE_LINE_SIZE;
            // size to store date
            for (int i = 0; i < N_; i++)
            {
                // result += MEMSIZE_MAT(nrows_.at(i), ncols_.at(i));
                result += MEMSIZE_MAT(nrows_.at(i), ncols_.at(i));
            }
            return result;
        };
        /** \brief set up memory element and advance pointer */
        void set_up()
        {
            free(mem);
            mem = malloc(this->memory_size());
            char *data_p = (char *)mem;
            MAT *bf_ptr = (MAT *)data_p;
            this->mat = bf_ptr;
            bf_ptr += N_;
            // align with cache line
            long long l_ptr = (long long)bf_ptr;
            l_ptr = (l_ptr + LEVEL1_DCACHE_LINE_SIZE - 1) / LEVEL1_DCACHE_LINE_SIZE * LEVEL1_DCACHE_LINE_SIZE;
            data_p = (char *)l_ptr;
            double *d_ptr_begin = (double *)data_p;
            for (int i = 0; i < N_; i++)
            {
                CREATE_MAT(nrows_.at(i), ncols_.at(i), mat + i, data_p);
                data_p += MEMSIZE_MAT(nrows_.at(i), ncols_.at(i));
                // data_p += (mat + i)->memsize;
            }
            double *d_ptr_end = (double *)data_p;
            for (double *d_ptr = d_ptr_begin; d_ptr < d_ptr_end; d_ptr++)
            {
                *d_ptr = 0.0;
            }
            // cout << "allocated memory size " << this->memory_size()<< endl;
            // cout << "used memory size " << (unsigned long long) d_ptr_end - (unsigned long long) mem << endl;
            // cout << "difference " << this->memory_size()-((unsigned long long) d_ptr_end - (unsigned long long) mem)<< endl;
        }
        /** \brief get fatrop matrix bf */
        FatropMatBF operator[](const int N) const
        {
#if DEBUG
            assert(N < N_);
#endif
            MAT *resmat = mat + N;
            FatropMatBF res(resmat->m, resmat->n, 0, 0, resmat);
            return res;
        }
        /** \brief get first blasfeo_xmat* struct */
        explicit operator MAT *() const
        {
            return mat;
        }
        FatropMemoryMatBF(const FatropMemoryMatBF &cpy) = delete;
        FatropMemoryMatBF &operator=(const FatropMemoryMatBF &) = delete;
        ~FatropMemoryMatBF()
        {
            free(mem);
        }

    private:
        void *mem = NULL;
        MAT *mat;
        const int N_;
        const FatropVector<int> nrows_;
        const FatropVector<int> ncols_;
    };
    /** this class is used for blasfeo vectors*/
    class FatropVecBF : public FatropVec
    {
    public:
        /** \brief constructor memory still has to be allocated*/
        FatropVecBF(const int nels, const int offset) : offset_(offset), nels_(nels) {}
        /** \brief constructor memory already allocated*/
        FatropVecBF(const int nels, const int offset, VEC *vecbf) : vec_(vecbf), offset_(offset), nels_(nels) {}
        /** \brief type conversion to blasfeo vector pointer*/
        inline explicit operator VEC *() const
        {
            return this->vec_;
        }
        /** \brief access to element of matrix */
        double &at(const int ai) const
        {
#if DEBUG
            assert(ai < nels_);
#endif
            return VECEL(vec_, ai + offset_);
        };
        /** \brief get element of vector */
        double get_el(const int ai) const { return this->at(ai); };
        /** \brief get number of elements */
        int nels() const { return nels_; };
        /** \brief get offset */
        int offset() const { return offset_; };
        /** \brief copies all elements from a given fatrop_vector to this vector*/
        void operator=(const FatropVec &fm)
        {
            for (int ai = 0; ai < nels_; ai++)
            {
                this->at(ai) = fm.get_el(ai);
            }
        }
        void operator=(const vector<double> &fm)
        {
            for (int ai = 0; ai < nels_; ai++)
            {
                this->at(ai) = fm.at(ai);
            }
        }
        /** \brief set data pointer*/
        void set_datap(VEC *vecbf)
        {
            vec_ = vecbf;
        }
        /** \brief take a block of size (p), starting at (i)*/
        FatropVecBF block(const int i, const int p) const
        {
            return FatropVecBF(p, offset_ + i, this->vec_);
        }
        inline void SwapWith(FatropVecBF &vb)
        {
            DBGASSERT(vb.offset_ == offset_);
            DBGASSERT(vb.nels_ == nels_);
            VEC *tmp = vec_;
            vec_ = vb.vec_;
            vb.vec_ = tmp;
        }

    protected:
        VEC *vec_ = NULL;
        const int offset_;
        const int nels_;
    };

    // class CachedQuantity
    // {
    // public:
    //     virtual double eval();
    //     inline double get()
    //     {
    //         return evaluated ? value : SetValue(eval());
    //     }
    // private:
    //     double SetValue(const double value_)
    //     {
    //         value = value;
    //         evaluated = true;
    //         return value;
    //     }
    //     bool evaluated = false;
    //     double value = 0.0;
    // };
    // class L1Quantity: public CachedQuantity
    // {
    //     // double eval() override{
    //     //     return
    //     // }
    // };
    // /** \brief decorator for FatropVecBF that allows computation of certain quantities*/
    // class FatropVecBFCached : public FatropVecBF
    // {
    // public:
    //     FatropVecBFCached(const FatropVecBF &fatropvecbf) : FatropVecBF(fatropvecbf){};

    // private:
    // };

    inline void axpy(const double alpha, const FatropVecBF &va, const FatropVecBF &vb, FatropVecBF &vc)
    {
        DBGASSERT(va.nels() == vb.nels());
        DBGASSERT(va.nels() == vc.nels());
        VEC *va_p = (VEC *)va;
        VEC *vb_p = (VEC *)vb;
        VEC *vc_p = (VEC *)vc;
        AXPY(va.nels(), alpha, va_p, va.offset(), vb_p, vb.offset(), vc_p, vc.offset());
    };
    inline void copy(const FatropVecBF &va, const FatropVecBF &vb)
    {
        DBGASSERT(va.nels() == vb.nels());
        VEC *va_p = (VEC *)va;
        VEC *vb_p = (VEC *)vb;
        VECCP(va.nels(), va_p, va.offset(), vb_p, vb.offset());
    };
    inline void axpby(const double alpha, const FatropVecBF &va, const double beta, const FatropVecBF &vb, FatropVecBF &vc)
    {
        DBGASSERT(va.nels() == vb.nels());
        DBGASSERT(va.nels() == vc.nels());
        VEC *va_p = (VEC *)va;
        VEC *vb_p = (VEC *)vb;
        VEC *vc_p = (VEC *)vc;
        AXPBY(va.nels(), alpha, va_p, va.offset(), beta, vb_p, vb.offset(), vc_p, vc.offset());
    };
    inline double dot(const FatropVecBF &va, FatropVecBF &vb)
    {
        DBGASSERT(va.nels() == vb.nels());
        VEC *va_p = (VEC *)va;
        VEC *vb_p = (VEC *)vb;
        return DOT(va.nels(), va_p, va.offset(), vb_p, vb.offset());
    };
    inline double Linf(const FatropVecBF &va)
    {
        VEC *va_p = (VEC *)va;
        int nels = va.nels();
        int offset = va.offset();
        double res = 0.0;
        for (int i = offset; i < nels + offset; i++)
        {
            res = MAX(res, abs(VECEL(va_p, i)));
        }
        return res;
    };
    inline double L1(const FatropVecBF &va)
    {
        VEC *va_p = (VEC *)va;
        int nels = va.nels();
        int offset = va.offset();
        double res = 0.0;
        for (int i = offset; i < nels + offset; i++)
        {
            res += abs(VECEL(va_p, i));
        }
        return res;
    };

    /** \brief this class is used for the allocation of a blasfeo vector, the dimsensions are set from a vector */
    class FatropMemoryVecBF
    {
    public:
        /** \brief constuction for allocation on MemoryAllocator*/
        FatropMemoryVecBF(const FatropVector<int> &nels, int N) : N_(N), nels_(nels)
        // TODO: if rvalue-reference is used -> unecessary copy, use move sementics instead.
        {
            set_up();
        }
        FatropMemoryVecBF(const int nels, int N) : N_(N), nels_(vector<int>(N, nels))
        {
            set_up();
        }
        /** \brief calculate memory size*/
        int memory_size() const
        {
            int result = 0;
            // size to store structs
            result += N_ * sizeof(VEC);
            // sufficient space for cache alignment
            result = (result + LEVEL1_DCACHE_LINE_SIZE - 1) / LEVEL1_DCACHE_LINE_SIZE * LEVEL1_DCACHE_LINE_SIZE + LEVEL1_DCACHE_LINE_SIZE;
            // size to store date
            for (int i = 0; i < N_; i++)
            {
                result += MEMSIZE_VEC(nels_.at(i));
            }
            return result;
        };
        /** \brief set up memory element and advance pointer */
        void set_up()
        {
            free(mem);
            mem = malloc(this->memory_size());
            char *data_p = (char *)mem;
            VEC *bf_ptr = (VEC *)data_p;
            this->vec = bf_ptr;
            bf_ptr += N_;
            // align with cache line
            long long l_ptr = (long long)bf_ptr;
            l_ptr = (l_ptr + LEVEL1_DCACHE_LINE_SIZE - 1) / LEVEL1_DCACHE_LINE_SIZE * LEVEL1_DCACHE_LINE_SIZE;
            data_p = (char *)l_ptr;
            double *d_ptr_begin = (double *)data_p;
            for (int i = 0; i < N_; i++)
            {
                CREATE_VEC(nels_.at(i), vec + i, data_p);
                data_p += MEMSIZE_VEC(nels_.at(i));
            }
            double *d_ptr_end = (double *)data_p;
            for (double *d_ptr = d_ptr_begin; d_ptr < d_ptr_end; d_ptr++)
            {
                *d_ptr = 0.0;
            }
        }
        /** \brief get fatrop matrix bf */
        FatropVecBF operator[](const int N) const
        {
#if DEBUG
            assert(N < N_);
#endif
            VEC *resvec = vec + N;
            FatropVecBF res(resvec->m, 0, resvec);
            return res;
        }
        /** \brief get first blasfeo_xmat* struct */
        explicit operator VEC *() const
        {
            return vec;
        }
        FatropMemoryVecBF(const FatropMemoryVecBF &cpy) = delete;
        FatropMemoryVecBF &operator=(const FatropMemoryVecBF &) = delete;
        ~FatropMemoryVecBF()
        {
            free(mem);
        }

    private:
        void *mem = NULL;
        VEC *vec;
        const int N_;
        const FatropVector<int> nels_;
    };

    /** \brief this class represents a permutation matrix */
    class PermMat : public FatropMat
    {
    public:
        /** \brief constructor memory still has to be allocated */
        PermMat(const int dim) : dim_(dim){};
        /** \brief constructor memory already allocated */
        PermMat(const int dim, int *data) : dim_(dim), data_(data){};
        /** \brief get number of rows */
        int nrows() const { return dim_; };
        /** \brief get number of columns */
        int ncols() const { return dim_; };
        /** \brief get element of matrix represented by this permutation matrix - only used for debugging and testing purposes */
        double get_el(const int ai, const int aj) const
        {
#if DEBUG
            assert(data_ != NULL);
#endif
            int aj_one = data_[ai];
            int row_curr = ai - 1;
            while (row_curr >= 0)
            {
                if (aj_one == data_[row_curr])
                {
                    aj_one = row_curr;
                }
                row_curr--;
            }
            if (aj == aj_one)
            {
                return 1.0;
            }
            else
            {
                return 0.0;
            }
        };
        void print(const int kmax) const
        {
            for (int k = 0; k < kmax; k++)
            {
                cout << k << " <-> " << data_[k] << endl;
            }
        }
        /** \brief set data pointer*/
        void set_datap(int *data)
        {
            data_ = data;
        }
        /** \brief set data point*/
        void set_datap(const int i, const int val)
        {
#if DEBUG
            assert(data_ != NULL);
            assert(i < dim_);
#endif
            data_[i] = val;
        }
        /** \brief apply row permutation*/
        void PM(const int kmax, MAT *M) const
        {
#if DEBUG
            assert(data_ != NULL);
#endif
            ROWPE(kmax, data_, M);
        }
        /** \brief apply vec permutation*/
        void PV(const int kmax, VEC *V, const int offs) const
        {
#if DEBUG
            assert(data_ != NULL);
#endif
            VECPE(kmax, data_, V, offs);
        }
        /** \brief apply vec permutation*/
        void PtV(const int kmax, VEC *V, const int offs) const
        {
#if DEBUG
            assert(data_ != NULL);
#endif
            VECPEI(kmax, data_, V, offs);
        }
        /** \brief apply row permutation on partial matrix*/
        void PM(const int kmax, const int n, MAT *M, const int ai, const int aj) const
        {
#if DEBUG
            assert(data_ != NULL);
#endif
            // invalidate stored inverse diagonal
            M->use_dA = 0;

            int ii;
            for (ii = 0; ii < kmax; ii++)
            {
                if (data_[ii] != ii)
                    ROWSW(n, M, ai + ii, aj, M, ai + data_[ii], aj);
            }
            return;
        }
        /** \brief apply inverse row permutation*/
        void
        PtM(const int kmax, MAT *M) const
        {
#if DEBUG
            assert(data_ != NULL);
#endif
            ROWPEI(kmax, data_, M);
        }
        /** \brief apply inverse col permutation*/
        void MP(const int kmax, MAT *M) const
        {
#if DEBUG
            assert(data_ != NULL);
#endif
            COLPEI(kmax, data_, M);
        }
        /** \brief apply col permutation*/
        void MPt(const int kmax, MAT *M) const
        {
#if DEBUG
            assert(data_ != NULL);
#endif
            COLPE(kmax, data_, M);
        }
        /** int pointer of permutation vector */
        explicit operator int *() { return data_; };

        // private:
        const int dim_;
        int *data_ = NULL;
    };

    /** \brief this class is used for the allocation of a permutation matrix */
    class MemoryPermMat : public PermMat
    {
    public:
        /** \brief constructor */
        MemoryPermMat(const int dim, const int N) : PermMat(dim), dim_(dim), N_(N)
        {
            set_up();
        };
        /** \brief calculate needed memory size*/
        int memory_size() const
        {
            int size = 0;
            size += N_ * sizeof(PermMat) + N_ * dim_ * sizeof(int);
            return size;
        }
        /** \brief set up memory*/
        void set_up()
        {
            free(mem);
            mem = malloc(this->memory_size());
            char *char_p = (char *)mem;
            perm_p = (PermMat *)char_p;
            PermMat *perm_pp = perm_p;
            for (int i = 0; i < N_; i++)
            {
                new (perm_pp) PermMat(dim_);
                perm_pp++;
            }
            int *data_p = (int *)perm_pp;
            this->set_datap(data_p);
            for (int i = 0; i < N_; i++)
            {
                perm_p[i].set_datap(data_p);
                data_p += dim_;
            }
            char_p = (char *)data_p;
        }
        explicit operator PermMat *() { return perm_p; };
        MemoryPermMat(const MemoryPermMat &cpy) = delete;
        MemoryPermMat &operator=(const MemoryPermMat &) = delete;
        ~MemoryPermMat()
        {
            free(mem);
        }

    private:
        void *mem = NULL;
        const int dim_;
        const int N_;
        PermMat *perm_p;
    };
    MatrixInd max_el(int m, int n, MAT *matr, int ai, int aj);

    /** \brief Function to calculate LU factorization result is saved in A, L is lower unitriangular */
    void LU_FACT(const int m, const int n, const int n_max, int &rank, MAT *A, PMAT *Pl_p, PMAT *Pr_p, double tol = 1e-8);
    /** \brief Function to calculate LU factorization but A, and result (L and U) are transposed, all indices refer to the dimensions of the original A matrix (and not the transposed one) */
    void LU_FACT_transposed(const int m, const int n, const int n_max, int &rank, MAT *At, PMAT *Pl_p, PMAT *Pr_p, double tol = 1e-8);
    void fatrop_dtrsv_unu(const int m, const int n, blasfeo_dmat *sA, const int ai, const int aj, blasfeo_dvec *sx, const int xi, blasfeo_dvec *sz, const int zi);

} // namespace fatrop
#endif // FATROP_BLASFEO_INCLUDED