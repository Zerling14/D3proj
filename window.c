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
	switch (uMsg){
	case WM_CREATE:
	{
		SetTimer( hWnd, 5, 100, NULL );
		return 0;
	}
	case WM_TIMER:
	{
		switch(wParam)
		{
		case 5:
			InvalidateRect(hWnd, NULL, 1);
			break;
		}
	}
	case WM_PAINT: // ���� ����� ����������, ��:
		hDC = BeginPaint(hWnd, &ps); // �������������� �������� ����������
		GetClientRect(hWnd, &rect); // �������� ������ � ������ ������� ��� ���������
		SetTextColor(hDC, colorText); // ������������� ���� ������������ ����������
		EnemyPosList *enemy_pos_list;
		get_enemy_pos_list(&enemy_pos_list);
		char buffer[10];
		DrawText(hDC, itoa(enemy_pos_list->size, buffer, 10), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); // ������ �����
		float x, y, z;
		get_player_cord(&x, &y, &z);
		for (int enemy_num = 0; enemy_num <= enemy_pos_list->size; enemy_num++) {
			if ((x - enemy_pos_list->list[enemy_num].x <= 250) && (x - enemy_pos_list->list[enemy_num].x >= -250)
				&& (y - enemy_pos_list->list[enemy_num].y <= 250) && (y - enemy_pos_list->list[enemy_num].y >= -250)) {
				SetPixel(hDC, 1448 + 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * cos(-150) - (y - enemy_pos_list->list[enemy_num].y) * sin(-150)), 172 - 0.6 * ((x - enemy_pos_list->list[enemy_num].x) * sin(-150) + (y - enemy_pos_list->list[enemy_num].y) * cos(-150)), RGB(255, 255, 0)); //
				
			}
		}
		free(enemy_pos_list->list);
		free(enemy_pos_list);
		//Rectangle(hDC, 1323, 47, 1577, 302);
		//541.3394165 - x 303.7431946 = 250
		//710.973938 - y 466.0963135 = 250
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); //�������� ������
		SelectObject(hDC, hPen); //������ �������� �������
		Line(hDC, 1323, 47, 1577, 47);
		Line(hDC, 1323, 47, 1323, 302);
		Line(hDC, 1577, 47, 1577, 302);
		Line(hDC, 1323, 302, 1577, 302);
		SetPixel(hDC, 1448, 172, RGB(255, 0, 0));
		SetPixel(hDC, 1448, 173, RGB(255, 0, 0));
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