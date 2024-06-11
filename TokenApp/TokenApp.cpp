#include <stdio.h>
#include <windows.h>
#include <sddl.h>

int main()
{
	HANDLE hprocess = NULL;
	HANDLE hToken = NULL;
	BOOL bResult = FALSE;
	DWORD dwError = 0;
	DWORD dwSessionId = 0;
	DWORD dwSize = 0;
	PTOKEN_OWNER pTokenOwner = NULL;
	CHAR* strSid = NULL;

	// 1. Retrieve a handle to the process
	hprocess = GetCurrentProcess();

	// 2. Retrieve a handle to the process token (OpenProcessToken)
	bResult = OpenProcessToken(hprocess, TOKEN_READ, &hToken);
	if (!bResult) {
		printf("Error in OpenProcessToken\n");
		dwError = GetLastError();
		printf("Error: 0x%lx\n", dwError);
		goto exit;
	}

	// 3. Retrieve the session number from the token (GetTokenInformation)
	bResult = GetTokenInformation(hToken, TokenSessionId, &dwSessionId, sizeof(DWORD), &dwSize);
	if (!bResult) {
		printf("GetTokenInformation\n");
		dwError = GetLastError();
		printf("Error: 0x%lx\n", dwError);
		goto exit;
	}
	printf("SessionID : %d\n", dwSessionId);

	// 4. Retrieve the owner (by calling GetTokenInformation twice)
	bResult = GetTokenInformation(hToken, TokenOwner, NULL, 0, &dwSize);
	if (!bResult && GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
		printf("Error in GetTokenInformation TakeOwner 1st\n");
		goto exit;
	}

	printf("Size: %d\n", dwSize);
	pTokenOwner = (PTOKEN_OWNER)malloc(dwSize);

	bResult = GetTokenInformation(hToken, TokenOwner, pTokenOwner, dwSize, &dwSize);
	if (!bResult) {
		printf("Error in GetTokenInformation TakeOwner 2nd\n");
		dwError = GetLastError();
		printf("Error: 0x%lx\n", dwError);
		goto exit;
	}

	// Display the SID
	ConvertSidToStringSidA(pTokenOwner->Owner, &strSid);
	printf("%s\n", strSid);

exit:
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
