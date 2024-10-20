#include<stdio.h>
#include<Windows.h>
#include<sddl.h>

int main() {
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	BOOL bResult = FALSE;
	DWORD dwError = 0;
	DWORD dwSessionId = 0;
	DWORD dwSize = 0;
	PTOKEN_OWNER pTokenOwner = NULL;
	PCHAR strSid = NULL;

	// 1. Get a handle for the process (GetCurrentProcess)
	hProcess = GetCurrentProcess();

	// 2. Get a handle for the process' token (OpenProcessToken)
	bResult = OpenProcessToken(hProcess, TOKEN_READ, &hToken);
	if (!bResult) {
		printf("Error in OpenProcessToken\n");
		// Similar to Linux's __errno
		dwError = GetLastError();
		printf("Error number: 0x%lx\n", dwError);
		goto EXIT;
	}

	// 3. Get information about the token (GetTokenInformation)
	// - Session ID

	bResult = GetTokenInformation(hToken, TokenSessionId, &dwSessionId, sizeof(DWORD), &dwSize);
	if (!bResult) {
		printf("Error in GetTokenInformation sessionID\n");
		dwError = GetLastError();
		printf("Error number: 0x%lx\n", dwError);
		goto EXIT;
	}

	printf("Session ID: %d\n", dwSessionId);

	// - Owner (TokenOwner)
	bResult = GetTokenInformation(hToken, TokenOwner, NULL, 0, &dwSize);
	if (bResult || GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
		printf("Error in GetTokenInformation Owner 1st");
		goto EXIT;
	}

	printf("Size: %d\n", dwSize);
	pTokenOwner = (PTOKEN_OWNER)malloc(dwSize);

	bResult = GetTokenInformation(hToken, TokenOwner, pTokenOwner, dwSize, &dwSize);
	if (!bResult) {
		printf("Error in GetTokenInformation TokenOwner 2nd\n");
		dwError = GetLastError();
		printf("Error number: 0x%lx\n", dwError);
		goto EXIT;
	}

	ConvertSidToStringSidA(pTokenOwner->Owner, &strSid);
	printf("SID: %s\n", strSid);


EXIT:
	if (hToken) {
		CloseHandle(hToken);
	}

	if (pTokenOwner) {
		free(pTokenOwner);
	}

	if (strSid) {
		LocalFree(strSid);
	}

	return 0;
}
