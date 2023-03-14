#include "ocp/BasicOCPApplication.hpp"
using namespace fatrop;
NLPApplication::NLPApplication() : fatropparams_(make_shared<FatropOptions>()), journaller_(make_shared<Journaller>(fatropparams_->maxiter + 1)){};

void NLPApplication::Build(const shared_ptr<FatropNLP> &nlp)
{
    // keep nlp around for getting nlpdims
    nlp_ = nlp;
    AlgBuilder algbuilder;
    algbuilder.BuildFatropAlgObjects(nlp, fatropparams_, fatropdata_, journaller_);
    fatropalg_ = algbuilder.BuildAlgorithm();
    dirty = false;
}

int NLPApplication::Optimize()
{
    assert(!dirty);
    int ret = fatropalg_->Optimize();
    return ret;
}
// TODO: make this protected and use last_solution instead and choose other name
FatropVecBF &NLPApplication::LastSolution()
{
    assert(!dirty);
    return fatropdata_->x_curr;
}
FatropVecBF &NLPApplication::InitialGuessPrimal()
{
    assert(!dirty);
    return fatropdata_->x_initial;
}
FatropStats NLPApplication::GetStats()
{
    return fatropalg_->GetStats();
}
NLPDims NLPApplication::GetNLPDims()
{
    return nlp_->GetNLPDims();
}

// TODO move this class to a separate file
OCPApplication::OCPApplication(const shared_ptr<OCPAbstract> &ocp) : ocp_(ocp)
{
}

void OCPApplication::Build()
{
    // keep the adapter around for accessing the parameters for samplers and parameter setters
    adapter = make_shared<BFOCPAdapter>(ocp_);
    shared_ptr<FatropNLP> nlp(FatropOCPBuilder(ocp_, fatropparams_).Build(adapter));
    NLPApplication::Build(nlp);
    dirty = false;
}

vector<double> &OCPApplication::GlobalParameters()
{
    assert(!dirty);
    return adapter->GetGlobalParamsVec();
}
vector<double> &OCPApplication::StageParameters()
{
    assert(!dirty);
    return adapter->GetStageParamsVec();
}
void OCPApplication::SetInitial(vector<double> &initial_u, vector<double> &initial_x)
{
    assert(!dirty);
    adapter->SetInitial(fatropdata_, initial_u, initial_x);
}
OCPDims OCPApplication::GetOCPDims()
{
    return adapter->GetOCPDims();
}


    FatropSolution::FatropSolution(){};
    void FatropSolution::SetDims(const NLPDims &dims)
    {
        sol_primal.resize(dims.nvars);
    };
    void FatropSolution::SetPrimalSolution(const FatropVecBF &sol)
    {
        sol.copyto(sol_primal);
    }
    BasicOCPApplicationAbstract::BasicOCPApplicationAbstract(const shared_ptr<BasicOCP> &ocp) : OCPApplication(ocp)
    {
    }
    BasicOCPSolution::BasicOCPSolution(const shared_ptr<BasicOCPApplicationAbstract> &app)
    {
        SetDims(app->GetOCPDims());
    }
    BasicOCPSolution::BasicOCPSolution(){};
    void BasicOCPSolution::SetDims(const OCPDims &dims)
    {
        FatropSolution::SetDims(dims);
        nx = dims.nx.at(0);
        nu = dims.nu.at(0);
        n_stage_params = dims.n_stage_params.at(0);
        n_global_params = dims.n_global_params;
        K = dims.K;
        global_params.resize(n_global_params);
        stage_params.resize(n_stage_params);
    }
    void BasicOCPSolution::Set(const FatropVecBF &sol, const vector<double> &global_params, const vector<double> &stage_params)
    {
        FatropSolution::SetPrimalSolution(sol);
        this->global_params = global_params;
        this->stage_params = stage_params;
    }

    void BasicOCPSolution::Sample(const shared_ptr<OCPControlSampler> &sampler, vector<double> &result)
    {
        sampler->Evaluate(sol_primal, global_params, stage_params, result);
    }
    vector<double> BasicOCPSolution::Eval(const shared_ptr<BasicOCPEvaluatorBase> &evaluator) const
    {
        return evaluator->Evaluate(sol_primal, global_params, stage_params);
    }
    void BasicOCPSolution::Eval(const shared_ptr<BasicOCPEvaluatorBase> &evaluator, vector<double> &result) const
    {
        evaluator->Evaluate(sol_primal, global_params, stage_params, result);
    }
    BasicOCPApplication::BasicOCPApplication(const shared_ptr<BasicOCP> &ocp) : BasicOCPApplicationAbstract(ocp), nx_(ocp->nx_), nu_(ocp->nu_), K_(ocp->K_){};

    BasicOCPApplication::AppParameterSetter::AppParameterSetter(const shared_ptr<BFOCPAdapter> &adapter, const shared_ptr<ParameterSetter> &ps) : ParameterSetter(*ps), adapter_(adapter){};
    void BasicOCPApplication::AppParameterSetter::SetValue(const double value[])
    {
        ParameterSetter::SetValue(adapter_->GetGlobalParamsVec(), adapter_->GetStageParamsVec(), value);
    };

    shared_ptr<BasicOCPEvaluatorFactory> BasicOCPApplication::GetEvaluator(const string &sampler_name)
    {
        return eval_factories[sampler_name];
    }
    shared_ptr<BasicOCPApplication::AppParameterSetter> BasicOCPApplication::GetParameterSetter(const string &setter_name)
    {
        return make_shared<AppParameterSetter>(adapter, param_setters[setter_name]);
    }
    void BasicOCPApplication::Build()
    {
        OCPApplication::Build();
        // allocate the last solution
        last_solution.SetDims(GetOCPDims());
        dirty = false;
    }
    int BasicOCPApplication::Optimize()
    {
        int ret = NLPApplication::Optimize();
        if (ret == 0)
        {
            last_solution.SetPrimalSolution(LastSolution());
        }
        return ret;
    }
    const BasicOCPSolution &BasicOCPApplication::LastBasicOCPSolution()
    {
        return last_solution;
    }

    shared_ptr<BasicOCPApplication> BasicOCPApplicationBuilder::FromRockitInterface(const string &functions, const string &json_spec_file)
    {
        shared_ptr<DLHandler> handle = make_shared<DLHandler>(functions);
        std::ifstream t(json_spec_file);
        std::stringstream buffer;
        buffer << t.rdbuf();
        json::jobject json_spec = json::jobject::parse(buffer.str());
        auto ocptemplatebasic = BasicOCPBuilder::FromRockitInterface(handle, json_spec);
        // instantiate the BasicOCPApplication
        auto result = make_shared<BasicOCPApplication>(ocptemplatebasic);
        // add all samplers
        vector<string> sampler_names = json_spec["samplers"];
        const int nu = ocptemplatebasic->nu_;
        const int nx = ocptemplatebasic->nx_;
        const int no_stage_params = ocptemplatebasic->n_stage_params_;
        const int K = ocptemplatebasic->K_;
        for (auto sampler_name : sampler_names)
        {
            auto eval = make_shared<EvalCasGen>(handle, "sampler_" + sampler_name);
            result->eval_factories[sampler_name] = make_shared<BasicOCPEvaluatorFactory>(make_shared<EvalBaseSE>(eval), nu, nx, no_stage_params, K);
        }
        // add state samplers
        json::jobject states_offset = json_spec["states_offset"];
        vector<string> state_names = states_offset.keys();
        for (auto state_name : state_names)
        {
            vector<int> in = states_offset[state_name].as_object().array(0).get_number_array<int>("%d");
            vector<int> out = states_offset[state_name].as_object().array(1).get_number_array<int>("%d");
            result->eval_factories[string("state_") + state_name] = make_shared<BasicOCPEvaluatorFactory>(make_shared<IndexEvaluator>(false, in, out), nu, nx, no_stage_params, K);
        }
        // add control samplers
        json::jobject controls_offset = json_spec["controls_offset"];
        vector<string> control_names = controls_offset.keys();
        for (auto control_name : control_names)
        {
            vector<int> in = controls_offset[control_name].as_object().array(0).get_number_array<int>("%d");
            vector<int> out = controls_offset[control_name].as_object().array(1).get_number_array<int>("%d");
            result->eval_factories[string("control_") + control_name] = make_shared<BasicOCPEvaluatorFactory>(make_shared<IndexEvaluator>(true, in, out), nu, nx, no_stage_params, K);
        }
        // add all parameter setters
        json::jobject control_params_offset = json_spec["control_params_offset"];
        vector<string> control_params_names = control_params_offset.keys();
        for (auto control_params_name : control_params_names)
        {
            vector<int> in = control_params_offset[control_params_name].as_object().array(0).get_number_array<int>("%d");
            vector<int> out = control_params_offset[control_params_name].as_object().array(1).get_number_array<int>("%d");
            result->param_setters[control_params_name] = make_shared<ParameterSetter>(in, out, no_stage_params, in.size(), K, false);
        }
        json::jobject global_params_offset = json_spec["global_params_offset"];
        vector<string> global_params_names = global_params_offset.keys();
        for (auto global_params_name : global_params_names)
        {
            vector<int> in = global_params_offset[global_params_name].as_object().array(0).get_number_array<int>("%d");
            vector<int> out = global_params_offset[global_params_name].as_object().array(1).get_number_array<int>("%d");
            result->param_setters[global_params_name] = make_shared<ParameterSetter>(in, out, no_stage_params, in.size(), K, true);
        }
        result->Build();
        return result;
    }