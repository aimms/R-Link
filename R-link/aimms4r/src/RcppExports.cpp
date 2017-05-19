// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// LoadAimmsAndOpenProject
bool LoadAimmsAndOpenProject(std::wstring path, std::wstring projectPath);
RcppExport SEXP aimms4r_LoadAimmsAndOpenProject(SEXP pathSEXP, SEXP projectPathSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::wstring >::type path(pathSEXP);
    Rcpp::traits::input_parameter< std::wstring >::type projectPath(projectPathSEXP);
    rcpp_result_gen = Rcpp::wrap(LoadAimmsAndOpenProject(path, projectPath));
    return rcpp_result_gen;
END_RCPP
}
// LoadAimms
bool LoadAimms(std::wstring path);
RcppExport SEXP aimms4r_LoadAimms(SEXP pathSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::wstring >::type path(pathSEXP);
    rcpp_result_gen = Rcpp::wrap(LoadAimms(path));
    return rcpp_result_gen;
END_RCPP
}
// OpenProject
bool OpenProject(std::wstring projectPath);
RcppExport SEXP aimms4r_OpenProject(SEXP projectPathSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::wstring >::type projectPath(projectPathSEXP);
    rcpp_result_gen = Rcpp::wrap(OpenProject(projectPath));
    return rcpp_result_gen;
END_RCPP
}
// CloseProject
bool CloseProject();
RcppExport SEXP aimms4r_CloseProject() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(CloseProject());
    return rcpp_result_gen;
END_RCPP
}
// TestLoadedApi
bool TestLoadedApi(std::wstring path);
RcppExport SEXP aimms4r_TestLoadedApi(SEXP pathSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::wstring >::type path(pathSEXP);
    rcpp_result_gen = Rcpp::wrap(TestLoadedApi(path));
    return rcpp_result_gen;
END_RCPP
}
// GetData
SEXP GetData(std::wstring identifierName);
RcppExport SEXP aimms4r_GetData(SEXP identifierNameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::wstring >::type identifierName(identifierNameSEXP);
    rcpp_result_gen = Rcpp::wrap(GetData(identifierName));
    return rcpp_result_gen;
END_RCPP
}
// SetData
void SetData(SEXP data, std::wstring identifierName, int modeForUnknownElements);
RcppExport SEXP aimms4r_SetData(SEXP dataSEXP, SEXP identifierNameSEXP, SEXP modeForUnknownElementsSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type data(dataSEXP);
    Rcpp::traits::input_parameter< std::wstring >::type identifierName(identifierNameSEXP);
    Rcpp::traits::input_parameter< int >::type modeForUnknownElements(modeForUnknownElementsSEXP);
    SetData(data, identifierName, modeForUnknownElements);
    return R_NilValue;
END_RCPP
}
