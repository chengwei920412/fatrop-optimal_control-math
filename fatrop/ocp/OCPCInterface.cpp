/*
 * Fatrop - A fast trajectory optimization solver
 * Copyright (C) 2022, 2023 Lander Vanroye, KU Leuven. All rights reserved.
 *
 * This file is part of Fatrop.
 *
 * Fatrop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fatrop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Fatrop.  If not, see <http://www.gnu.org/licenses/>. */

#include "fatrop/ocp/OCP.hpp"
#include "fatrop/ocp/OCPAbstract.hpp"
#include "fatrop/ocp/FatropOCP.hpp"
#include "fatrop/ocp/StageOCPApplication.hpp"

namespace fatrop
{

    class OcpSolverDriver;

    struct FatropOcpCSolver
    {
        OcpSolverDriver* driver;
    };

    #define FATROP_OCP_SOLVER_IMPLEMENTATION

    #include "OCPCInterface.h"

    class FatropOcpCDimsMapping : public OCPAbstract
    {
        public:
            FatropOcpCInterface* ocp;
            FatropOcpCDims s;
            FatropOcpCDimsMapping(FatropOcpCInterface* ocp) : ocp(ocp)
            {
            }

        fatrop_int get_nx(fatrop_int k) const override
        {
            return ocp->get_nx(k, ocp->user_data);
        }
        fatrop_int get_nu(fatrop_int k) const override
        {
            return ocp->get_nu(k, ocp->user_data);
        }
        fatrop_int get_ng(fatrop_int k) const override
        {
            return ocp->get_ng(k, ocp->user_data);
        }
        fatrop_int get_ng_ineq(fatrop_int k) const override
        {
            return ocp->get_ng_ineq(k, ocp->user_data);
        }
        fatrop_int get_n_stage_params(fatrop_int k) const override
        {
            if (!ocp->get_n_stage_params) return 0;
            return ocp->get_n_stage_params(k, ocp->user_data);
        }
        fatrop_int get_n_global_params() const override
        {
            if (!ocp->get_n_global_params) return 0;
            return ocp->get_n_global_params(ocp->user_data);
        }

        fatrop_int get_default_stage_params(double *stage_params, fatrop_int k) const override
        {
            if (!ocp->get_default_stage_params) return 0;
            return ocp->get_default_stage_params(stage_params, k, ocp->user_data);
        }
        fatrop_int get_default_global_params(double *global_params) const override
        {
            if (!ocp->get_default_global_params) return 0;
            return ocp->get_default_global_params(global_params, ocp->user_data);
        }
        fatrop_int get_horizon_length() const override
        {
            return ocp->get_horizon_length(ocp->user_data);
        }
        fatrop_int eval_BAbt(
            const double *states_kp1,
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            MAT *res,
            const fatrop_int k) override
            {
            if (!ocp->eval_BAbt) return 0;
            return ocp->eval_BAbt(states_kp1, inputs_k, states_k, stage_params_k, global_params, res, k, ocp->user_data);
        }
        fatrop_int eval_RSQrqt(
            const double *objective_scale,
            const double *inputs_k,
            const double *states_k,
            const double *lam_dyn_k,
            const double *lam_eq_k,
            const double *lam_eq_ineq_k,
            const double *stage_params_k,
            const double *global_params,
            MAT *res,
            const fatrop_int k) override
            {
            if (!ocp->eval_RSQrqt) return 0;
            return ocp->eval_RSQrqt(objective_scale, inputs_k, states_k, lam_dyn_k, lam_eq_k, lam_eq_ineq_k, stage_params_k, global_params, res, k, ocp->user_data);
        }
        fatrop_int eval_Ggt(
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            MAT *res,
            const fatrop_int k) override
            {
            if (!ocp->eval_Ggt) return 0;
            return ocp->eval_Ggt(inputs_k, states_k, stage_params_k, global_params, res, k, ocp->user_data);
        }
        fatrop_int eval_Ggt_ineq(
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            MAT *res,
            const fatrop_int k) override
            {
            if (!ocp->eval_Ggt_ineq) return 0;
            return ocp->eval_Ggt_ineq(inputs_k, states_k, stage_params_k, global_params, res, k, ocp->user_data);
        }
        fatrop_int eval_b(
            const double *states_kp1,
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) override
            {
            if (!ocp->eval_b) return 0;
            return ocp->eval_b(states_kp1, inputs_k, states_k, stage_params_k, global_params, res, k, ocp->user_data);
        }
        fatrop_int eval_g(
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) override
            {
            if (!ocp->eval_g) return 0;
            return ocp->eval_g(inputs_k, states_k, stage_params_k, global_params, res, k, ocp->user_data);
        }
        fatrop_int eval_gineq(
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) override
            {
            if (!ocp->eval_gineq) return 0;
            return ocp->eval_gineq(inputs_k, states_k, stage_params_k, global_params, res, k, ocp->user_data);
        }
        fatrop_int eval_rq(
            const double *objective_scale,
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) override
            {
            if (!ocp->eval_rq) return 0;
            return ocp->eval_rq(objective_scale, inputs_k, states_k, stage_params_k, global_params, res, k, ocp->user_data);
        }
        fatrop_int eval_L(
            const double *objective_scale,
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) override
            {
            if (!ocp->eval_L) return 0;
            return ocp->eval_L(objective_scale, inputs_k, states_k, stage_params_k, global_params, res, k, ocp->user_data);
        }
        fatrop_int get_bounds(double *lower, double *upper, const fatrop_int k) const override
        {
            return ocp->get_bounds(lower, upper, k, ocp->user_data);
        }
        fatrop_int get_initial_xk(double *xk, const fatrop_int k) const override
        {
            return ocp->get_initial_xk(xk, k, ocp->user_data);
        }
        fatrop_int get_initial_uk(double *uk, const fatrop_int k) const override
        {
            return ocp->get_initial_uk(uk, k, ocp->user_data);
        }
        fatrop_int full_eval_lag_hess(double objective_scale, const double* primal_data, const double* lam_data, const double* stageparams_p, const double* globalparams_p,
            MAT * RSQrqt_p) override
        {
            if (!ocp->full_eval_lag_hess) return 0;
            return ocp->full_eval_lag_hess(objective_scale,
                primal_data,
                lam_data,
                stageparams_p,
                globalparams_p,
                RSQrqt_p,
                &s, ocp->user_data);
        }

        fatrop_int full_eval_constr_jac(const double* primal_data, const double* stageparams_p, const double* globalparams_p,
            MAT* BAbt_p, MAT* Ggt_p, MAT* Ggt_ineq_p) override
        {
            if (!ocp->full_eval_constr_jac) return 0;
            return ocp->full_eval_constr_jac(
                primal_data,
                stageparams_p,
                globalparams_p,
                BAbt_p, Ggt_p, Ggt_ineq_p,
                &s, ocp->user_data);
        }
        fatrop_int full_eval_contr_viol(const double* primal_data, const double* stageparams_p, const double* globalparams_p,
            double* cv_p) override
        {
            if (!ocp->full_eval_contr_viol) return 0;
            return ocp->full_eval_contr_viol(
                primal_data,
                stageparams_p,
                globalparams_p,
                cv_p,
                &s, ocp->user_data);
        }
        fatrop_int full_eval_obj_grad(double objective_scale, const double* primal_data, const double* stageparams_p, const double* globalparams_p,
            double* grad_p) override
        {
            if (!ocp->full_eval_obj_grad) return 0;
            return ocp->full_eval_obj_grad(
                objective_scale,
                primal_data,
                stageparams_p,
                globalparams_p,
                grad_p,
                &s, ocp->user_data);
        }
        fatrop_int full_eval_obj(double objective_scale, const double* primal_data, const double* stageparams_p, const double* globalparams_p,
            double* res) override
        {
            if (!ocp->full_eval_obj) return 0;
            return ocp->full_eval_obj(
                objective_scale,
                primal_data,
                stageparams_p,
                globalparams_p,
                res,
                &s, ocp->user_data);
        }
    };

    class OcpSolverDriver
    {
    public:
        OcpSolverDriver(FatropOcpCInterface* ocp_interface) :
                m(std::make_shared<FatropOcpCDimsMapping>(ocp_interface)),
                app(m)
        {
            app.build();
            const OCPKKTMemory& kktmem = std::dynamic_pointer_cast<FatropOCP>(app.nlp_)->ocpkktmemory_;
            m->s.nx = kktmem.nx.data();
            m->s.nu = kktmem.nu.data();
            m->s.ng = kktmem.ng.data();
            m->s.ng_ineq = kktmem.ng_ineq.data();
            m->s.K = kktmem.K;
            m->s.ux_offs = kktmem.aux.ux_offs.data();
            m->s.g_offs = kktmem.aux.g_offs.data();
            m->s.dyn_offs = kktmem.aux.dyn_offs.data();
            m->s.dyn_eq_offs = kktmem.aux.dyn_eq_offs.data();
            m->s.g_ineq_offs = kktmem.aux.g_ineq_offs.data();
            m->s.max_nu = kktmem.aux.max_nu;
            m->s.max_nx = kktmem.aux.max_nx;
            m->s.max_ng = kktmem.aux.max_ng;
            m->s.max_ngineq = kktmem.aux.max_ngineq;
            m->s.n_ineqs = kktmem.aux.n_ineqs;
        }
        fatrop_int solve()
        {
            return app.optimize();
        }
        std::shared_ptr<FatropOcpCDimsMapping> m;
        FatropOcpCStats stats;
        OCPAbstractApplication app;
    };

    FatropOcpCSolver* fatrop_ocp_c_create(FatropOcpCInterface* ocp_interface)
    {
        FatropOcpCSolver* ret = new FatropOcpCSolver();
        ret->driver = new OcpSolverDriver(ocp_interface);
        return ret;
    }

    int fatrop_ocp_c_set_option_double(FatropOcpCSolver* s, const char* name, double val)
    {
        try
        {
            s->driver->app.set_option(name, val);
        } catch (std::exception& e) {
            return 1;
        }
        return 0;
    }

    int fatrop_ocp_c_set_option_bool(FatropOcpCSolver* s, const char* name, int val)
    {
        try
        {
            s->driver->app.set_option(name, bool(val));
        } catch (std::exception& e) {
            return 1;
        }
        return 0;
    }

    int fatrop_ocp_c_set_option_int(FatropOcpCSolver* s, const char* name, int val)
    {
        try
        {
            s->driver->app.set_option(name, int(val));
        } catch (std::exception& e) {
            return 1;
        }
        return 0;
    }

    int fatrop_ocp_c_set_option_string(FatropOcpCSolver* s, const char* name, const char* val)
    {
        try
        {
            s->driver->app.set_option(name, std::string(val));
        } catch (std::exception& e) {
            return 1;
        }
        return 0;
    }

    const blasfeo_dvec* fatrop_ocp_c_get_primal(FatropOcpCSolver* s)
    {
        return static_cast<const blasfeo_dvec*>(s->driver->app.last_solution_primal());
    }

    const blasfeo_dvec* fatrop_ocp_c_get_dual(FatropOcpCSolver* s)
    {
        return static_cast<const blasfeo_dvec*>(s->driver->app.last_solution_dual());
    }

    int fatrop_ocp_c_solve(FatropOcpCSolver* s)
    {
        s->driver->solve();
        return 0;
    }

    void fatrop_ocp_c_destroy(FatropOcpCSolver* s)
    {
        delete s->driver;
        delete s;
    }


    int fatrop_ocp_c_option_type(FatropOcpCSolver* s, const char* name) {
        const FatropOptions& options = s->driver->app.get_options();
        if (options.numeric_options.find(name)!=options.numeric_options.end()) return 0;
        if (options.integer_options.find(name)!=options.integer_options.end()) return 1;
        if (options.boolean_options.find(name)!=options.boolean_options.end()) return 2;
        if (options.string_options.find(name)!=options.string_options.end()) return 3;

        return -1;
    }

    const struct FatropOcpCDims* fatrop_ocp_c_get_dims(struct FatropOcpCSolver* s) {
        return &s->driver->m->s;
    }

    const FatropOcpCStats* fatrop_ocp_c_get_stats(struct FatropOcpCSolver* s) {
        FatropStats stats_solver = s->driver->app.get_stats();
        FatropOcpCStats* stats = &s->driver->stats;

        stats->compute_sd_time = stats_solver.compute_sd_time;
        stats->duinf_time = stats_solver.duinf_time;
        stats->eval_hess_time = stats_solver.eval_hess_time;
        stats->eval_jac_time = stats_solver.eval_jac_time;
        stats->eval_cv_time = stats_solver.eval_cv_time;
        stats->eval_grad_time = stats_solver.eval_grad_time;
        stats->eval_obj_time = stats_solver.eval_obj_time;
        stats->initialization_time = stats_solver.initialization_time;
        stats->time_total = stats_solver.time_total;
        stats->eval_hess_count = stats_solver.eval_hess_count;
        stats->eval_jac_count = stats_solver.eval_jac_count;
        stats->eval_cv_count = stats_solver.eval_cv_count;
        stats->eval_grad_count = stats_solver.eval_grad_count;
        stats->eval_obj_count = stats_solver.eval_obj_count;
        stats->iterations_count = stats_solver.iterations_count;
        stats->return_flag = stats_solver.return_flag;

        return &s->driver->stats;
    }

} // namespace fatrop