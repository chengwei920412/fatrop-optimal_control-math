/*
 * Fatrop - A fast trajectory optimization solver
 * Copyright (C) 2022, 2023 Lander Vanroye <lander.vanroye@kuleuven.be>
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
#ifndef LINESEARCHINCLUDED
#define LINESEARCHINCLUDED
#include "AlgStrategy.hpp"
#include "IterationData.hpp"
#include "templates/NLPAlg.hpp"
#include "solver/FatropData.hpp"
#include "solver/Filter.hpp"
#include "solver/FatropPrinter.hpp"
#include <cmath>
#include <memory>
namespace fatrop
{
    struct LineSearchInfo
    {
        int ls = 0;
        bool first_rejected_by_filter = false;
        bool last_rejected_by_filter = false;
    };
    class LineSearch : public AlgStrategy
    {
    public:
        LineSearch(
            const std::shared_ptr<FatropOptions> &fatropparams,
            const std::shared_ptr<FatropNLP> &nlp,
            const std::shared_ptr<FatropData> &fatropdata, const std::shared_ptr<FatropPrinter> &printer);
        virtual LineSearchInfo find_acceptable_trial_point(double mu, bool small_sd, bool from_backup) = 0;
        inline int eval_constr_viol_trial();
        double eval_obj_trial();
        void reset();
        virtual int update_trial_step(double alpha_pr, double alpha_du) const;
        virtual int initialize_second_order_correction() const;
        virtual int exit_second_order_correction() const;
        virtual int compute_second_order_correction(double alpha) const;
        std::shared_ptr<FatropNLP> fatropnlp_;
        std::shared_ptr<FatropData> fatropdata_;
        std::shared_ptr<FatropPrinter> printer_;
        int eval_cv_count;
        int eval_obj_count;
        int eval_cv_time;
        int eval_obj_time;
    };

    class BackTrackingLineSearch : public LineSearch
    {
    public:
        BackTrackingLineSearch(
            const std::shared_ptr<FatropOptions> &fatropparams,
            const std::shared_ptr<FatropNLP> &nlp,
            const std::shared_ptr<FatropData> &fatropdata,
            const std::shared_ptr<Filter> &filter,
            const std::shared_ptr<Journaller> &journaller, const std::shared_ptr<FatropPrinter> &printer);
        void initialize();
        LineSearchInfo find_acceptable_trial_point(double mu, bool small_sd, bool from_backup);
        std::shared_ptr<Filter> filter_;
        std::shared_ptr<Journaller> journaller_;
        double s_phi;
        double delta;
        double s_theta;
        double gamma_theta;
        double gamma_phi;
        double eta_phi;
        double gamma_alpha;
        bool accept_every_trial_step = false;
        int max_soc;
    };
} // namespace fatrop
#endif // LINESEARCHINCLUDED