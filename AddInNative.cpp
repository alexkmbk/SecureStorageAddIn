#if !defined( __linux__ ) && !defined(__APPLE__) && !defined(__ANDROID__)
#include "stdafx.h"
#endif

#include <stdio.h>
#include <wchar.h>
#include "AddInNative.h"

static const std::u16string sClassName(u"SecureStorage");
static const std::u16string sVersion(u"1.0");

static const std::array<std::u16string, CAddInNative::eMethLast> osMethods = { u"getvalue"};
static const std::array<std::u16string, CAddInNative::eMethLast> osMethods_ru = { u"получитьзначение"};
static const std::array<std::u16string, CAddInNative::ePropLast> osProps = {};
static const std::array<std::u16string, CAddInNative::ePropLast> osProps_ru = {};

AppCapabilities g_capabilities = eAppCapabilitiesInvalid;


//---------------------------------------------------------------------------//
long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface)
{
	if (!*pInterface)
	{
		*pInterface = new CAddInNative;
		return (long)*pInterface;
	}
	return 0;
}
//---------------------------------------------------------------------------//
AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities)
{
	g_capabilities = capabilities;
	return eAppCapabilitiesLast;
}
//---------------------------------------------------------------------------//
long DestroyObject(IComponentBase** pIntf)
{
	if (!*pIntf)
		return -1;

	delete *pIntf;
	*pIntf = 0;
	return 0;
}
//---------------------------------------------------------------------------//
const WCHAR_T* GetClassNames()
{
	return (WCHAR_T*)sClassName.c_str();
}
//---------------------------------------------------------------------------//

// CAddInNative
//---------------------------------------------------------------------------//
CAddInNative::CAddInNative()
{
	m_iMemory = 0;
	m_iConnect = 0;
}
//---------------------------------------------------------------------------//
CAddInNative::~CAddInNative()
{
}
//---------------------------------------------------------------------------//
bool CAddInNative::Init(void* pConnection)
{
	m_iConnect = (IAddInDefBase*)pConnection;
	return m_iConnect != NULL;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetInfo()
{
	// Component should put supported component technology version 
	// This component supports 2.1 version
	return 2100;
}
//---------------------------------------------------------------------------//
void CAddInNative::Done()
{

}
/////////////////////////////////////////////////////////////////////////////
// ILanguageExtenderBase
//---------------------------------------------------------------------------//
bool CAddInNative::RegisterExtensionAs(WCHAR_T** wsExtensionName)
{
	if (wsExtensionName == nullptr) {
		return false;
	}
	const size_t size = (sClassName.size() + 1) * sizeof(char16_t);

	if (!m_iMemory || !m_iMemory->AllocMemory(reinterpret_cast<void**>(wsExtensionName), size) || *wsExtensionName == nullptr) {
		return false;
	};

	memcpy(*wsExtensionName, sClassName.c_str(), size);

	return true;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetNProps()
{
	return ePropLast;
}
//---------------------------------------------------------------------------//
long CAddInNative::FindProp(const WCHAR_T* wsPropName)
{
	return -1;
}

//---------------------------------------------------------------------------//
const WCHAR_T* CAddInNative::GetPropName(long lPropNum, long lPropAlias)
{

	return 0;
}
//---------------------------------------------------------------------------//
bool CAddInNative::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{
	return true;
}
//---------------------------------------------------------------------------//
bool CAddInNative::SetPropVal(const long lPropNum, tVariant *varPropVal)
{
	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::IsPropReadable(const long lPropNum)
{
	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::IsPropWritable(const long lPropNum)
{
	return false;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetNMethods()
{
	return eMethLast;
}
//---------------------------------------------------------------------------//
long CAddInNative::FindMethod(const WCHAR_T* wsMethodName)
{
	std::u16string usMethodName = (char16_t*)(wsMethodName);
	tolowerStr(usMethodName);

	auto it = std::find(osMethods.begin(), osMethods.end(), usMethodName);
	if (it != osMethods.end()) {
		return it - osMethods.begin();
	}

	it = std::find(osMethods_ru.begin(), osMethods_ru.end(), usMethodName);
	if (it != osMethods_ru.end()) {
		return it - osMethods_ru.begin();
	}

	return -1;
}
//---------------------------------------------------------------------------//
const WCHAR_T* CAddInNative::GetMethodName(const long lMethodNum, const long lMethodAlias)
{

	if (lMethodNum >= eMethLast)
		return nullptr;

	const std::basic_string<char16_t>* usCurrentName;

	switch (lMethodAlias)
	{
	case 0: // First language
	{
		usCurrentName = &osMethods[lMethodNum];
		break;
	}
	case 1: // Second language
	{
		usCurrentName = &osMethods_ru[lMethodNum];
		break;
	}
	default:
		return nullptr;
	}

	if (usCurrentName == nullptr)
		return nullptr;

	WCHAR_T* result = nullptr;

	const size_t bytes = (usCurrentName->length() + 1) * sizeof(char16_t);

	if (!m_iMemory || !m_iMemory->AllocMemory(reinterpret_cast<void**>(&result), bytes)) {
		return nullptr;
	};

	memcpy(result, usCurrentName->c_str(), bytes);

	return result;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetNParams(const long lMethodNum)
{
	switch (lMethodNum)
	{
	case eMethGetValaue:
		return 1;
	default:
		return 0;
	}

	return 0;
}
//---------------------------------------------------------------------------//
bool CAddInNative::GetParamDefValue(const long lMethodNum, const long lParamNum,
	tVariant *pvarParamDefValue)
{
	TV_VT(pvarParamDefValue) = VTYPE_EMPTY;
	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::HasRetVal(const long lMethodNum)
{
	switch (lMethodNum)
	{
	case eMethGetValaue:
		return true;
	default:
		return false;
	}

	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::CallAsProc(const long lMethodNum,
	tVariant* paParams, const long lSizeArray)
{
	return true;
}
//---------------------------------------------------------------------------//
bool CAddInNative::CallAsFunc(const long lMethodNum,
	tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray)
{
	switch (lMethodNum)
	{
	case eMethGetValaue:
	{
		if (paParams[0].vt != VTYPE_PWSTR)
		{
			return false;
		}
		TV_VT(pvarRetValue) = VTYPE_PWSTR;
		std::wstring valueName;
		valueName.assign(paParams[0].pwstrVal, paParams[0].wstrLen);

		std::wstring res;

		PCREDENTIALW pcred;
		if (CredReadW(valueName.c_str(), CRED_TYPE_GENERIC, 0, &pcred)) {
			res.assign((wchar_t*)pcred->CredentialBlob, pcred->CredentialBlobSize / sizeof(wchar_t));
			CredFree(pcred);
		}

		if (m_iMemory->AllocMemory((void**)&pvarRetValue->pwstrVal, (res.length() + 1) * sizeof(wchar_t)))
		{
			memcpy(pvarRetValue->pwstrVal, res.c_str(), (res.length() + 1) * sizeof(wchar_t));
			pvarRetValue->wstrLen = res.length();
		}
		return true;


	}
	default:
		return false;
	}
	return true;
}

void CAddInNative::SetLocale(const WCHAR_T* loc)
{
#if !defined( __linux__ ) && !defined(__APPLE__) && !defined(__ANDROID__)
	_wsetlocale(LC_ALL, L"");
#else
	setlocale(LC_ALL, "");
#endif
}

/////////////////////////////////////////////////////////////////////////////
// LocaleBase
//---------------------------------------------------------------------------//
bool CAddInNative::setMemManager(void* mem)
{
	m_iMemory = (IMemoryManager*)mem;
	return m_iMemory != 0;
}