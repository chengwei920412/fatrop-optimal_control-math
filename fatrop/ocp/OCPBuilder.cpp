#include "OCPBuilder.hpp"
using namespace fatrop;
OCPBuilder::OCPBuilder(const string &functions, const string &json_spec_file, bool GN, bool DDP) {
    std::ifstream t(json_spec_file);
    std::stringstream buffer;
    buffer << t.rdbuf();
    json::jobject json_spec = json::jobject::parse(buffer.str());
    K = json_spec["K"];
    const int nx = json_spec["nx"];
    const int nu = json_spec["nu"];
    const int ngI = json_spec["ngI"];
    const int ngF = json_spec["ngF"];
    const int ng_ineq = json_spec["ng_ineq"];
    const int ng_ineqF = json_spec["ng_ineqF"];
    const int n_stage_params = json_spec["n_stage_params"];
    const int n_global_params = json_spec["n_global_params"];
    shared_ptr<DLHandler> handle = make_shared<DLHandler>(functions);
    EvalCasGen BAbtf(handle, "BAbt");
    EvalCasGen bkf(handle, "bk");
    EvalCasGen RSQrqtIf = GN?EvalCasGen(handle, "RSQrqtIGN"):EvalCasGen(handle, "RSQrqtI");
    EvalCasGen rqIf(handle, "rqI");
    EvalCasGen RSQrqtf = GN?EvalCasGen(handle, "RSQrqtGN"):EvalCasGen(handle, "RSQrqt");
    EvalCasGen rqf(handle, "rqk");
    EvalCasGen RSQrqtFf = GN?EvalCasGen(handle, "RSQrqtFGN"):EvalCasGen(handle, "RSQrqtF");
    EvalCasGen rqFf(handle, "rqF");
    EvalCasGen GgtIf(handle, "GgtI");
    EvalCasGen gIf(handle, "gI");
    EvalCasGen GgtFf(handle, "GgtF");
    EvalCasGen gFf(handle, "gF");
    EvalCasGen Lkf(handle, "Lk");
    EvalCasGen LFf(handle, "LF");
    EvalCasGen Ggineqtf(handle, "Ggineqt");
    EvalCasGen gineqf(handle, "gineq");
    EvalCasGen GgineqFtf(handle, "GgineqFt");
    EvalCasGen gineqFf(handle, "gineqF");
    shared_ptr<BFOCP> ocptemplatebasic = make_shared<BFOCPBasic>(nu, nx, ngI, ngF, ng_ineq, ng_ineqF, n_stage_params, n_global_params, K,
                                                                 BAbtf,
                                                                 bkf,
                                                                 RSQrqtIf,
                                                                 rqIf,
                                                                 RSQrqtf,
                                                                 rqf,
                                                                 RSQrqtFf,
                                                                 rqFf,
                                                                 GgtIf,
                                                                 gIf,
                                                                 GgtFf,
                                                                 gFf,
                                                                 Ggineqtf,
                                                                 gineqf,
                                                                 GgineqFtf,
                                                                 gineqFf,
                                                                 Lkf,
                                                                 LFf);
    ocptempladapter = make_shared<BFOCPAdapter>(ocptemplatebasic);
    ocptempladapter->SetParams(json_spec["stage_params"].get_number_array<double>("%lf"), json_spec["global_params"].get_number_array<double>("%lf"));
    shared_ptr<OCPLSRiccati> ocplsriccati1 = make_shared<OCPLSRiccati>(ocptempladapter->GetOCPDims());
    ocplsriccati = ocplsriccati1;
    params = make_shared<FatropParams>();
    ocpscaler = make_shared<OCPNoScaling>(params);
    shared_ptr<FatropOCP> fatropocp1 = make_shared<FatropOCP>(ocptempladapter, ocplsriccati, ocpscaler);
    fatropocp = fatropocp1;
    fatropdata = make_shared<FatropData>(fatropocp->GetNLPDims(), params);
    initial_u = json_spec["initial_u"].get_number_array<double>("%lf");
    initial_x = json_spec["initial_x"].get_number_array<double>("%lf");
    lower = json_spec["lower"].get_number_array<double>("%lf");
    upper = json_spec["upper"].get_number_array<double>("%lf");
    lowerF = json_spec["lowerF"].get_number_array<double>("%lf");
    upperF = json_spec["upperF"].get_number_array<double>("%lf");
    lower.insert(lower.end(), lowerF.begin(), lowerF.end());
    upper.insert(upper.end(), upperF.begin(), upperF.end());
    SetBounds();
    SetInitial();
    // vector<double> upper = vector<double>(lower.size(), INFINITY);
    filter = make_shared<Filter>(params->maxiter + 1);
    journaller = make_shared<Journaller>(params->maxiter + 1);
    linesearch =DDP?make_shared<LineSearchDDP>(params, fatropocp, fatropdata, filter, journaller, ocplsriccati1, &(fatropocp1->ocpkktmemory_), ocptempladapter):make_shared<BackTrackingLineSearch>(params, fatropocp, fatropdata, filter, journaller);
    fatropalg = make_shared<FatropAlg>(fatropocp, fatropdata, params, filter, linesearch, journaller);
    // blasfeo_timer timer;
    // blasfeo_tic(&timer);
    // fatropalg->Optimize();
    // double el = blasfeo_toc(&timer);
    // cout << "el time " << el << endl;
}
void OCPBuilder::SetBounds()
{
    ocptempladapter->SetInitial(K, fatropdata, initial_u, initial_x);
}
void OCPBuilder::SetInitial()
{
    fatropdata->SetBounds(lower, upper);
}