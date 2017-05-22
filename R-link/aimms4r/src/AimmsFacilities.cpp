/*  This file is part of R-Link.

    R-Link is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    R-Link is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with R-Link.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <Rcpp.h>
using namespace Rcpp;
#include <iostream>
#include "AimmsEnvironmentState.h"
#include "AimmsAPI.h"

#include "ReadTable.h"
#include "RValue.h"
#include "AimmsException.h"
#include <stdexcept>
#include "WriteTable.h"

// This is a simple function using Rcpp that creates an R list
// containing a character vector and a numeric vector.
//
// Learn more about how to use Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//
// and browse examples of code using Rcpp at:
//
//   http://gallery.rcpp.org/
//

// [[Rcpp::export]]
bool LoadAimmsAndOpenProject(std::wstring path, std::wstring projectPath) {

    bool aimmsLibLoaded = Aimms::AimmsEnvironmentState::getAimmsResources().loadAimmsAPI(path);

    if(!aimmsLibLoaded) {
        std::wcout << "Couldn't load AIMMS library in " << path << std::endl;
        return false;
    }
    auto api = Aimms::AimmsEnvironmentState::getAimmsResources().getAimmsAPI();
    if(!api) {
      std::cout << "Load Aimms library first.."<< std::endl;
      return false;
    }

    int handle = 0;
    bool ret = api->ProjectOpen(const_cast<wchar_t*> ( projectPath.c_str() ), &handle);

    if(ret && handle){
      auto state = Aimms::AimmsEnvironmentState::getInstance();
      state.getAimmsResources().setProjectHandle(handle);
      return true;
    }
    // Maybe show a message in case the user couldnt get a handle
    return false;

}

// [[Rcpp::export]]
bool LoadAimms(std::wstring path){
  bool aimmsLibLoaded = Aimms::AimmsEnvironmentState::getAimmsResources().loadAimmsAPI(path);

  if(!aimmsLibLoaded) {
    std::wcout << "Couldn't load AIMMS library in " << path << std::endl;
    return false;
  }
  return true;
}

// [[Rcpp::export]]
bool OpenProject(std::wstring projectPath) {

  auto api = Aimms::AimmsEnvironmentState::getAimmsResources().getAimmsAPI();
  if(!api) {
      std::cout << "Load Aimms library first.."<< std::endl;
      return false;
  }

  int handle = 0;
  bool ret = api->ProjectOpen(const_cast<wchar_t*> ( projectPath.c_str() ), &handle);
  if(ret && handle){
    auto state = Aimms::AimmsEnvironmentState::getInstance();
    state.getAimmsResources().setProjectHandle(handle);
    return true;
  }
  // Maybe show a message in case the user couldnt get a handle
  return false;
}


// [[Rcpp::export]]
bool CloseProject() {

  auto api = Aimms::AimmsEnvironmentState::getInstance().getAimmsResources().getAimmsAPI();
  if(!api) return false;


  auto state = Aimms::AimmsEnvironmentState::getInstance();
  if(state.getAimmsResources().getProjectHandle()){
    state.getAimmsResources().cleanSpace();
  }

  int handle =       state.getAimmsResources().getProjectHandle();
  if(!handle) return false;


  if(api->ProjectClose(handle, 0)){
      //Closed successfully
      state.getAimmsResources().setProjectHandle(0);
      return true;
  }
  return false;
}


// [[Rcpp::export]]
bool TestLoadedApi(std::wstring path) {
  auto p = Aimms::AimmsAPI::getAimmsAPI(path);
  if(p) return true;
  return false;
}

// [[Rcpp::export]]
SEXP GetData(std::wstring identifierName) {
  auto space = Aimms::AimmsEnvironmentState::getAimmsResources().getSpace();
  if(!space) throw std::logic_error("You need to load lib Aimms dll & open an Aimms Project first..");
  std::shared_ptr<Aimms::MultiDimIdentifier> multiDim = space->obtainIdentifier(identifierName);
  if ( !multiDim->getDimension() ){

    R::Value rval;
    multiDim->storeScalarTo(rval);
    return rval.createSExpr();
  }
  R::ReadTable dt;

  multiDim->storeTo(dt);

  return dt.createDataFrame();
}


// [[Rcpp::export]]
void SetData(SEXP data, std::wstring identifierName, int modeForUnknownElements = 0) {
  if(modeForUnknownElements < 0 || modeForUnknownElements > 1){
    throw std::logic_error("The mode for unknown elements should be either 0 or 1.");
  }
  auto space = Aimms::AimmsEnvironmentState::getAimmsResources().getSpace();
  if(!space) throw std::logic_error("You need to load lib Aimms dll & open an Aimms Project first..");
  std::shared_ptr<Aimms::MultiDimIdentifier> multiDim = space->obtainIdentifier(identifierName);

  //the boolean addNewElement will be taken into account from the Identifier's sets
  multiDim->setModeForNewElements(modeForUnknownElements);

  if(!multiDim->getDimension()){
      R::Value rval(data);
      multiDim->loadScalarFrom(rval);
      return;
  }

  Rcpp::DataFrame df(data);
  R::WriteTable wt(&df);
  if( multiDim->loadFrom(wt) ){
    //Successful write to Identifier
  }

  space->clear();
  return;
}
