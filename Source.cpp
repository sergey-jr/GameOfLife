#include <iostream>
#include <ctime>
#include <vector>
#include <Windows.h>
#include <conio.h>

using namespace std;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR *mainMessage = "";
int width, height;
vector<bool> field;
void start()
{
	width = 49;
	height = 23;
	field.resize(width*height);
	srand((unsigned)time(NULL));
	for (size_t i = 0; i < field.size(); i++)
		field[i] = rand() % 2;
}
BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2);
}
void draw(HWND hWnd)
{
	HDC hdc = GetDC(hWnd); //получаем DC(контекст устройства) для рисования
	HPEN hpen1; //объявляем объект перо
	HGDIOBJ hpenOld, hbrushOld;
	HBRUSH hbrush; //объявляем кисть
				   //SetWindowText(hWnd, "TASK44");

	RECT rect;

	GetClientRect(hWnd, &rect);
	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
	hpen1 = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); //логическое перо с заданным стилем, шириной и цветом
	hpenOld = (HPEN)SelectObject(hdc, hpen1);
	hbrush = CreateSolidBrush(RGB(255, 255, 255));
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	for (int i = 0,y=0; i < height; i++,y+=20)
	{
		for (int j = 0,x=0; j < width; j++,x+=20)
		{
			if (!field[j + i*width])
			{
				Line(hdc, x, y, x + 20, y);
				Line(hdc, x + 20, y, x + 20, y + 20);
				Line(hdc, x + 20, y + 20, x, y + 20);
			}
			else
			{
				Line(hdc, x, y, x + 20, y);
				Line(hdc, x + 20, y, x + 20, y + 20);
				Line(hdc, x + 20, y + 20, x, y + 20);
				Ellipse(hdc, x + 5, y + 5, x + 15, y + 15);
			}
		}
	}
}
void checker(int &countCells, int i, int j)
{
	//top-left cell of current 
	if ((i - 1) >= 0 && (j - 1) >= 0)
	{
		if (field[(j - 1) + (i - 1)*width])
			countCells++;
	}
	//top cell of current 
	if ((i - 1) >= 0)
	{
		if (field[j + (i - 1)*width])
			countCells++;
	}
	//top-right cell of current 
	if ((i - 1) >= 0 && (j + 1) <= (width - 1))
	{
		if (field[(j + 1) + (i - 1)*width])
			countCells++;
	}
	//left cell of current
	if ((j - 1) >= 0)
	{
		if (field[(j - 1) + i*width])
			countCells++;
	}
	//right cell of current
	if ((j + 1) <= width - 1)
	{
		if (field[(j + 1) + i*width])
			countCells++;
	}
	//bottom-left cell of current
	if ((i + 1) <= (height - 1) && (j - 1) >= 0)
	{
		if (field[(j - 1) + (i + 1)*width])
			countCells++;
	}
	//bottom cell of current
	if ((i + 1) <= (height - 1))
	{
		if (field[j + (i + 1)*width])
			countCells++;
	}
	//bottom-right cell of current
	if ((i + 1) <= (height - 1) && (j + 1) <= (width - 1))
	{
		if (field[(j + 1) + (i + 1)*width])
			countCells++;
	}
}
void logic()
{
	vector<bool> nField = field;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int countCells = 0;
			checker(countCells, i, j);
			if (countCells < 2 || countCells>3)
				nField[j + i*width] = false;
			else
			{
				if (!field[j + i*width] && countCells != 3)
					nField[j + i*width] = false;
				else
					nField[j + i*width] = true;
			}
		}
	}
	field = nField;
}
bool endOfGame()
{
	for (size_t i = 0; i<field.size(); i++)
		if (field[i])
			return true;
	return false;
}
int WINAPI WinMain(HINSTANCE hInst,
	HINSTANCE hPrevInst,
	LPSTR lpCmdLine,
	int nCmdShow) // режим отображения окошка
{

	TCHAR szClassName[] = "My class"; // строка с именем класса
	HWND hMainWnd; // создаём дескриптор будущего окошка
	MSG msg; // создём экземпляр структуры MSG для обработки сообщений
	WNDCLASSEX wc; // создаём экземпляр, для обращения к членам класса WNDCLASSEX
	wc.cbSize = sizeof(wc); // размер структуры (в байтах)
	wc.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окошка
	wc.lpfnWndProc = WndProc; // указатель на пользовательскую функцию
	wc.lpszMenuName = NULL; // указатель на имя меню (у нас его нет)
	wc.lpszClassName = szClassName; // указатель на имя класса
	wc.cbWndExtra = NULL; // число освобождаемых байтов в конце структуры
	wc.cbClsExtra = NULL; // число освобождаемых байтов при создании экземпляра приложения
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE("\\res\Icon.ico")); // декриптор пиктограммы
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // дескриптор маленькой пиктограммы (в трэе)
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // дескриптор курсора
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // дескриптор кисти для закраски фона окна
	wc.hInstance = hInst; // указатель на строку, содержащую имя меню, применяемого для класса
	if (!RegisterClassEx(&wc)) {
		// в случае отсутствия регистрации класса:
		MessageBox(NULL, "Не получилось зарегистрировать класс!", "Ошибка", MB_OK);
		return NULL; // возвращаем, следовательно, выходим из WinMain
	}
	// Функция, создающая окошко:
	hMainWnd = CreateWindow(
		szClassName, // имя класса
		"Game of Life", // имя окошка (то что сверху)
		WS_OVERLAPPEDWINDOW | WS_VSCROLL, // режимы отображения окошка
		CW_USEDEFAULT, // позиция окошка по оси х
		NULL, // позиция окошка по оси у (раз дефолт в х, то писать не нужно)
		CW_USEDEFAULT, // ширина окошка
		NULL, // высота окошка (раз дефолт в ширине, то писать не нужно)
		(HWND)NULL, // дескриптор родительского окна
		NULL, // дескриптор меню
		HINSTANCE(hInst), // дескриптор экземпляра приложения
		NULL); // ничего не передаём из WndProc
	if (!hMainWnd) {
		// в случае некорректного создания окошка (неверные параметры и тп):
		MessageBox(NULL, "Не получилось создать окно!", "Ошибка", MB_OK);
		return NULL;
	}
	ShowWindow(hMainWnd, nCmdShow); // отображаем окошко
	
	UpdateWindow(hMainWnd); // обновляем окошко
	while (GetMessage(&msg, NULL, NULL, NULL)) { // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
		TranslateMessage(&msg); // интерпретируем сообщения
		DispatchMessage(&msg); // передаём сообщения обратно ОС
	}
	return NULL; // возвращаем код выхода из приложения
}
char *itoc(double x, int redux)
{
	int k = 0;
	int wl = x * 100;
	int wa = x * 100;
	while (wa)
	{
		wa /= redux;
		k++;
	}
	char *res = new char[k + 1];
	for (int i = 0; i < k + 2; i++)
		res[i] = 0;
	int i = k;
	while (wl)
	{
		if (i != k - 2)
		{
			res[i] = (wl % redux) + '0';
			wl /= redux;
		}
		if (wl)
			i--;

	}
	if (wl)
		res[i + 1] = wl%redux + '0';
	if (!wl) res = "0";
	return res;
}
void worker(HWND hWnd)
{
	start();
	size_t i = 1;
	vector<bool> prev = field;
	while (endOfGame())
	{
		cout << "generation " << i << endl;
		string s = "generation ";
		s += itoc(i,10);
		char *res = new char[s.length()+1];
		for (size_t i = 0; i < s.length(); i++)
			res[i] = s[i];
		mainMessage = res;
		draw(hWnd);
		logic();
		if (field == prev)
			break;
		else
			prev = field;
		Sleep(2000);

		i++;
	}
	_getch();
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hDC; // создаём дескриптор ориентации текста на экране
	PAINTSTRUCT ps; // структура, сод-щая информацию о клиентской области (размеры, цвет и тп)
	RECT rect; // стр-ра, определяющая размер клиентской области
	COLORREF colorText = RGB(0, 0, 0); // задаём цвет текста
	switch (uMsg) {
	case WM_PAINT: // если нужно нарисовать, то:
		hDC = BeginPaint(hWnd, &ps); // инициализируем контекст устройства
		GetClientRect(hWnd, &rect); // получаем ширину и высоту области для рисования
		SetTextColor(hDC, colorText); // устанавливаем цвет контекстного устройства
		worker(hWnd);
		MoveToEx(hDC, 0, 49 * 20 + 1, NULL);
		DrawText(hDC, mainMessage, -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER); // рисуем текст\
		EndPaint(hWnd, &ps); // заканчиваем рисовать
		break;
	case WM_DESTROY: // если окошко закрылось, то:
		PostQuitMessage(NULL); // отправляем WinMain() сообщение WM_QUIT
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam); // если закрыли окошко
	}
	return NULL; // возвращаем значение
}
