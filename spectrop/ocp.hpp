#pragma once
#include <casadi/casadi.hpp>
#include <string>
#include "casadi_utilities.hpp"
#include "stage.hpp"
#include "solver.hpp"

namespace fatrop
{
    namespace spectrop
    {
        namespace cs = casadi;
        class Ocp;
        // class Stage;
        class OcpInternal
        {
        public:
            const uo_set_mx &get_states();
            const uo_set_mx &get_controls();
            const uo_set_mx &get_global_parameters();
            const uo_set_mx &get_control_parameters();
            const std::vector<cs::MX> &get_global_parameter_syms()
            {
                return global_parammeter_syms_;
            }

        protected:
            friend class Ocp;
            friend class StageInternal;
            uo_set_mx states_;
            uo_set_mx controls_;
            uo_set_mx automatics_;
            uo_set_mx global_parameters_;
            uo_set_mx control_parameters_;
            std::vector<cs::MX> global_parammeter_syms_;
            bool is_state(const cs::MX &var);
            bool is_control(const cs::MX &var);
            bool is_automatic(const cs::MX &var);
            bool is_global_parameter(const cs::MX &var);
            bool is_control_parameter(const cs::MX &var);
            std::vector<std::pair<cs::MX, cs::MX>> initial_values;
            std::vector<std::pair<cs::MX, cs::MX>> parameter_values;
            std::string solver_name = "fatrop";
        };
        class Ocp : private std::shared_ptr<OcpInternal>
        {
        public:
            Ocp() : std::shared_ptr<OcpInternal>(new OcpInternal())
            {
            }
            cs::MX state(const int m = 1, const int n = 1);
            cs::MX control(const int m = 1, const int n = 1);
            cs::MX automatic(const int m = 1, const int n = 1);
            cs::MX parameter(const int m = 1, const int n = 1, const std::string &grid = "global");
            cs::MX sample(const cs::MX &expr) const;
            Stage new_stage(const int K = 1);
            const std::vector<Stage> &get_stages() const;
            const std::vector<cs::MX> &get_global_parameters() const
            {
                return get()->global_parammeter_syms_;
            }
            cs::Function to_function(const std::vector<cs::MX> &in, const std::vector<cs::MX> &out);
            cs::MX at_t0(const cs::MX &expr) const {return stages_.front().at_t0(expr);};
            cs::MX at_tf(const cs::MX &expr) const {return stages_.back().at_tf(expr);};
            void set_initial(const cs::MX &var, const cs::MX &value);
            cs::MX eval_at_initial(const cs::MX &expr) const;

        protected:
            std::vector<Stage> stages_;
        };
    } // namespace spectrop
} // namespace fatrop