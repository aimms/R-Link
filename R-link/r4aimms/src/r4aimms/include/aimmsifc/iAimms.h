#ifndef _INCLUDED_aimmsifc_iAimms
#define _INCLUDED_aimmsifc_iAimms

//////////////////////////
// IMPORTANT !!!!!
//////////////////////////
//
// When you make structural changes to the interface you MUST change the version number if the following define!
//

#define aimmsifc aimmsifc_2_11

#include "iAimmsCharTypes.h"
#include <stdexcept>

#ifdef _MSC_VER
 #include <windows.h>
#else
 #ifndef CALLBACK
   #define CALLBACK
 #endif
#endif

class EventBus;

namespace aimmsifc {

class MethodNotSupportedException : public std::runtime_error
{
public:
	MethodNotSupportedException(const char *msg)
		: std::runtime_error(msg)
	{}
};

typedef int (CALLBACK* AimmsInterruptCallback)( int );

template<typename _char_t> class iAimmsT 
{
public:
	typedef _char_t CharType;
	typedef AimmsStringType<_char_t> String;
	typedef AimmsValueType<_char_t> Value;

protected:
	virtual ~iAimmsT() {}  // delete protection

public:
	virtual int AttributeName( int handle, String *name ) = 0;
	virtual int AttributeType( int handle, int *type ) = 0;
	virtual int AttributeStorage( int handle, int *storage ) = 0;
	virtual int AttributeDefaultN( int handle, Value *value ) = 0;
	virtual int AttributeDefaultS( int handle, Value *value ) = 0;
	virtual int AttributeDimension( int handle, int *full, int *sliced ) = 0;
	virtual int AttributeRootDomain( int handle, int *domain ) = 0;
	virtual int AttributeDeclarationDomain( int handle, int *domain ) = 0;
	virtual int AttributeCallDomain( int handle, int *domain ) = 0;
	virtual int AttributeRestriction( int handle, int *domainhandle ) = 0;
	virtual int AttributeSlicing( int handle, int *slicing ) = 0;
	virtual int AttributePermutation( int handle, int *permutation ) = 0;
	virtual int AttributeFlagsSet( int handle, int flags ) = 0;
	virtual int AttributeFlagsGet( int handle, int *flags ) = 0;
	virtual int AttributeElementRange( int handle, int *sethandle ) = 0;
	virtual int AttributeCompoundDimension( int handle, int *dim ) = 0;
	virtual int AttributeCompoundDomain( int handle, int *domain ) = 0;
	virtual int AttributeSetUnit( int handle, const _char_t *unit, const _char_t *convention ) = 0;
	virtual int AttributeGetUnit( int handle, String *unitName ) = 0;
	
	virtual int IdentifierHandleCreate( const _char_t *name, int *domain, int *slicing, int flags, int *handle ) = 0;
	virtual int IdentifierHandleCreatePermuted( const _char_t *name, int *domain, int *slicing, int *permutation, int flags, int *handle ) = 0;
	virtual int IdentifierHandleDelete( int handle ) = 0;
	virtual int IdentifierEmpty( int handle ) = 0;
	virtual int IdentifierCleanup( int handle ) = 0;
	virtual int IdentifierUpdate( int handle ) = 0;
	virtual int IdentifierDataVersion( int handle, int *version ) = 0;
	
	virtual int IdentifierSetUnit( int handle, const _char_t *unitname, const _char_t *convention ) = 0;
	virtual int IdentifierGetUnit( int handle, String *unitname ) = 0;

	virtual int ValueCard( int handle, int *card ) = 0;
	virtual int ValueResetHandle( int handle ) = 0;

	virtual int ValueSearchN( int handle, int *tuple, Value *value ) = 0;
	virtual int ValueNextN( int handle, int *tuple, Value *value ) = 0;
	virtual int ValueNextMultiN( int handle, int *n, int *tuple, Value *value ) = 0;
	virtual int ValueRetrieveN( int handle, int *tuple, Value *value ) = 0;
	virtual int ValueAssignN( int handle, int *tuple, Value *value ) = 0;
	virtual int ValueAssignMultiN( int handle, int n, int *tuple, Value *value ) = 0;

	virtual int ValueSearchS( int handle, int *tuple, Value *value ) = 0;
	virtual int ValueNextS( int handle, int *tuple, Value *value ) = 0;
	virtual int ValueNextMultiS( int handle, int *n, int *tuple, Value *value ) = 0;
	virtual int ValueRetrieveS( int handle, int *tuple, Value *value ) = 0;
	virtual int ValueAssignS( int handle, int *tuple, Value *value ) = 0;
	virtual int ValueAssignMultiS( int handle, int n, int *tuple, Value *value ) = 0;
	
	virtual int ValueDoubleToMapval( double value, int *mapval ) = 0;
	virtual int ValueMapvalToDouble( int mapval, double *value ) = 0;
	
	virtual int SetElementNumber( int handle, const _char_t *name, int allowCreate, int *elementNumber, int *isCreated ) = 0;
	virtual int SetAddElement( int handle, const _char_t *name, int *element ) = 0;
	virtual int SetAddElementMulti( int handle, int n, int *elementNumbers ) = 0;
	virtual int SetAddElementRecursive( int handle, const _char_t *name, int *element ) = 0;
	virtual int SetAddElementRecursiveMulti( int handle, int n, int *elementNumbers ) = 0;
	virtual int SetDeleteElement( int handle, int element ) = 0;
	virtual int SetDeleteElementMulti( int handle, int n, int *elementNumbers ) = 0;
	virtual int SetRenameElement( int handle, int element, const _char_t *name ) = 0;
	virtual int SetElementToOrdinal( int handle, int element, int *ordinal ) = 0;
	virtual int SetElementToName( int handle, int element, String *name ) = 0;
	virtual int SetOrdinalToElement( int handle, int ordinal, int *element ) = 0;
	virtual int SetOrdinalToName( int handle, int ordinal, String *name ) = 0;
	virtual int SetNameToElement( int handle, const _char_t *name, int *element ) = 0;
	virtual int SetNameToOrdinal( int handle, const _char_t *name, int *ordinal ) = 0;
	virtual int SetCompoundToTuple( int handle, int compound, int *tuple ) = 0;
	virtual int SetTupleToCompound( int handle, int *tuple, int *compound ) = 0;
	virtual int SetAddTupleToCompound( int handle, int *tuple, int *compound ) = 0;
	virtual int SetAddTupleToCompoundRecursive( int handle, int *tuple, int *compound ) = 0;
    virtual int SetIsCalendar(int calendarHandle) = 0;
    virtual int SetElementToDate(int calendarHandle, int element, int *year, int *month, int *day, int *hour, int *minute, int *second) = 0;
    virtual int SetDateToElement(int calendarHandle, int year, int month, int day, int hour, int minute, int second, int *element) = 0;

	virtual int ProcedureHandleCreate( const _char_t *procedure, int *handle, int *nargs, int *argtype ) = 0;
	virtual int ProcedureHandleDelete( int handle ) = 0;
	virtual int ProcedureArgumentHandleCreate( int prochandle, int argnumber, int *arghandle ) = 0;
	virtual int ProcedureRun( int handle, int numArgs, int *argtype, Value *arglist, int *result ) = 0;
	virtual int ProcedureAsyncRunCreate( int handle, int numArgs, int *argtype, Value *arglist, int *request ) = 0;
	virtual int ProcedureAsyncRunDelete( int request ) = 0;
	virtual int ProcedureAsyncRunStatus( int request, int *status, int *result ) = 0;
	
	virtual int APIPassMessage( int severity, const _char_t *message ) = 0;
	virtual int APIStatus( int *status ) = 0;
	virtual int APILastError( int *code, _char_t *message, int messageSize = -1 ) = 0;
	
	virtual int ProjectOpen( const _char_t *commandline, int *handle ) = 0;
	virtual int ServerProjectOpen( const _char_t *commandline, int *handle ) = 0;
	virtual int ProjectClose( int handle, int interactive ) = 0;
	virtual int ProjectSwitch( const _char_t *prjName, int interactive ) = 0;
	
	virtual int ProjectWindow( void *window ) = 0;
	virtual int ProjectInterface( void **ifc ) = 0;
	
	virtual int ControlGet( int timeout ) = 0;
	virtual int ControlRelease( void ) = 0;
	
	virtual int InterruptCallbackInstall( AimmsInterruptCallback cbFcn ) = 0;
	
	virtual int InterruptPending( void ) = 0;
	
	virtual int ThreadAttach( void ) = 0;
	virtual int ThreadDetach( void ) = 0;

	virtual int ErrorStatus(void) = 0;
	virtual int ErrorCount(void) = 0;
	virtual const _char_t * ErrorMessage(int errNo) = 0;
	virtual int ErrorSeverity(int errNo) = 0;
	virtual const _char_t * ErrorCode(int errNo) = 0;
	virtual const _char_t * ErrorCategory(int errNo) = 0;
	virtual int ErrorNumberOfLocations(int errNo) = 0;
	virtual const _char_t * ErrorFilename(int errNo) = 0;
	virtual const _char_t * ErrorNode(int errNo, int pos) = 0;
	virtual const _char_t * ErrorAttributeName(int errNo, int pos) = 0;
	virtual int ErrorLine(int errNo, int pos) = 0;
	virtual int ErrorColumn(int errNo) = 0;
	virtual int ErrorDelete(int errNo) = 0;
	virtual int ErrorClear(void) = 0;
	virtual int ErrorRaise(int severity, const _char_t *message, const _char_t *code) = 0;

	virtual bool AimmsIsUnicode() = 0;

	virtual wchar_t *ConvertToAimms(wchar_t *outBuf, size_t outBufSize, const _char_t *inBuf) = 0;
	virtual char *ConvertToAimms(char *outBuf, size_t outBufSize, const _char_t *inBuf) = 0;
	virtual _char_t *ConvertFromAimms(_char_t *outBuf, size_t outBufSize, const wchar_t *inBuf) = 0;
	virtual _char_t *ConvertFromAimms(_char_t *outBuf, size_t outBufSize, const char *inBuf) = 0;

	virtual bool AddVirtualDataManagerFileSystem(void *ptr, bool deleteOnExit) = 0;
	virtual bool RemoveVirtualDataManagerFileSystem(void *ptr) = 0;
	virtual bool NewCasesUse64Bit() = 0;
	virtual class ::EventBus *AimmsGetEventBus() = 0;
};

typedef iAimmsT<wchar_t> iAimmsW;
typedef iAimmsT<char> iAimmsA;

#if defined(UNICODE) || defined(_UNICODE)
	typedef iAimmsW iAimms;
#else
	typedef iAimmsA iAimms;
#endif


// lots of enumerations, matching the constants in aimmsapi.h

    enum E_ReturnValue{
        Failure = 0,
        Success = 1
    };

    enum E_Boolean{
        False = 0, 
        True = 1
    };

    struct Storage{
        enum{
            Handle  = 0x00000000,
            Double  = 0x00000001,
            Int     = 0x00000002,
            Binary  = 0x00000003,
            String  = 0x00000004
        };
    };

    enum{
        Input           = 0x00000010,
        Output          = 0x00000020,
        Input_Output    = 0x00000040
    };

    struct IdentifierType{
        enum{
            None                =  0,
            SimpleSetRoot       =  1,
            SimpleSetSubset     =  2,
            CompoundSetRoot     = 11,
            CompoundSetSubset   = 12,
            SetRelation         = 20,
            SetIndexed          = 21,
            ParameterNumerics   = 30,
            ParameterElements   = 31,
            ParameterStrings    = 32,
            ParameterUnits      = 33,
            Variable            = 40,
            VariableSuffix      = 50,
            EquationSuffix      = 51,
            ModelSuffix         = 52,
            FileTokenSuffix     = 53,
            Index               = 60, 
            ElementVariable     = 70
        };
    };

    struct Flag{
        enum{
            Ordered             = 0x00000001,
            RetainSpecials      = 0x00000002,
            Raw                 = 0x00000004,
            NoInactiveData      = 0x00000008,
            Readonly            = 0x00000010,
            ElementsAsOrdinals  = 0x00000020,
            Units               = 0x00000040,
            ElementsAsStrings   = 0x00000080,         // For XML
            SaveRealDomain      = 0x00000100,         // For XML
            IsLocal             = 0x00000200,
            CodeXmlCharacters   = 0x00000400,         // For XML
            DoNotCheckTuples    = 0x00000800,
            SaveDeclDomain      = 0x00001000         // For XML
        };
    };

    struct SeverityCode{
        enum{
            Never        = 0,
            Error        = 1,
            Warning      = 2,
            Remark       = 3,
            All          = 4
        };
    };

    enum{
        NoElement = 0
    };

    enum{
        NoSymbol = 0
    };

    enum{
        NullRequestNumber = -1,
        MaxDimension = 32,
        MaxNumbOfProcArg = 256,
        ErrorBufSize   = 4096
    };

    enum{
        NullHandleNumber = 0
    };
    enum{
        NoOrdinal = 0
    };

    enum{
        MaxPendingRequests     = 32
    };

    struct Status{
        enum{
            Pending      = 0,
            Running      = 1,
            Finished     = 2,
            Deleted      = 3,
            Unknown      = 4
        };
    };
    enum{
        StartHandleNumbering   = 25600
    };
    enum{
        StartProjectNumbering  = 25600
    };
    enum{
        WaitInfinite      = 0xFFFFFFFF
    };

    // Flags used in InterruptCallback
    struct InterruptContext{
        enum{
            Executing = 0,
            Solving   = 1
        };
    };

    // Return values for InterruptCallback
    struct InterruptReturn{
        enum{
            Not      = 0,
            Execute  = 1,
            Solve    = 2
        };
    };

    struct Error{
        enum {
            None                            =   0 ,
            MEMORY                          = 100 ,
            INCORRECT_IDENTIFIER_NAME             ,
            INCORRECT_PROCEDURE_NAME              ,
            NO_VALID_SUFFIX                       ,
            INCORRECT_IDENTIFIER_TYPE             ,
            UNEXPECTED_CALL_DOMAIN                ,
            UNEXPECTED_SLICING                    ,
            SLICING_LAST_DIM_INDEXED_SET          ,
            SLICE_NOT_IN_ROOT                     ,
            NULL_HANDLE                           ,
            NO_VALID_HANDLE                       ,
            NO_IDENTIFIER_HANDLE                  ,
            IS_PROCEDURE_HANDLE                   ,
            NO_PROCEDURE_HANDLE                   ,
            NO_VALID_FLAG                         ,
            NO_INDEXED_IDENTIFIER                 ,
            NO_ELEMENT_PARAMETER                  ,
            ROOT_DOMAIN                           ,
            CALL_DOMAIN_NO_SUBSET_OF_ROOT         ,
            EMPTY_DEFINED_IDENTIFIER              ,
            RESET_HANDLE_SCALAR                   ,
            SEARCH_SCALAR                         ,
            NEXT_SCALAR                           ,
            FIRST_RESET_OR_SEARCH                 ,
            NO_NEXT_ELEMENT                       ,
            CHANGE_DEFINED_IDENTIFIER             ,
            CHANGE_NON_OUTPUT_ARGUMENT            ,
            NO_SIMPLE_COMPOUND_ROOT_SET           ,
            NO_SIMPLE_SET                         ,
            NO_SIMPLE_ROOT_SET                    ,
            NO_COMPOUND_SET                       ,
            NO_SIMPLE_OR_COMPOUND_SET             ,
            ELEMENT_ALREADY_IN_SET                ,
            ELEMENT_NOT_IN_ROOT                   ,
            ELEMENT_NOT_IN_SET                    ,
            ORDINAL_NOT_IN_SET                    ,
            ELEMENT_NAME_NOT_IN_SET               ,
            TUPLE_NOT_IN_ROOT_OF_COMPOUND_SET     ,
            TUPLE_NOT_IN_ROOT_DOMAIN              ,
            TUPLE_NOT_IN_DECLARATION_DOMAIN       ,
            TUPLE_NOT_IN_CALL_DOMAIN              ,
            TUPLE_NOT_IN_COMPOUND_DOMAIN          ,
            NO_PROCEDURE                          ,
            INVALID_ELEMENT_NUMBER                ,
            ELEMENT_NOT_IN_RANGE                  ,
            NO_NON_DEFAULT_ELEMENT_LEFT           ,
            NOT_0_OR_1                            ,
            INCORRECT_STORAGE_TYPE_FOR_NON_SCALAR ,
            INCORRECT_DIMENSION_FOR_ARGUMENT      ,
            PROC_DIFFERENT_IDENTIFIER_TYPES       ,
            PROC_DIFFERENT_STORAGE_TYPES          ,
            PROC_DIFFERENT_DEFAULT_VALUES         ,
            PROC_NOT_SAME_ROOT_DOMAIN             ,
            PROC_INCORRECT_ROOT_DOMAIN            ,
            PROC_INCORRECT_GLOBAL_DECL_DOMAIN     ,
            PROC_INCORRECT_LOCAL_DECL_DOMAIN      ,
            PROC_INCORRECT_GLOBAL_SET_RANGE       ,
            PROC_INCORRECT_LOCAL_SET_RANGE        ,
            DELETE_PROCEDURE_HANDLE               ,
            DELETE_PASSED_HANDLE                  ,
            INCORRECT_SEVERITY_CODE               ,
            INTEGER_SPECIAL_VALUE                 ,
            LARGE_DOUBLE_TO_INT                   ,
            CHANGE_NON_OUTPUT_SUPERSET            ,
            CHANGE_DEFINED_SUPERSET               ,
            PROC_RELATION                         ,
            INCORRECT_HANDLE                      ,
            INCORRECT_HANDLE_CALL_DOMAIN          ,
            INCORRECT_HANDLE_PROCEDURE_ARGUMENT   ,
            NULL_AIMMSSTRING                      ,
            SCHEDULE_PROCEDURE                    ,
            INCORRECT_WORKING_DIRECTORY           ,
            OPEN_PROJECT                          ,
            STILL_REQUESTS_RUNNING_OR_PENDING     ,
            TO_MANY_PENDING_REQUESTS              ,
            INVALID_REQUEST_HANDLE_NUMBER         ,
            REQUEST_HANDLE_ALREADY_DELETED        ,
            DELETING_RUNNING_REQUEST              ,
            REMOVING_PENDING_REQUEST_FAILED       ,
            DOUBLE_OUT_OF_RANGE                   ,
            INT_OUT_OF_RANGE                      ,
            ASSIGN_MULTI_NOT_FOR_SCALAR           ,
            NEXT_MULTI_NOT_FOR_SCALAR             ,
            NEXT_MULTI_NO_POSITIVE_N              ,
            COMPOUND_NOT_IN_ROOT_OF_COMPOUND_SET  ,
            INVALID_PROJECT_HANDLE                ,
            NO_CONTROL                            ,
            ELEMENT_TO_ADD_NOT_IN_ROOT            ,
            PROCEDURE_ALREADY_RUNNING             ,
            PROCEDURE_RUNNING_FAILURE             ,
            ANY_PROJECT_FAILURE                   ,
            PERMUTE_VALUE_INCORRECT               ,
            PERMUTE_OF_SLICED_DIM_NOT_ZERO        ,
            DOUBLY_NONZERO_VALUES_IN_PERMUTATION  ,
            PERMUTED_LAST_DIM_INDEXED_SET         ,
            UNEXPECTED_PERMUTATION                ,
            CHANGE_PERMUTED_HANDLE                ,
            TUPLE_IS_NULL                         ,
            NO_VALID_POINTER                      ,
            PROCEDURE_NO_BODY_CALL                ,
            UNIT_DOES_NOT_APPLY_TO_IDENTIFIER     ,
            LICENSE_FAILURE                       ,
            NYI
        };
    };




} // end namespace

#endif // _INCLUDED_aimmsifc_iAimms

