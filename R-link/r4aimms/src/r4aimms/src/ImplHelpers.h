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
#include <vector>

#ifndef WINAPI
    #define WINAPI
 #endif

enum ALL_AIMMS_API_FUNCTIONS {
	FCN_AttributeName,
	FCN_AttributeType,
	FCN_AttributeStorage,
	FCN_AttributeDefault,
	FCN_AttributeDimension,
	FCN_AttributeRootDomain,
	FCN_AttributeDeclarationDomain,
	FCN_AttributeCallDomain,
	FCN_AttributeRestriction,
	FCN_AttributeSlicing,
	FCN_AttributePermutation,
	FCN_AttributeFlagsSet,
	FCN_AttributeFlagsGet,
	FCN_AttributeFlags,
	FCN_AttributeElementRange,
	FCN_AttributeCompoundDimension,
	FCN_AttributeCompoundDomain,
	FCN_AttributeSetUnit,
	FCN_AttributeGetUnit,
	
	FCN_IdentifierHandleCreate,
	FCN_IdentifierHandleCreatePermuted,
	FCN_IdentifierHandleDelete,
	FCN_IdentifierEmpty,
	FCN_IdentifierCleanup,
	FCN_IdentifierUpdate,
	FCN_IdentifierDataVersion,
	
	FCN_IdentifierSetUnit,
	FCN_IdentifierGetUnit,

	FCN_ValueCard,
	FCN_ValueResetHandle,
	FCN_ValueSearch,
	FCN_ValueNext,
	FCN_ValueNextMulti,
	FCN_ValueRetrieve,
	FCN_ValueAssign,
	FCN_ValueAssignMulti,
	FCN_ValueDoubleToMapval,
	FCN_ValueMapvalToDouble,
	
	FCN_SetElementNumber,
	FCN_SetAddElement,
	FCN_SetAddElementMulti,
	FCN_SetAddElementRecursive,
	FCN_SetAddElementRecursiveMulti,
	FCN_SetDeleteElement,
	FCN_SetDeleteElementMulti,
	FCN_SetRenameElement,
	FCN_SetElementToOrdinal,
	FCN_SetElementToName,
	FCN_SetOrdinalToElement,
	FCN_SetOrdinalToName,
	FCN_SetNameToElement,
	FCN_SetNameToOrdinal,
	FCN_SetCompoundToTuple,
	FCN_SetTupleToCompound,
	FCN_SetAddTupleToCompound,
	FCN_SetAddTupleToCompoundRecursive,
    FCN_SetIsCalendar,
    FCN_SetElementToDate,
    FCN_SetDateToElement,
	
	FCN_ProcedureHandleCreate,
	FCN_ProcedureHandleDelete,
	FCN_ProcedureArgumentHandleCreate,
	FCN_ProcedureRun,
	FCN_ProcedureAsyncRunCreate,
	FCN_ProcedureAsyncRunDelete,
	FCN_ProcedureAsyncRunStatus,
	
	FCN_APIPassMessage,
	FCN_APIStatus,
	FCN_APILastError,
	
	FCN_ProjectOpen,
	FCN_ServerProjectOpen,
	FCN_ProjectClose,
	FCN_ProjectSwitch,
	
	FCN_ProjectWindow,
	FCN_ProjectInterface,
	
	FCN_ControlGet,
	FCN_ControlRelease,
	
	FCN_InterruptCallbackInstall,
	
	FCN_InterruptPending,
	
	FCN_ThreadAttach,
	FCN_ThreadDetach,

	FCN_ErrorStatus,
	FCN_ErrorCount,
	FCN_ErrorMessage,
	FCN_ErrorSeverity,
	FCN_ErrorCode,
	FCN_ErrorCategory,
	FCN_ErrorNumberOfLocations,
	FCN_ErrorFilename,
	FCN_ErrorNode,
	FCN_ErrorAttributeName,
	FCN_ErrorLine,
	FCN_ErrorColumn,
	FCN_ErrorDelete,
	FCN_ErrorClear,
	FCN_ErrorRaise,

	FCN_AddVirtualDataManagerFileSystem,
	FCN_RemoveVirtualDataManagerFileSystem,
	FCN_NewCasesUse64Bit,
	FCN_AimmsGetEventBus,

	FCN_MAX
};

#define DECLARE_FUNCTION_POINTERS std::vector<void*> m_Fcn

#define INIT_FUNCTION_POINTERS  m_Fcn(FCN_MAX,(void*)0)


#if defined(_MSC_VER) && defined(_TARGET_ARCHITECTURE_X86)
	#define DECORATED_DLL_NAME(sym,n) "_" #sym "@" #n
	#define DECORATED_DLL_NAME_A(sym,n) "_" #sym "A@" #n
	#define DECORATED_DLL_NAME_W(sym,n) "_" #sym "W@" #n
#else
	#define DECORATED_DLL_NAME(sym,n) #sym 
	#define DECORATED_DLL_NAME_A(sym,n) #sym "A"
	#define DECORATED_DLL_NAME_W(sym,n) #sym "W"
#endif


#define GET_FCN(fieldEnum, methodName, rval, args) \
	if (!m_Fcn[fieldEnum]) { \
		m_Fcn[fieldEnum] = m_Library.GetProcAddress(methodName); \
		if (!m_Fcn[fieldEnum]) { std::string msg(methodName); msg += " not found."; throw MethodNotSupportedException(msg.c_str()); } \
	} \
	typedef rval (WINAPI *fcntype) args ; \
	fcntype fcn = (fcntype)(m_Fcn[fieldEnum]); 


#define DECLARE_ALL_NONCHAR_METHODS \
	virtual int AttributeType( int handle, int *type )                                                                             \
	{ 																															   \
		GET_FCN(FCN_AttributeType, DECORATED_DLL_NAME(AimmsAttributeType,8), int, (int, int*) ) 								   \
		return fcn(handle, type); 																								   \
	} 																															   \
																																   \
	virtual int AttributeStorage( int handle, int *storage ) 																	   \
	{ 																															   \
		GET_FCN(FCN_AttributeStorage, DECORATED_DLL_NAME(AimmsAttributeStorage,8), int, (int, int*) ) 							   \
		return fcn(handle, storage); 																							   \
	} 																															   \
																																   \
	virtual int AttributeDimension( int handle, int *full, int *sliced ) 														   \
	{ 																															   \
		GET_FCN(FCN_AttributeDimension, DECORATED_DLL_NAME(AimmsAttributeDimension,12), int, (int, int*, int*) ) 				   \
		return fcn(handle, full, sliced); 																						   \
	} 																															   \
																																   \
	virtual int AttributeRootDomain( int handle, int *domain ) 																	   \
	{ 																															   \
		GET_FCN(FCN_AttributeRootDomain, DECORATED_DLL_NAME(AimmsAttributeRootDomain,8), int, (int, int*) ) 					   \
		return fcn(handle, domain); 																							   \
	} 																															   \
																																   \
	virtual int AttributeDeclarationDomain( int handle, int *domain )															   \
	{ 																															   \
		GET_FCN(FCN_AttributeDeclarationDomain, DECORATED_DLL_NAME(AimmsAttributeDeclarationDomain,8), int, (int, int*) ) 		   \
		return fcn(handle, domain);																								   \
	}																															   \
																																   \
	virtual int AttributeCallDomain( int handle, int *domain )																	   \
	{ 																															   \
		GET_FCN(FCN_AttributeCallDomain, DECORATED_DLL_NAME(AimmsAttributeCallDomain,8), int, (int, int*) ) 					   \
		return fcn(handle, domain);																								   \
	}																															   \
																																   \
	virtual int AttributeRestriction( int handle, int *domainhandle ) 															   \
	{ 																															   \
		GET_FCN(FCN_AttributeRestriction, DECORATED_DLL_NAME(AimmsAttributeRestriction,8), int, (int, int*) ) 					   \
		return fcn(handle, domainhandle);																						   \
	}																															   \
																																   \
	virtual int AttributeSlicing( int handle, int *slicing )																	   \
	{ 																															   \
		GET_FCN(FCN_AttributeSlicing, DECORATED_DLL_NAME(AimmsAttributeSlicing,8), int, (int, int*) ) 							   \
		return fcn(handle, slicing);																							   \
	}																															   \
																																   \
	virtual int AttributePermutation( int handle, int *permutation )															   \
	{ 																															   \
		GET_FCN(FCN_AttributePermutation, DECORATED_DLL_NAME(AimmsAttributePermutation,8), int, (int, int*) ) 					   \
		return fcn(handle, permutation);																						   \
	}																															   \
																																   \
	virtual int AttributeFlagsSet( int handle, int flags )																		   \
	{ 																															   \
		GET_FCN(FCN_AttributeFlagsSet, DECORATED_DLL_NAME(AimmsAttributeFlagsSet,8), int, (int, int) ) 							   \
		return fcn(handle, flags);																								   \
	}																															   \
																																   \
	virtual int AttributeFlagsGet( int handle, int *flags )																		   \
	{ 																															   \
		GET_FCN(FCN_AttributeFlagsGet, DECORATED_DLL_NAME(AimmsAttributeFlagsGet,8), int, (int, int*) ) 						   \
		return fcn(handle, flags);																								   \
	}																															   \
																																   \
	virtual int AttributeElementRange( int handle, int *sethandle )																   \
	{ 																															   \
		GET_FCN(FCN_AttributeElementRange, DECORATED_DLL_NAME(AimmsAttributeElementRange,8), int, (int, int*) ) 				   \
		return fcn(handle, sethandle);																							   \
	}																															   \
																																   \
	virtual int AttributeCompoundDimension( int handle, int *dim )																   \
	{ 																															   \
		GET_FCN(FCN_AttributeCompoundDimension, DECORATED_DLL_NAME(AimmsAttributeCompoundDimension,8), int, (int, int*) ) 		   \
		return fcn(handle, dim);																								   \
	}																															   \
																																   \
	virtual int AttributeCompoundDomain( int handle, int *domain )																   \
	{ 																															   \
		GET_FCN(FCN_AttributeCompoundDomain, DECORATED_DLL_NAME(AimmsAttributeCompoundDomain,8), int, (int, int*) ) 			   \
		return fcn(handle, domain);																								   \
	}																															   \
																																   \
	virtual int IdentifierHandleDelete( int handle )																			   \
	{ 																															   \
		GET_FCN(FCN_IdentifierHandleDelete, DECORATED_DLL_NAME(AimmsIdentifierHandleDelete,4), int, (int) ) 					   \
		return fcn(handle);																										   \
	}																															   \
																																   \
	virtual int IdentifierEmpty( int handle )																					   \
	{ 																															   \
		GET_FCN(FCN_IdentifierEmpty, DECORATED_DLL_NAME(AimmsIdentifierEmpty,4), int, (int) ) 									   \
		return fcn(handle);																										   \
	}																															   \
																																   \
	virtual int IdentifierCleanup( int handle )																					   \
	{ 																															   \
		GET_FCN(FCN_IdentifierCleanup, DECORATED_DLL_NAME(AimmsIdentifierCleanup,4), int, (int) ) 								   \
		return fcn(handle);																										   \
	}																															   \
																																   \
	virtual int IdentifierUpdate( int handle )																					   \
	{ 																															   \
		GET_FCN(FCN_IdentifierUpdate, DECORATED_DLL_NAME(AimmsIdentifierUpdate,4), int, (int) ) 								   \
		return fcn(handle);																										   \
	}																															   \
																																   \
	virtual int IdentifierDataVersion( int handle, int *version )																   \
	{ 																															   \
		GET_FCN(FCN_IdentifierDataVersion, DECORATED_DLL_NAME(AimmsIdentifierDataVersion,8), int, (int, int*) ) 				   \
		return fcn(handle, version);																							   \
	}																															   \
																																   \
	virtual int ValueCard( int handle, int *card )																				   \
	{ 																															   \
		GET_FCN(FCN_ValueCard, DECORATED_DLL_NAME(AimmsValueCard,8), int, (int, int*) ) 										   \
		return fcn(handle, card);																								   \
	}																															   \
																																   \
	virtual int ValueResetHandle( int handle )																					   \
	{ 																															   \
		GET_FCN(FCN_ValueResetHandle, DECORATED_DLL_NAME(AimmsValueResetHandle,4), int, (int) ) 								   \
		return fcn(handle);																										   \
	}																															   \
																																   \
	virtual int ValueDoubleToMapval( double value, int *mapval )																   \
	{ 																															   \
		GET_FCN(FCN_ValueDoubleToMapval, DECORATED_DLL_NAME(AimmsValueDoubleToMapval,12), int, (double, int*) ) 				   \
		return fcn(value, mapval);																								   \
	}																															   \
																																   \
	virtual int ValueMapvalToDouble( int mapval, double *value )																   \
	{ 																															   \
		GET_FCN(FCN_ValueMapvalToDouble, DECORATED_DLL_NAME(AimmsValueMapvalToDouble,8), int, (int, double*) ) 					   \
		return fcn(mapval, value);																								   \
	}																															   \
																																   \
	virtual int SetAddElementMulti( int handle, int n, int *elementNumbers )													   \
	{ 																															   \
		GET_FCN(FCN_SetAddElementMulti, DECORATED_DLL_NAME(AimmsSetAddElementMulti,12), int, (int, int, int*) ) 				   \
		return fcn(handle, n, elementNumbers);																					   \
	}																															   \
																																   \
	virtual int SetAddElementRecursiveMulti( int handle, int n, int *elementNumbers )											   \
	{ 																															   \
		GET_FCN(FCN_SetAddElementRecursiveMulti, DECORATED_DLL_NAME(AimmsSetAddElementRecursiveMulti,12), int, (int, int, int*) )  \
		return fcn(handle, n, elementNumbers);																					   \
	}																															   \
																																   \
	virtual int SetDeleteElement( int handle, int element )																		   \
	{ 																															   \
		GET_FCN(FCN_SetDeleteElement, DECORATED_DLL_NAME(AimmsSetDeleteElement,8), int, (int, int) ) 							   \
		return fcn(handle, element);																							   \
	}																															   \
																																   \
	virtual int SetDeleteElementMulti( int handle, int n, int *elementNumbers )													   \
	{ 																															   \
		GET_FCN(FCN_SetDeleteElementMulti, DECORATED_DLL_NAME(AimmsSetDeleteElementMulti,12), int, (int, int, int*) ) 			   \
		return fcn(handle, n, elementNumbers);																					   \
	}																															   \
																																   \
	virtual int SetElementToOrdinal( int handle, int element, int *ordinal )													   \
	{ 																															   \
		GET_FCN(FCN_SetElementToOrdinal, DECORATED_DLL_NAME(AimmsSetElementToOrdinal,12), int, (int, int, int*) ) 				   \
		return fcn(handle, element, ordinal);																					   \
	}																															   \
																																   \
	virtual int SetOrdinalToElement( int handle, int ordinal, int *element )													   \
	{ 																															   \
		GET_FCN(FCN_SetOrdinalToElement, DECORATED_DLL_NAME(AimmsSetOrdinalToElement,12), int, (int, int, int*) ) 				   \
		return fcn(handle, ordinal, element);																					   \
	}																															   \
																																   \
	virtual int SetCompoundToTuple( int handle, int compound, int *tuple )														   \
	{ 																															   \
		GET_FCN(FCN_SetCompoundToTuple, DECORATED_DLL_NAME(AimmsSetCompoundToTuple,12), int, (int, int, int*) ) 				   \
		return fcn(handle, compound, tuple);																					   \
	}																															   \
																																   \
	virtual int SetTupleToCompound( int handle, int *tuple, int *compound )														   \
	{ 																															   \
		GET_FCN(FCN_SetTupleToCompound, DECORATED_DLL_NAME(AimmsSetTupleToCompound,12), int, (int, int*, int*) ) 				   \
		return fcn(handle, tuple, compound);																					   \
	}																															   \
																																   \
	virtual int SetAddTupleToCompound( int handle, int *tuple, int *compound )													   \
	{ 																															   \
		GET_FCN(FCN_SetAddTupleToCompound, DECORATED_DLL_NAME(AimmsSetAddTupleToCompound,12), int, (int, int*, int*) ) 			   \
		return fcn(handle, tuple, compound);																					   \
	}																															   \
																																   \
	virtual int SetAddTupleToCompoundRecursive( int handle, int *tuple, int *compound )													 \
	{ 																																	 \
		GET_FCN(FCN_SetAddTupleToCompoundRecursive, DECORATED_DLL_NAME(AimmsSetAddTupleToCompoundRecursive,12), int, (int, int*, int*) ) \
		return fcn(handle, tuple, compound);																							 \
	}																																	 \
                                                                                                                                             \
    virtual int SetIsCalendar(int calendarHandle)      \
    {                                                                                                                                        \
        GET_FCN(FCN_SetIsCalendar, DECORATED_DLL_NAME(AimmsSetIsCalendar, 4), int, (int) )   \
            return fcn(calendarHandle);													\
    }                                                                                                                                        \
                                                                                                                                             \
    virtual int SetElementToDate(int calendarHandle, int element, int *year, int *month, int *day, int *hour, int *minute, int *second)      \
    {                                                                                                                                        \
        GET_FCN(FCN_SetElementToDate, DECORATED_DLL_NAME(AimmsSetElementToDate, 32), int, (int, int, int*, int*, int*, int*, int*, int*) )   \
            return fcn(calendarHandle, element, year, month, day, hour, minute, second);													\
    }                                                                                                                                        \
                                                                                                                                             \
    virtual int SetDateToElement(int calendarHandle, int year, int month, int day, int hour, int minute, int second, int *element)          \
    {                                                                                                                                       \
        GET_FCN(FCN_SetDateToElement, DECORATED_DLL_NAME(AimmsSetDateToElement, 32), int, (int, int, int, int, int, int, int, int*))        \
            return fcn(calendarHandle, year, month, day, hour, minute, second, element);													\
    }                                                                                                                                       \
                                                                                                                                            \
	virtual int ProcedureHandleDelete( int handle )																						 \
	{ 																																	 \
		GET_FCN(FCN_ProcedureHandleDelete, DECORATED_DLL_NAME(AimmsProcedureHandleDelete,4), int, (int) ) 								 \
		return fcn(handle);																												 \
	}																																	 \
																																		 \
	virtual int ProcedureArgumentHandleCreate( int prochandle, int argnumber, int *arghandle )											 \
	{ 																																	 \
		GET_FCN(FCN_ProcedureArgumentHandleCreate, DECORATED_DLL_NAME(AimmsProcedureArgumentHandleCreate,12), int, (int, int, int*) ) 	 \
		return fcn(prochandle, argnumber, arghandle);																					 \
	}																																	 \
																																		 \
	virtual int ProcedureAsyncRunDelete( int request )																					 \
	{ 																																	 \
		GET_FCN(FCN_ProcedureAsyncRunDelete, DECORATED_DLL_NAME(AimmsProcedureAsyncRunDelete,4), int, (int) ) 							 \
		return fcn(request);																											 \
	}																																	 \
																																		 \
	virtual int ProcedureAsyncRunStatus( int request, int *status, int *result )														 \
	{ 																																	 \
		GET_FCN(FCN_ProcedureAsyncRunStatus, DECORATED_DLL_NAME(AimmsProcedureAsyncRunStatus,12), int, (int, int*, int*) ) 				 \
		return fcn(request,status,result);																								 \
	}																																	 \
																																		 \
	virtual int APIStatus( int *status )																								 \
	{ 																																	 \
		GET_FCN(FCN_APIStatus, DECORATED_DLL_NAME(AimmsAPIStatus,4), int, (int*) ) 														 \
		return fcn(status);																												 \
	}																																	 \
																																		 \
	virtual int ProjectClose( int handle, int interactive )																				 \
	{ 																																	 \
		GET_FCN(FCN_ProjectClose, DECORATED_DLL_NAME(AimmsProjectClose,8), int, (int, int) ) 											 \
		return fcn(handle, interactive);																								 \
	}																																	 \
																																		 \
	virtual int ProjectWindow( void *window )																							 \
	{ 																																	 \
		GET_FCN(FCN_ProjectWindow, DECORATED_DLL_NAME(AimmsProjectWindow,4), int, (void*) ) 											 \
		return fcn(window);																												 \
	}																																	 \
																																		 \
	virtual int ProjectInterface( void **ifc )																							 \
	{ 																																	 \
		GET_FCN(FCN_ProjectInterface, DECORATED_DLL_NAME(AimmsProjectInterface,4), int, (void**) ) 										 \
		return fcn(ifc);																												 \
	}																																	 \
																																		 \
																																		 \
	virtual int ControlGet( int timeout )																								 \
	{ 																																	 \
		GET_FCN(FCN_ControlGet, DECORATED_DLL_NAME(AimmsControlGet,4), int, (int) ) 													 \
		return fcn(timeout);																											 \
	}																																	 \
																																		 \
	virtual int ControlRelease()																										 \
	{ 																																	 \
		GET_FCN(FCN_ControlRelease, DECORATED_DLL_NAME(AimmsControlRelease,0), int, () ) 												 \
		return fcn();																													 \
	}																																	 \
																																		 \
	virtual int InterruptCallbackInstall( AimmsInterruptCallback cbFcn )																 \
	{																																	 \
		GET_FCN(FCN_InterruptCallbackInstall, DECORATED_DLL_NAME(AimmsInterruptCallbackInstall,4), int, (AimmsInterruptCallback) ) 		 \
		return fcn(cbFcn);																												 \
	}																																	 \
																																		 \
	virtual int InterruptPending( void )																								 \
	{ 																																	 \
		GET_FCN(FCN_InterruptPending, DECORATED_DLL_NAME(AimmsInterruptPending,0), int, () ) 											 \
		return fcn();																													 \
	}																																	 \
																																		 \
	virtual int ThreadAttach( void )																									 \
	{ 																																	 \
		GET_FCN(FCN_ThreadAttach, DECORATED_DLL_NAME(AimmsThreadAttach,0), int, () ) 													 \
		return fcn();																													 \
	}																																	 \
																																		 \
	virtual int ThreadDetach( void )																									 \
	{ 																																	 \
		GET_FCN(FCN_ThreadDetach, DECORATED_DLL_NAME(AimmsThreadDetach,0), int, () ) 													 \
		return fcn();																													 \
	}																																	 \
																																		 \
	virtual int ErrorStatus()																											 \
	{ 																																	 \
		GET_FCN(FCN_ErrorStatus, DECORATED_DLL_NAME(AimmsErrorStatus,0), int, () ) 														 \
		return fcn();																													 \
	}																																	 \
																																		 \
	virtual int ErrorCount()																											 \
	{ 																																	 \
		GET_FCN(FCN_ErrorCount, DECORATED_DLL_NAME(AimmsErrorCount,0), int, () ) 														 \
		return fcn();																													 \
	}																																	 \
																																		 \
	virtual int ErrorSeverity(int errNo)																								 \
	{																																	 \
		GET_FCN(FCN_ErrorSeverity, DECORATED_DLL_NAME(AimmsErrorSeverity,4), int, (int) ) 												 \
		return fcn(errNo);																												 \
	}																																	 \
																																		 \
	virtual int ErrorNumberOfLocations(int errNo)																						 \
	{																																	 \
		GET_FCN(FCN_ErrorNumberOfLocations, DECORATED_DLL_NAME(AimmsErrorNumberOfLocations,4), int, (int) ) 							 \
		return fcn(errNo);																												 \
	}																																	 \
																																		 \
	virtual int ErrorLine(int errNo, int pos)																							 \
	{																																	 \
		GET_FCN(FCN_ErrorLine, DECORATED_DLL_NAME(AimmsErrorLine,8), int, (int,int) ) 													 \
		return fcn(errNo,pos);																											 \
	}																																	 \
																																		 \
	virtual int ErrorColumn(int errNo)																									 \
	{																																	 \
		GET_FCN(FCN_ErrorColumn, DECORATED_DLL_NAME(AimmsErrorColumn,4), int, (int) ) 													 \
		return fcn(errNo);																												 \
	}																																	 \
																																		 \
	virtual int ErrorDelete(int errNo)																									 \
	{																																	 \
		GET_FCN(FCN_ErrorDelete, DECORATED_DLL_NAME(AimmsErrorDelete,4), int, (int) ) 													 \
		return fcn(errNo);																												 \
	}																																	 \
																																		 \
	virtual int ErrorClear(void)																										 \
	{																																	 \
		GET_FCN(FCN_ErrorClear, DECORATED_DLL_NAME(AimmsErrorClear,0), int, () ) 														 \
		return fcn();																													 \
	}																																	 \
	virtual bool AddVirtualDataManagerFileSystem(void *ptr, bool deleteOnExit) 											  \
	{																													  \
		GET_FCN(FCN_AddVirtualDataManagerFileSystem, DECORATED_DLL_NAME(AddVirtualDataFileSystem,8), bool, (void*, bool) )  \
		return fcn(ptr,deleteOnExit);																					  \
	}																													  \
	virtual bool RemoveVirtualDataManagerFileSystem(void *ptr) 															  \
	{																													  \
		GET_FCN(FCN_RemoveVirtualDataManagerFileSystem, DECORATED_DLL_NAME(RemoveVirtualDataFileSystem,4), bool, (void*) )  \
		return fcn(ptr);																								  \
	}                                                                                                                      \
	virtual bool NewCasesUse64Bit() 															                           \
	{																													   \
		GET_FCN(FCN_NewCasesUse64Bit, DECORATED_DLL_NAME(NewCasesUse64Bit,0), bool, () )                                   \
		return fcn();																								       \
	}																														\
	virtual class ::EventBus *AimmsGetEventBus()																				\
	{																														\
		GET_FCN(FCN_AimmsGetEventBus, DECORATED_DLL_NAME(AimmsGetEventBus,0), class ::EventBus *, () )						\
		return fcn();																										\
	}

// This assumes that DECORATED_DLL_NAME_T is set to either DECORATED_DLL_NAME_W, DECORATED_DLL_NAME_A, or DECORATED_DLL_NAME
#define DECLARE_ALL_VALUE_N_METHODS \
	virtual int AttributeDefaultN( int handle, Value *value ) 															\
	{ 																													\
		GET_FCN(FCN_AttributeDefault, DECORATED_DLL_NAME_T(AimmsAttributeDefault,8), int, (int, Value*) ) 				\
		return fcn(handle, value);																						\
	}																													\
																														\
	virtual int ValueSearchN( int handle, int *tuple, Value *value )													\
	{ 																													\
		GET_FCN(FCN_ValueSearch, DECORATED_DLL_NAME_T(AimmsValueSearch,12), int, (int, int*,Value*) ) 					\
		return fcn(handle, tuple, value);																				\
	}																													\
																														\
	virtual int ValueNextN( int handle, int *tuple, Value *value )														\
	{ 																													\
		GET_FCN(FCN_ValueNext, DECORATED_DLL_NAME_T(AimmsValueNext,12), int, (int, int*, Value*) ) 					\
		return fcn(handle, tuple, value);																				\
	}																													\
																														\
	virtual int ValueNextMultiN( int handle, int *n, int *tuple, Value *value )											\
	{ 																													\
		GET_FCN(FCN_ValueNextMulti, DECORATED_DLL_NAME_T(AimmsValueNextMulti,16), int, (int, int*, int*, Value*) )		\
		return fcn(handle, n, tuple, value);																			\
	}																													\
																														\
	virtual int ValueRetrieveN( int handle, int *tuple, Value *value )													\
	{ 																													\
		GET_FCN(FCN_ValueRetrieve, DECORATED_DLL_NAME_T(AimmsValueRetrieve,12), int, (int, int*, Value*) ) 			\
		return fcn(handle, tuple, value);																				\
	}																													\
																														\
	virtual int ValueAssignN( int handle, int *tuple, Value *value )													\
	{ 																													\
		GET_FCN(FCN_ValueAssign, DECORATED_DLL_NAME_T(AimmsValueAssign,12), int, (int, int*, Value*) ) 				\
		return fcn(handle, tuple, value);																				\
	}																													\
																														\
	virtual int ValueAssignMultiN( int handle, int n, int *tuple, Value *value )										\
	{ 																													\
		GET_FCN(FCN_ValueAssignMulti, DECORATED_DLL_NAME_T(AimmsValueAssignMulti,16), int, (int, int, int*, Value*) )  \
		return fcn(handle, n, tuple, value);																			\
	}
	


#define DECLARE_ALL_STRING_METHODS \
	virtual int AttributeName( int handle, String *name ) 															   \
	{																												   \
		GET_FCN(FCN_AttributeName, DECORATED_DLL_NAME_T(AimmsAttributeName,8), int, (int, DllStringType*) ) 		   \
		return fcn(handle, outAS(name));																			   \
	}																												   \
																													   \
	virtual int AttributeDefaultS( int handle, Value *value ) 														   \
	{ 																												   \
		GET_FCN(FCN_AttributeDefault, DECORATED_DLL_NAME_T(AimmsAttributeDefault,8), int, (int, DllValueType*) )       \
		return fcn(handle, outV(value));																			   \
	}																																									   \
	virtual int AttributeSetUnit( int handle, const CharType *unit, const CharType *convention )																					   \
	{ 																																									   \
		GET_FCN(FCN_AttributeSetUnit, DECORATED_DLL_NAME_T(AimmsAttributeSetUnit,12), int, (int, const DllCharType*, const DllCharType*) ) 								   \
		return fcn(handle, inCS(unit), inCS(convention));																												   \
	}																																									   \
																																										   \
	virtual int AttributeGetUnit( int handle, String *unitName )																										   \
	{ 																																									   \
		GET_FCN(FCN_AttributeGetUnit, DECORATED_DLL_NAME_T(AimmsAttributeGetUnit,8), int, (int, DllStringType*) ) 														   \
		return fcn(handle, outAS(unitName));																															   \
	}																																									   \
																																										   \
	virtual int IdentifierHandleCreate( const CharType *name, int *domain, int *slicing, int flags, int *handle )																   \
	{ 																																									   \
		GET_FCN(FCN_IdentifierHandleCreate, DECORATED_DLL_NAME_T(AimmsIdentifierHandleCreate,20), int, (const DllCharType*, int*, int*, int, int*) ) 					   \
		return fcn(inCS(name),domain,slicing,flags,handle);																												   \
	}																																									   \
																																										   \
	virtual int IdentifierHandleCreatePermuted( const CharType *name, int *domain, int *slicing, int *permutation, int flags, int *handle )									   \
	{ 																																									   \
		GET_FCN(FCN_IdentifierHandleCreatePermuted, DECORATED_DLL_NAME_T(AimmsIdentifierHandleCreatePermuted,24), int, (const DllCharType*, int*, int*, int*, int, int*) ) \
		return fcn(inCS(name),domain,slicing,permutation,flags,handle);																									   \
	}																																									   \
																																										   \
	virtual int IdentifierSetUnit( int handle, const CharType *unitname, const CharType *convention ) 																				   \
	{																																									   \
		GET_FCN(FCN_IdentifierSetUnit, DECORATED_DLL_NAME_T(AimmsIdentifierSetUnit,12), int, (int, const DllCharType*, const DllCharType*) ) 							   \
		return fcn(handle, inCS(unitname), inCS(convention));																											   \
	}																																									   \
																																										   \
	virtual int IdentifierGetUnit( int handle, String *unitname ) 																										   \
	{																																									   \
		GET_FCN(FCN_IdentifierGetUnit, DECORATED_DLL_NAME_T(AimmsIdentifierGetUnit,8), int, (int, DllStringType*) ) 													   \
		return fcn(handle, outAS(unitname));																															   \
	}																																									   \
																																										   \
	virtual int ValueSearchS( int handle, int *tuple, Value *value )																									   \
	{ 																																									   \
		GET_FCN(FCN_ValueSearch, DECORATED_DLL_NAME_T(AimmsValueSearch,12), int, (int, int*,DllValueType*) ) 															   \
		return fcn(handle, tuple, outV(value));																															   \
	}																																									   \
																																										   \
	virtual int ValueNextS( int handle, int *tuple, Value *value )																										   \
	{ 																																									   \
		GET_FCN(FCN_ValueNext, DECORATED_DLL_NAME_T(AimmsValueNext,12), int, (int, int*, DllValueType*) ) 																   \
		return fcn(handle, tuple, outV(value));																															   \
	}																																									   \
																																										   \
	virtual int ValueNextMultiS( int handle, int *n, int *tuple, Value *value )																							   \
	{ 																																									   \
		GET_FCN(FCN_ValueNextMulti, DECORATED_DLL_NAME_T(AimmsValueNextMulti,16), int, (int, int*, int*, DllValueType*) )												   \
		return fcn(handle, n, tuple, outVA(*n,value));																													   \
	}																																									   \
																																										   \
	virtual int ValueRetrieveS( int handle, int *tuple, Value *value )																									   \
	{ 																																									   \
		GET_FCN(FCN_ValueRetrieve, DECORATED_DLL_NAME_T(AimmsValueRetrieve,12), int, (int, int*, DllValueType*) ) 														   \
		return fcn(handle, tuple, outV(value));																															   \
	}																																									   \
																																										   \
	virtual int ValueAssignS( int handle, int *tuple, Value *value )																									   \
	{ 																																									   \
		GET_FCN(FCN_ValueAssign, DECORATED_DLL_NAME_T(AimmsValueAssign,12), int, (int, int*, const DllValueType*) ) 													   \
		return fcn(handle, tuple, inV(value));																															   \
	}																																									   \
																																										   \
	virtual int ValueAssignMultiS( int handle, int n, int *tuple, Value *value )																						   \
	{ 																																									   \
		GET_FCN(FCN_ValueAssignMulti, DECORATED_DLL_NAME_T(AimmsValueAssignMulti,16), int, (int, int, int*, const DllValueType*) )										   \
		return fcn(handle, n, tuple, inVA(n,value));																													   \
	}																																									   \
																																										   \
	virtual int SetElementNumber( int handle, const CharType *name, int allowCreate, int *elementNumber, int *isCreated )														   \
	{ 																																									   \
		GET_FCN(FCN_SetElementNumber, DECORATED_DLL_NAME_T(AimmsSetElementNumber,20), int, (int, const DllCharType*, int, int*, int*) ) 								   \
		return fcn(handle, inCS(name), allowCreate, elementNumber, isCreated);																							   \
	}																																									   \
																																										   \
	virtual int SetAddElement( int handle, const CharType *name, int *element )																								   \
	{ 																																									   \
		GET_FCN(FCN_SetAddElement, DECORATED_DLL_NAME_T(AimmsSetAddElement,12), int, (int, const DllCharType*, int*) ) 													   \
		return fcn(handle, inCS(name), element);																														   \
	}																																									   \
																																										   \
	virtual int SetAddElementRecursive( int handle, const CharType *name, int *element )																						   \
	{ 																																									   \
		GET_FCN(FCN_SetAddElementRecursive, DECORATED_DLL_NAME_T(AimmsSetAddElementRecursive,12), int, (int, const DllCharType*, int*) ) 								   \
		return fcn(handle, inCS(name), element);																														   \
	}																																									   \
																																										   \
	virtual int SetRenameElement( int handle, int element, const CharType *name )																								   \
	{ 																																									   \
		GET_FCN(FCN_SetRenameElement, DECORATED_DLL_NAME_T(AimmsSetRenameElement,12), int, (int, int, const DllCharType*) ) 											   \
		return fcn(handle, element, inCS(name));																														   \
	}																																									   \
																																										   \
	virtual int SetElementToName( int handle, int element, String *name )																								   \
	{ 																																									   \
		GET_FCN(FCN_SetElementToName, DECORATED_DLL_NAME_T(AimmsSetElementToName,12), int, (int, int, DllStringType*) ) 												   \
		return fcn(handle, element, outAS(name));																														   \
	}																																									   \
																																										   \
	virtual int SetOrdinalToName( int handle, int ordinal, String *name )																								   \
	{ 																																									   \
		GET_FCN(FCN_SetOrdinalToName, DECORATED_DLL_NAME_T(AimmsSetOrdinalToName,12), int, (int, int, DllStringType*) ) 												   \
		return fcn(handle, ordinal, outAS(name));																														   \
	}																																									   \
																																										   \
	virtual int SetNameToElement( int handle, const CharType *name, int *element )																							   \
	{ 																																									   \
		GET_FCN(FCN_SetNameToElement, DECORATED_DLL_NAME_T(AimmsSetNameToElement,12), int, (int, const DllCharType*, int*) ) 											   \
		return fcn(handle, inCS(name), element);																														   \
	}																																									   \
																																										   \
	virtual int SetNameToOrdinal( int handle, const CharType *name, int *ordinal )																							   \
	{ 																																									   \
		GET_FCN(FCN_SetNameToOrdinal, DECORATED_DLL_NAME_T(AimmsSetNameToOrdinal,12), int, (int, const DllCharType*, int*) ) 											   \
		return fcn(handle, inCS(name), ordinal);																														   \
	}																																									   \
																																										   \
	virtual int ProcedureHandleCreate( const CharType *procedure, int *handle, int *nargs, int *argtype )																		   \
	{ 																																									   \
		GET_FCN(FCN_ProcedureHandleCreate, DECORATED_DLL_NAME_T(AimmsProcedureHandleCreate,16), int, (const DllCharType*, int*, int*, int*) ) 							   \
		return fcn(inCS(procedure),handle,nargs,argtype);																												   \
	}																																									   \
																																										   \
	virtual int ProcedureRun( int handle, int numArgs, int *argtype, Value *arglist, int *result )	  																       \
	{																																									   \
	    for (int i=0; i<numArgs;i++) {																																	   \
			if (argtype[i] == Storage::String) throw std::runtime_error("ProcedureRun does not support Storage::String arguments. Please use a Storage::Handle approach.");    \
		}																																								   \
		GET_FCN(FCN_ProcedureRun, DECORATED_DLL_NAME_T(AimmsProcedureRun,16), int, (int, int*, DllValueType*, int*) ) 													   \
		return fcn(handle,argtype,reinterpret_cast<DllValueType*>(arglist),result);																						   \
	}																																									   \
																																										   \
	virtual int ProcedureAsyncRunCreate( int handle, int numArgs, int *argtype, Value *arglist, int *request )																		   \
	{ 																																									   \
		GET_FCN(FCN_ProcedureAsyncRunCreate, DECORATED_DLL_NAME_T(AimmsProcedureAsyncRunCreate,16), int, (int, int*, DllValueType*, int*) ) 							   \
		return fcn(handle,argtype,reinterpret_cast<DllValueType*>(arglist),request);																					   \
	}																																									   \
																																										   \
	virtual int APIPassMessage( int severity, const CharType *message )																										   \
	{ 																																									   \
		GET_FCN(FCN_APIPassMessage, DECORATED_DLL_NAME_T(AimmsAPIPassMessage,8), int, (int, const DllCharType*) ) 														   \
		return fcn(severity,inCS(message));																																   \
	}																																									   \
																																										   \
	virtual int ProjectOpen( const CharType *commandline, int *handle ) 																										   \
	{																																									   \
		GET_FCN(FCN_ProjectOpen, DECORATED_DLL_NAME_T(AimmsProjectOpen,8), int, (const DllCharType *, int*) ) 															   \
		return fcn(inCS(commandline), handle);																															   \
	}																																									   \
																																										   \
	virtual int ServerProjectOpen( const CharType *commandline, int *handle )																									   \
	{ 																																									   \
		GET_FCN(FCN_ServerProjectOpen, DECORATED_DLL_NAME_T(AimmsServerProjectOpen,8), int, (const DllCharType*, int*) ) 												   \
		return fcn(inCS(commandline),handle);																															   \
	}																																									   \
																																										   \
	virtual int ProjectSwitch( const CharType *prjName, int interactive ) 																									   \
	{ 																																									   \
		GET_FCN(FCN_ProjectSwitch, DECORATED_DLL_NAME_T(AimmsProjectSwitch,8), int, (const DllCharType*, int) ) 														   \
		return fcn(inCS(prjName),interactive);																															   \
	}																																									   \
																																										   \
	virtual const CharType * ErrorMessage(int errNo)																													   \
	{ 																																									   \
		GET_FCN(FCN_ErrorMessage, DECORATED_DLL_NAME_T(AimmsErrorMessage,4), const DllCharType*, (int) ) 																   \
		return CallFunctionAndReturnString1Arg(fcn,errNo);																												   \
	}																																									   \
																																										   \
	virtual const CharType * ErrorCode(int errNo)																														   \
	{ 																																									   \
		GET_FCN(FCN_ErrorCode, DECORATED_DLL_NAME_T(AimmsErrorCode,4), const DllCharType*, (int) ) 																		   \
		return CallFunctionAndReturnString1Arg(fcn,errNo);																												   \
	}																																									   \
																																										   \
	virtual const CharType * ErrorCategory(int errNo)																													   \
	{ 																																									   \
		GET_FCN(FCN_ErrorCategory, DECORATED_DLL_NAME_T(AimmsErrorCategory,4), const DllCharType*, (int) ) 																   \
		return CallFunctionAndReturnString1Arg(fcn,errNo);																												   \
	}																																									   \
																																										   \
	virtual const CharType * ErrorFilename(int errNo)																													   \
	{ 																																									   \
		GET_FCN(FCN_ErrorFilename, DECORATED_DLL_NAME_T(AimmsErrorFilename,4), const DllCharType*, (int) ) 																   \
		return CallFunctionAndReturnString1Arg(fcn,errNo);																												   \
	}																																									   \
																																										   \
	virtual const CharType * ErrorNode(int errNo, int pos)																												   \
	{ 																																									   \
		GET_FCN(FCN_ErrorNode, DECORATED_DLL_NAME_T(AimmsErrorNode,8), const DllCharType*, (int,int) ) 																	   \
		return CallFunctionAndReturnString2Arg(fcn,errNo,pos);																											   \
	}																																									   \
																																										   \
	virtual const CharType * ErrorAttributeName(int errNo, int pos)																										   \
	{ 																																									   \
		GET_FCN(FCN_ErrorAttributeName, DECORATED_DLL_NAME_T(AimmsErrorAttributeName,8), const DllCharType*, (int,int) ) 												   \
		return CallFunctionAndReturnString2Arg(fcn,errNo,pos);																											   \
	}																																									   \
																																										   \
	virtual int ErrorRaise(int severity, const CharType *message, const CharType *code)																					   \
	{																																									   \
		GET_FCN(FCN_ErrorRaise, DECORATED_DLL_NAME_T(AimmsErrorRaise,12), int, (int, const DllCharType*, const DllCharType*) ) 											   \
		return fcn(severity,inCS(message),inCS(code));																													   \
	}
