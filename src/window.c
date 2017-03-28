#include <windows.h> // ������������ ����, ���������� WINAPI
#include <cmath>
extern "C" {
#include "D3Tool.h"
}
// �������� ������� ��������� ��������� � ���������������� ���������:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR mainMessage[] = "����� ��-�����!"; // ������ � ����������

// ����������� �������:
int WINAPI WinMain(HINSTANCE hInst, // ���������� ���������� ����������
	HINSTANCE hPrevInst, // �� ����������
	LPSTR lpCmdLine, // �� ����������
	int nCmdShow) // ����� ����������� ������
{
	TCHAR szClassName[] = "��� �����"; // ������ � ������ ������
	HWND hMainWnd; // ������ ���������� �������� ������
	MSG msg; // ����� ��������� ��������� MSG ��� ��������� ���������
	WNDCLASSEX wc; // ������ ���������, ��� ��������� � ������ ������ WNDCLASSEX
	wc.cbSize = sizeof(wc); // ������ ��������� (� ������)
	wc.style = CS_HREDRAW | CS_VREDRAW; // ����� ������ ������
	wc.lpfnWndProc = WndProc; // ��������� �� ���������������� �������
	wc.lpszMenuName = NULL; // ��������� �� ��� ���� (� ��� ��� ���)
	wc.lpszClassName = szClassName; // ��������� �� ��� ������
	wc.cbWndExtra = 0; // ����� ������������� ������ � ����� ���������
	wc.cbClsExtra = 0; // ����� ������������� ������ ��� �������� ���������� ����������
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // ��������� �����������
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // ���������� ��������� ����������� (� ����)
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ���������� �������
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // ���������� ����� ��� �������� ���� ����
	wc.hInstance = hInst; // ��������� �� ������, ���������� ��� ����, ������������ ��� ������
	if (!RegisterClassEx(&wc)){
		// � ������ ���������� ����������� ������:
		MessageBox(NULL, "�� ���������� ���������������� �����!", "������", MB_OK);
		return 0; // ����������, �������������, ������� �� WinMain
	}
	// �������, ��������� ������:
	hMainWnd = CreateWindow(
		szClassName, // ��� ������
		"����������� ������� ���������", // ��� ������ (�� ��� ������)
		WS_OVERLAPPEDWINDOW, // ������ ����������� ������
		0, // ������� ������ �� ��� �
		0, // ������� ������ �� ��� � (��� ������ � �, �� ������ �� �����)
		1600, // ������ ������
		800, // ������ ������ (��� ������ � ������, �� ������ �� �����)
		(HWND)NULL, // ���������� ������������� ����
		NULL, // ���������� ����
		HINSTANCE(hInst), // ���������� ���������� ����������
		NULL); // ������ �� ������� �� WndProc
	if (!hMainWnd){
		// � ������ ������������� �������� ������ (�������� ��������� � ��):
		MessageBox(NULL, "�� ���������� ������� ����!", "������", MB_OK);
		return 0;
	}
	LONG lStyle = GetWindowLong(hMainWnd, GWL_STYLE);
	lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
	SetWindowLong(hMainWnd, GWL_STYLE, lStyle);

	SetWindowLong(hMainWnd, GWL_EXSTYLE, GetWindowLong(hMainWnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOPMOST);
	SetLayeredWindowAttributes(hMainWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

	RECT rect;

	// get the current window size and position
	GetWindowRect(hMainWnd, &rect);

	// now change the size, position, and Z order 
	// of the window.
	SetWindowPos(hMainWnd, HWND_TOPMOST, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
	ShowWindow(hMainWnd, nCmdShow); // ���������� ������
	UpdateWindow(hMainWnd); // ��������� ������
	while (GetMessage(&msg, 0, 0, 0)) { // ��������� ��������� �� �������, ���������� ��-�����, ��
		TranslateMessage(&msg); // �������������� ���������
		DispatchMessage(&msg); // ������� ��������� ������� ��
	}
	return msg.wParam; // ���������� ��� ������ �� ����������
}

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); //������� �������� ���������� x1, y1
	return LineTo(hdc, x2, y2);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	HDC hDC; // ������ ���������� ���������� ������ �� ������
	PAINTSTRUCT ps; // ���������, ���-��� ���������� � ���������� ������� (�������, ���� � ��)
	RECT rect; // ���-��, ������������ ������ ���������� �������
	COLORREF colorText = RGB(255, 0, 0); // ����� ���� ������
	HPEN hPen;
	
	HDC hdcMem;
	HBITMAP hbmMem;
	HANDLE hOld;
	
	COLORREF color;
	switch (uMsg){
	case WM_CREATE:
	{
		SetTimer( hWnd, 5, 34, NULL );
		return 0;
	}
	case WM_TIMER:
	{
		switch(wParam)
		{
		case 5:
			InvalidateRect(hWnd, NULL, 0);
			break;
		}
	}
	case WM_PAINT: // ���� ����� ����������, ��:
		hDC = BeginPaint(hWnd, &ps); // �������������� �������� ����������
		GetClientRect(hWnd, &rect); // �������� ������ � ������ ������� ��� ���������
		hdcMem = CreateCompatibleDC(hDC);
		hbmMem = CreateCompatibleBitmap(hDC, rect.right - rect.left, rect.bottom - rect.top);
		hOld = SelectObject(hdcMem, hbmMem);
		SetTextColor(hdcMem, colorText); // ������������� ���� ������������ ����������
		EnemyPosList *enemy_pos_list;
		get_enemy_pos_list(&enemy_pos_list);
		char buffer[10];
		DrawText(hdcMem, itoa(enemy_pos_list->size, buffer, 10), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); // ������ �����
		float x, y, z;
		get_player_cord(&x, &y, &z);
		for (int enemy_num = 0; enemy_num <= enemy_pos_list->size; enemy_num++) {
			if ((x - enemy_pos_list->list[enemy_num].x <= 500) && (x - enemy_pos_list->list[enemy_num].x >= -500)
				&& (y - enemy_pos_list->list[enemy_num].y <= 500) && (y - enemy_pos_list->list[enemy_num].y >= -500)) {
				if ( enemy_pos_list->type_list[enemy_num] == 0) {
					color = RGB(100, 100, 100);
				} else if ( enemy_pos_list->type_list[enemy_num] == 1) {
					color = RGB(0, 150, 255);
				} else if ( enemy_pos_list->type_list[enemy_num] == 2) {
					color = RGB(255, 200, 0);
				} else if ( enemy_pos_list->type_list[enemy_num] == 3) {
					color = RGB(255, 100, 0);
				}
				SetPixel(hdcMem, 1449 + 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * cos(-150) - (y - enemy_pos_list->list[enemy_num].y) * sin(-150)), 173 - 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * sin(-150) + (y - enemy_pos_list->list[enemy_num].y) * cos(-150)), color); //
				SetPixel(hdcMem, 1449 + 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * cos(-150) - (y - enemy_pos_list->list[enemy_num].y) * sin(-150)), 172 - 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * sin(-150) + (y - enemy_pos_list->list[enemy_num].y) * cos(-150)), color); //
				SetPixel(hdcMem, 1449 + 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * cos(-150) - (y - enemy_pos_list->list[enemy_num].y) * sin(-150)), 171 - 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * sin(-150) + (y - enemy_pos_list->list[enemy_num].y) * cos(-150)), color); //
				SetPixel(hdcMem, 1448 + 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * cos(-150) - (y - enemy_pos_list->list[enemy_num].y) * sin(-150)), 173 - 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * sin(-150) + (y - enemy_pos_list->list[enemy_num].y) * cos(-150)), color); //
				SetPixel(hdcMem, 1448 + 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * cos(-150) - (y - enemy_pos_list->list[enemy_num].y) * sin(-150)), 172 - 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * sin(-150) + (y - enemy_pos_list->list[enemy_num].y) * cos(-150)), color); //
				SetPixel(hdcMem, 1448 + 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * cos(-150) - (y - enemy_pos_list->list[enemy_num].y) * sin(-150)), 171 - 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * sin(-150) + (y - enemy_pos_list->list[enemy_num].y) * cos(-150)), color); //
				SetPixel(hdcMem, 1447 + 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * cos(-150) - (y - enemy_pos_list->list[enemy_num].y) * sin(-150)), 173 - 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * sin(-150) + (y - enemy_pos_list->list[enemy_num].y) * cos(-150)), color); //
				SetPixel(hdcMem, 1447 + 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * cos(-150) - (y - enemy_pos_list->list[enemy_num].y) * sin(-150)), 172 - 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * sin(-150) + (y - enemy_pos_list->list[enemy_num].y) * cos(-150)), color); //
				SetPixel(hdcMem, 1447 + 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * cos(-150) - (y - enemy_pos_list->list[enemy_num].y) * sin(-150)), 171 - 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * sin(-150) + (y - enemy_pos_list->list[enemy_num].y) * cos(-150)), color); //
				
			}
		}
		free(enemy_pos_list->list);
		free(enemy_pos_list);
		//Rectangle(hDC, 1323, 47, 1577, 302);
		//541.3394165 - x 303.7431946 = 250
		//710.973938 - y 466.0963135 = 250
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); //�������� ������
		SelectObject(hdcMem, hPen); //������ �������� �������
		Line(hdcMem, 1323, 47, 1577, 47);
		Line(hdcMem, 1323, 47, 1323, 302);
		Line(hdcMem, 1577, 47, 1577, 302);
		Line(hdcMem, 1323, 302, 1577, 302);
		SetPixel(hdcMem, 1448, 172, RGB(255, 0, 0));
		SetPixel(hdcMem, 1448, 173, RGB(255, 0, 0));
		BitBlt(hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, SRCCOPY);
		
		SelectObject(hdcMem, hOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);
		EndPaint(hWnd, &ps); // ����������� ��������
		break;
	case WM_DESTROY: // ���� ������ ���������, ��:
		PostQuitMessage(0); // ���������� WinMain() ��������� WM_QUIT
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam); // ���� ������� ������
	}
	return 0; // ���������� ��������
}