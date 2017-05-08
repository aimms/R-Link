#pragma once
#include "IAimms.h"
#include "AimmsLoader.h"
#include <memory>

namespace Aimms{

class LoadedAimmsAPI: public IAimms{

#define START_LOADED_CALL         if(m_Loader.get()) {
#define END_LOADED_CALL           return Aimms::AimmsAPICallStatus::API_CALL_UNAVAILABLE;}return Aimms::AimmsAPICallStatus::CALL_PROVIDER_UNAVAILABLE;

private:
    std::unique_ptr<AimmsLoader> m_Loader;
    bool m_IsLoaded = false;

    public:
    LoadedAimmsAPI(std::wstring path){
        m_Loader = std::unique_ptr<AimmsLoader>(new AimmsLoader());
        m_IsLoaded = m_Loader.get()->Load(path);
    }
    bool IsAPILoaded(){return m_IsLoaded;}

    virtual bool Reload(std::wstring path) override{
      m_Loader.get()->Unload();
      m_IsLoaded = m_Loader.get()->Load(path);
      return m_IsLoaded;
    }
    virtual ~LoadedAimmsAPI(){}
    virtual int ProjectOpen(wchar_t* proj, int* handle) override{
        START_LOADED_CALL
        auto call = m_Loader.get()->ProjectOpen();
        if(call) return call(proj, handle);
        END_LOADED_CALL
    }
    virtual int ProjectClose(int handle, int interactive) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->ProjectClose();
      if(call) {
        auto ret = call(handle, interactive);
        ret = m_Loader.get()->Reload() == true ? 1 : 0;
        return ret;
      }
      END_LOADED_CALL
    }
    virtual int ProcedureHandleCreateA(char *procedure, int *handle, int *nargs, int *argtype ) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->ProcedureHandleCreateA();
      if(call) return call( procedure, handle, nargs, argtype);
      END_LOADED_CALL
    }
    virtual int ProcedureArgumentHandleCreate( int prochandle, int argnumber, int *arghandle) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->ProcedureArgumentHandleCreate();
      if(call) return call( prochandle, argnumber, arghandle);
      END_LOADED_CALL
    }
    virtual int AttributeNameW(int attr, AimmsStringW *text) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->AttributeNameW();
      if(call) return call( attr, text);
      END_LOADED_CALL
    }
    virtual int ValueCard(int handle, int *card) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->ValueCard();
      if(call) return call( handle, card);
      END_LOADED_CALL
    }
    virtual int AttributeDimension(int handle, int *full, int *sliced) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->AttributeDimension();
      if(call) return call( handle, full, sliced);
      END_LOADED_CALL
    }
    virtual int ValueResetHandle(int handle) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->ValueResetHandle();
      if(call) return call( handle);
      END_LOADED_CALL
    }
    virtual int ValueNextW(int handle, int *tuple, AimmsValueW *value) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->ValueNextW();
      if(call) return call( handle, tuple, value);
      END_LOADED_CALL
    }
    virtual int APILastError( int *code, wchar_t *message ) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->APILastError();
      if(call) return call( code, message);
      END_LOADED_CALL
    }

    virtual int IdentifierHandleCreateW(wchar_t *name, int *domain, int *slicing, int flags, int *handle ) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->IdentifierHandleCreateW();
      if(call) return call(name, domain, slicing, flags, handle);
      END_LOADED_CALL
    }

    virtual int AttributeDeclarationDomain(int handle, int *sethandle ) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->AttributeDeclarationDomain();
      if(call) return call( handle, sethandle);
      END_LOADED_CALL
    }

    virtual int SetOrdinalToElement( int handle, int ordinal, int *element)  override{
      START_LOADED_CALL
      auto call = m_Loader.get()->SetOrdinalToElement();
      if(call) return call( handle, ordinal, element);
      END_LOADED_CALL
    }
    virtual int SetElementToNameW(int handle, int element, AimmsStringW *name)  override{
      START_LOADED_CALL
      auto call = m_Loader.get()->SetElementToNameW();
      if(call) return call( handle, element, name);
      END_LOADED_CALL
    }

    virtual int AttributeRootDomain(int handle, int *domain)  override{
      START_LOADED_CALL
      auto call = m_Loader.get()->AttributeRootDomain();
      if(call) return call( handle, domain);
      END_LOADED_CALL
    }
    virtual int AttributeStorage( int handle, int *storage)  override{
      START_LOADED_CALL
      auto call = m_Loader.get()->AttributeStorage();
      if(call) return call( handle, storage);
      END_LOADED_CALL
    }
    virtual int AttributeType(int handle, int *type)  override{
      START_LOADED_CALL
      auto call = m_Loader.get()->AttributeType();
      if(call) return call( handle, type);
      END_LOADED_CALL
    }
    virtual int IdentifierHandleDelete(int handle)  override{
      START_LOADED_CALL
      auto call = m_Loader.get()->IdentifierHandleDelete();
      if(call) return call( handle);
      END_LOADED_CALL
    }
    virtual int AttributeElementRange(int handle, int *sethandle)  override{
      START_LOADED_CALL
      auto call = m_Loader.get()->AttributeElementRange();
      if(call) return call( handle, sethandle);
      END_LOADED_CALL
    }

    virtual int ValueRetrieveW(int handle, int *tuple, AimmsValueW *value ) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->ValueRetrieveW();
      if(call) return call( handle, tuple, value);
      END_LOADED_CALL
    }

    virtual int ValueRetrieve(int handle, int *tuple, AimmsValue *value ) override{
        START_LOADED_CALL
        auto call = m_Loader.get()->ValueRetrieve();
        if(call) return call( handle, tuple, value);
        END_LOADED_CALL
    }

    virtual int ValueNextMultiW (int handle, int* n, int* tuple,  Aimms::AimmsValueW* value) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->ValueNextMultiW();
      if(call) return call( handle, n, tuple, value);
      END_LOADED_CALL
    }

    virtual int ValueAssignW (int handle, int* tuple,  Aimms::AimmsValueW* value) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->ValueAssignW();
      if(call) return call( handle, tuple, value);
      END_LOADED_CALL
    }

    virtual int ValueAssignA (int handle, int* tuple,  Aimms::AimmsValue* value) override{
      START_LOADED_CALL
      auto call = m_Loader.get()->ValueAssignA();
      if(call) return call( handle, tuple, value);
      END_LOADED_CALL
    }

    virtual int IdentifierEmpty (int handle) override {
      START_LOADED_CALL
      auto call = m_Loader.get()->IdentifierEmpty();
      if(call) return call(handle);
      END_LOADED_CALL
    }

};
}
