// ieProxy.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <Wininet.h>
#include <tchar.h>

//#pragma comment(lib, "Wininet.lib")


BOOL SetConnectionOptions(BOOL ProxyEnable)
{
	INTERNET_PER_CONN_OPTION_LIST list;
	BOOL    bReturn;
	DWORD   dwBufSize = sizeof(list);

	// Fill the list structure.
	list.dwSize = sizeof(list);

	// NULL == LAN, otherwise connectoid name.
	list.pszConnection = NULL;

	// Set three options.
	list.dwOptionCount = 3;
	list.pOptions = new INTERNET_PER_CONN_OPTION[3];

	// Ensure that the memory was allocated.
	if (NULL == list.pOptions)
	{
		// Return FALSE if the memory wasn't allocated.
		return FALSE;
	}

	// Set flags.
	list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
	if (ProxyEnable) {
		list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT | PROXY_TYPE_PROXY;
	}	

	// Set proxy name.
	list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
	list.pOptions[1].Value.pszValue = TEXT("127.0.0.1:8118");

	// Set proxy override.
	list.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	list.pOptions[2].Value.pszValue = TEXT("local");

	// Set the options on the connection.
	bReturn = InternetSetOption(NULL,
		INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);

	// Free the allocated memory.
	delete[] list.pOptions;

	return bReturn;
}

BOOL QueryConnectionOptions()
{
	BOOL bReturn;
	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[1];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

	Option[0].dwOption = INTERNET_PER_CONN_FLAGS;

	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = NULL;
	List.dwOptionCount = 1;
	List.dwOptionError = 0;
	List.pOptions = Option;

	InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize);
	if ((Option[0].Value.dwValue &  PROXY_TYPE_PROXY) == PROXY_TYPE_PROXY)
	{
		return TRUE;
	}
	return FALSE;
}

int main(int argc, _TCHAR* argv[])
{
	BOOL ProxyEnable;

	ProxyEnable = QueryConnectionOptions();
	SetConnectionOptions(!ProxyEnable);
	if (!ProxyEnable) {
		MessageBox(NULL, TEXT("ENABLED"), TEXT("ie Proxy"), NULL);
	}
	else {
		MessageBox(NULL, TEXT("DISABLED"), TEXT("ie Proxy"), NULL);
	}
	return 0;
}

