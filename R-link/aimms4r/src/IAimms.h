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
#pragma once
namespace Aimms{

typedef struct tagAimmsStringW {
  int       Length;
  wchar_t  *String;
} AimmsStringW;

typedef union tagAimmsValueW {
  double        Double;
  int           Int;
  struct {
    int         Length;
    wchar_t    *String;
  };
} AimmsValueW;

typedef union tagAimmsValue {
  double        Double;
  int           Int;
  struct {
    int         Length;
    char    *String;
  };
} AimmsValue;


enum AimmsAPICallStatus{
    API_CALL_UNAVAILABLE,
    CALL_PROVIDER_UNAVAILABLE,
    ALL
};

  enum AimmsAttributeTypes{
    NO_IDTYPE = 0,
    SIMPLE_SET_ROOT = 1,
    SIMPLE_SET_SUBSET = 2,
    COMPOUND_SET_ROOT = 11,
    COMPOUND_SET_SUBSET = 12,
    SET_RELATION = 20,
    SET_INDEXED = 21,
    PARAMETER_NUMERICS = 30,
    PARAMETER_ELEMENTS = 31,
    PARAMETER_STRINGS = 32,
    PARAMETER_UNITS = 33,
    VARIABLE = 40,
    VARIABLE_SUFFIX = 50,
    EQUATION_SUFFIX = 51,
    MODEL_SUFFIX = 52,
    FILE_TOKEN_SUFFIX = 53,
    INDEX = 60,
    ELEMENT_VARIABLE = 70
  };

  enum AimmsStorageTypes{
    STORAGE_HANDLE = 0,
    STORAGE_DOUBLE,
    STORAGE_INT,
    STORAGE_BINARY,
    STORAGE_STRING
  };

class IAimms{
public:
  enum AimmsAttributeTypes{
    NO_IDTYPE = 0,
    SIMPLE_SET_ROOT = 1,
    SIMPLE_SET_SUBSET = 2,
    COMPOUND_SET_ROOT = 11,
    COMPOUND_SET_SUBSET = 12,
    SET_RELATION = 20,
    SET_INDEXED = 21,
    PARAMETER_NUMERICS = 30,
    PARAMETER_ELEMENTS = 31,
    PARAMETER_STRINGS = 32,
    PARAMETER_UNITS = 33,
    VARIABLE = 40,
    VARIABLE_SUFFIX = 50,
    EQUATION_SUFFIX = 51,
    MODEL_SUFFIX = 52,
    FILE_TOKEN_SUFFIX = 53,
    INDEX = 60,
    ELEMENT_VARIABLE = 70
  };

  enum AimmsStorageTypes{
    STORAGE_HANDLE = 0,
    STORAGE_DOUBLE,
    STORAGE_INT,
    STORAGE_BINARY,
    STORAGE_STRING
  };
public:
  virtual ~IAimms(){}
  virtual bool Reload(std::wstring) = 0;
  virtual int ProjectOpen(wchar_t*, int*) = 0;
  virtual int ProjectClose(int, int) = 0;
  virtual int ProcedureHandleCreateA(char *, int *, int *, int *) = 0;
  virtual int ProcedureArgumentHandleCreate(int, int, int *) = 0;
  virtual int AttributeNameW(int, AimmsStringW *) = 0;
  virtual int ValueCard(int, int*) = 0;
  virtual int AttributeDimension(int, int *, int *) = 0;
  virtual int ValueResetHandle(int) = 0;
  virtual int ValueNextW(int, int *, AimmsValueW *) = 0;
  virtual int APILastError(int*, wchar_t*) = 0;
  virtual int IdentifierHandleCreateW(wchar_t *, int *, int *, int, int *) = 0;

  virtual int AttributeDeclarationDomain(int, int*) = 0;
  virtual int SetOrdinalToElement(int, int, int *) = 0;
  virtual int SetElementToNameW(int, int, Aimms::AimmsStringW *) = 0;

  virtual int AttributeRootDomain(int, int *) = 0;
  virtual int AttributeStorage(int, int *) = 0;
  virtual int AttributeType(int, int *) = 0;
  virtual int IdentifierHandleDelete(int) = 0;
  virtual int AttributeElementRange(int, int*) = 0;
  virtual int ValueRetrieveW(int, int*,  Aimms::AimmsValueW*) = 0;
  virtual int ValueRetrieve(int, int*,  Aimms::AimmsValue*) = 0;
  virtual int ValueNextMultiW (int, int*, int*,  Aimms::AimmsValueW*) = 0;
  virtual int ValueAssignW (int, int*,  Aimms::AimmsValueW*) = 0;
  virtual int ValueAssignA (int, int*,  Aimms::AimmsValue*) = 0;
  virtual int IdentifierEmpty (int ) = 0;


};
}
