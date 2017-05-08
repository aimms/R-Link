#include "Common/EnvMap.h"
#include "Common/CaseInsensitiveString.h"

#if defined(_HOST_COMPILER_GCC)
#include <unistd.h>
#endif

namespace Common{
    namespace {
#ifdef _TARGET_PLATFORM_WINDOWS
        typedef std::map<CaseInsensitiveString, std::string> String2StringMap;
#else
        typedef std::map<std::string, std::string> String2StringMap;
#endif
    }

    struct Impl{
    private:
        String2StringMap m_mapKey2Val;
    public:
        Impl(){};
        ~Impl(){};
        void reload(){
#if defined(_HOST_COMPILER_MICROSOFT)
            for (int i = 0; _environ[i]; ++i){
                addVar( _environ[i] );
            }
#endif
#if defined(_HOST_COMPILER_GCC)
            for (int i = 0; environ[i]; ++i){
                addVar( environ[i] );
            }
#endif
        }

        const char* get( const char* szKey ) const{
            String2StringMap::const_iterator itMap = m_mapKey2Val.find(szKey);
            if (itMap != m_mapKey2Val.end() ) return (*itMap).second.c_str();
            return 0;
        }

        void set( const char* szKey, const char* szValue ){
            String2StringMap::iterator itMap = m_mapKey2Val.lower_bound(szKey);
            if (itMap == m_mapKey2Val.end() || (*itMap).first != szKey){
                m_mapKey2Val.insert(itMap, String2StringMap::value_type(szKey, szValue) );
            }
            else{
                (*itMap).second = szValue;
            }
        }

        void append( const char* szKey, const char* szValue ){
            String2StringMap::iterator itMap = m_mapKey2Val.lower_bound(szKey);
            if (itMap == m_mapKey2Val.end() || (*itMap).first != szKey){
                m_mapKey2Val.insert(itMap, String2StringMap::value_type(szKey, szValue) );
            }
            else{
                (*itMap).second += szValue;
            }
        }

        void prepend( const char* szKey, const char* szValue ){
            String2StringMap::iterator itMap = m_mapKey2Val.lower_bound(szKey);
            if (itMap == m_mapKey2Val.end() || (*itMap).first != szKey){
                m_mapKey2Val.insert(itMap, String2StringMap::value_type(szKey, szValue) );
            }
            else{
                (*itMap).second = std::string(szValue) + (*itMap).second;
            }
        }

        void clear(){
            m_mapKey2Val.clear();
        }

        void remove( const char* szKey ){
            String2StringMap::iterator itMap = m_mapKey2Val.lower_bound(szKey);
            if (itMap != m_mapKey2Val.end() && (*itMap).first == szKey){
                m_mapKey2Val.erase(itMap);
            }
        }

        void asEnvVarVector( std::vector<std::string>& vectorEnvVar ) const{
            String2StringMap::const_iterator itMap = m_mapKey2Val.begin();
            while (itMap != m_mapKey2Val.end()){
                vectorEnvVar.push_back( std::string((*itMap).first.c_str()) );
                vectorEnvVar.back() += "=";
                vectorEnvVar.back() += (*itMap).second;
                itMap++;
            }

        }

        void addVar( const char* szEnvVarString ){
            std::string strTmp( szEnvVarString );

            std::string::size_type pos = strTmp.find('=');
            std::string strKey = strTmp.substr(0,pos);
            std::string strValue = strTmp.substr(pos+1);

            set( strKey.c_str(), strValue.c_str() );
        }
    };

    EnvMap::EnvMap(bool bLoadImmediately) : pImpl(new Impl()){
        if (bLoadImmediately){
            reload();
        }
    }

    EnvMap::~EnvMap(){
        delete pImpl;
    }

    void EnvMap::reload(){
        pImpl->reload();
    }

    const char* EnvMap::get( const char* szKey ) const{
        return pImpl->get(szKey);
    }

    void EnvMap::set( const char* szKey, const char* szValue ){
        pImpl->set(szKey,szValue);
    }

    void EnvMap::append( const char* szKey, const char* szValue ){
        pImpl->append(szKey,szValue);
    }

    void EnvMap::prepend( const char* szKey, const char* szValue ){
        pImpl->prepend(szKey,szValue);
    }

    void EnvMap::clear(){
        pImpl->clear();
    }

    void EnvMap::remove( const char* szKey ){
        pImpl->remove(szKey);
    }

    void EnvMap::asEnvVarVector( std::vector<std::string>& vectorEnvVar ) const{
        pImpl->asEnvVarVector(vectorEnvVar);
    }


}
