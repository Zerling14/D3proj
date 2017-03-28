#include <windows.h> // заголовочный файл, содержащий WINAPI
#include <cmath>
extern "C" {
#include "D3Tool.h"
}
// Прототип функции обработки сообщений с пользовательским названием:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR mainMessage[] = "Какой то-текст!"; // строка с сообщением

// Управляющая функция:
int WINAPI WinMain(HINSTANCE hInst, // дескриптор экземпляра приложения
	HINSTANCE hPrevInst, // не используем
	LPSTR lpCmdLine, // не используем
	int nCmdShow) // режим отображения окошка
{
	TCHAR szClassName[] = "Мой класс"; // строка с именем класса
	HWND hMainWnd; // создаём дескриптор будущего окошка
	MSG msg; // создём экземпляр структуры MSG для обработки сообщений
	WNDCLASSEX wc; // создаём экземпляр, для обращения к членам класса WNDCLASSEX
	wc.cbSize = sizeof(wc); // размер структуры (в байтах)
	wc.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окошка
	wc.lpfnWndProc = WndProc; // указатель на пользовательскую функцию
	wc.lpszMenuName = NULL; // указатель на имя меню (у нас его нет)
	wc.lpszClassName = szClassName; // указатель на имя класса
	wc.cbWndExtra = 0; // число освобождаемых байтов в конце структуры
	wc.cbClsExtra = 0; // число освобождаемых байтов при создании экземпляра приложения
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // декриптор пиктограммы
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // дескриптор маленькой пиктограммы (в трэе)
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // дескриптор курсора
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // дескриптор кисти для закраски фона окна
	wc.hInstance = hInst; // указатель на строку, содержащую имя меню, применяемого для класса
	if (!RegisterClassEx(&wc)){
		// в случае отсутствия регистрации класса:
		MessageBox(NULL, "Не получилось зарегистрировать класс!", "Ошибка", MB_OK);
		return 0; // возвращаем, следовательно, выходим из WinMain
	}
	// Функция, создающая окошко:
	hMainWnd = CreateWindow(
		szClassName, // имя класса
		"Полноценная оконная процедура", // имя окошка (то что сверху)
		WS_OVERLAPPEDWINDOW, // режимы отображения окошка
		0, // позиция окошка по оси х
		0, // позиция окошка по оси у (раз дефолт в х, то писать не нужно)
		1600, // ширина окошка
		800, // высота окошка (раз дефолт в ширине, то писать не нужно)
		(HWND)NULL, // дескриптор родительского окна
		NULL, // дескриптор меню
		HINSTANCE(hInst), // дескриптор экземпляра приложения
		NULL); // ничего не передаём из WndProc
	if (!hMainWnd){
		// в случае некорректного создания окошка (неверные параметры и тп):
		MessageBox(NULL, "Не получилось создать окно!", "Ошибка", MB_OK);
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
	ShowWindow(hMainWnd, nCmdShow); // отображаем окошко
	UpdateWindow(hMainWnd); // обновляем окошко
	while (GetMessage(&msg, 0, 0, 0)) { // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
		TranslateMessage(&msg); // интерпретируем сообщения
		DispatchMessage(&msg); // передаём сообщения обратно ОС
	}
	return msg.wParam; // возвращаем код выхода из приложения
}

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	HDC hDC; // создаём дескриптор ориентации текста на экране
	PAINTSTRUCT ps; // структура, сод-щая информацию о клиентской области (размеры, цвет и тп)
	RECT rect; // стр-ра, определяющая размер клиентской области
	COLORREF colorText = RGB(255, 0, 0); // задаём цвет текста
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
	case WM_PAINT: // если нужно нарисовать, то:
		hDC = BeginPaint(hWnd, &ps); // инициализируем контекст устройства
		GetClientRect(hWnd, &rect); // получаем ширину и высоту области для рисования
		hdcMem = CreateCompatibleDC(hDC);
		hbmMem = CreateCompatibleBitmap(hDC, rect.right - rect.left, rect.bottom - rect.top);
		hOld = SelectObject(hdcMem, hbmMem);
		SetTextColor(hdcMem, colorText); // устанавливаем цвет контекстного устройства
		EnemyPosList *enemy_pos_list;
		get_enemy_pos_list(&enemy_pos_list);
		char buffer[10];
		DrawText(hdcMem, itoa(enemy_pos_list->size, buffer, 10), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); // рисуем текст
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
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); //Создаётся объект
		SelectObject(hdcMem, hPen); //Объект делается текущим
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
		EndPaint(hWnd, &ps); // заканчиваем рисовать
		break;
	case WM_DESTROY: // если окошко закрылось, то:
		PostQuitMessage(0); // отправляем WinMain() сообщение WM_QUIT
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam); // если закрыли окошко
	}
	return 0; // возвращаем значение
}