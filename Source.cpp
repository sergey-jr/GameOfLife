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
	MoveToEx(hdc, x1, y1, NULL); //������� �������� ���������� x1, y1
	return LineTo(hdc, x2, y2);
}
void draw(HWND hWnd)
{
	HDC hdc = GetDC(hWnd); //�������� DC(�������� ����������) ��� ���������
	HPEN hpen1; //��������� ������ ����
	HGDIOBJ hpenOld, hbrushOld;
	HBRUSH hbrush; //��������� �����
				   //SetWindowText(hWnd, "TASK44");

	RECT rect;

	GetClientRect(hWnd, &rect);
	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
	hpen1 = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); //���������� ���� � �������� ������, ������� � ������
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
	int nCmdShow) // ����� ����������� ������
{

	TCHAR szClassName[] = "My class"; // ������ � ������ ������
	HWND hMainWnd; // ������ ���������� �������� ������
	MSG msg; // ����� ��������� ��������� MSG ��� ��������� ���������
	WNDCLASSEX wc; // ������ ���������, ��� ��������� � ������ ������ WNDCLASSEX
	wc.cbSize = sizeof(wc); // ������ ��������� (� ������)
	wc.style = CS_HREDRAW | CS_VREDRAW; // ����� ������ ������
	wc.lpfnWndProc = WndProc; // ��������� �� ���������������� �������
	wc.lpszMenuName = NULL; // ��������� �� ��� ���� (� ��� ��� ���)
	wc.lpszClassName = szClassName; // ��������� �� ��� ������
	wc.cbWndExtra = NULL; // ����� ������������� ������ � ����� ���������
	wc.cbClsExtra = NULL; // ����� ������������� ������ ��� �������� ���������� ����������
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE("\\res\Icon.ico")); // ��������� �����������
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // ���������� ��������� ����������� (� ����)
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ���������� �������
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ���������� ����� ��� �������� ���� ����
	wc.hInstance = hInst; // ��������� �� ������, ���������� ��� ����, ������������ ��� ������
	if (!RegisterClassEx(&wc)) {
		// � ������ ���������� ����������� ������:
		MessageBox(NULL, "�� ���������� ���������������� �����!", "������", MB_OK);
		return NULL; // ����������, �������������, ������� �� WinMain
	}
	// �������, ��������� ������:
	hMainWnd = CreateWindow(
		szClassName, // ��� ������
		"Game of Life", // ��� ������ (�� ��� ������)
		WS_OVERLAPPEDWINDOW | WS_VSCROLL, // ������ ����������� ������
		CW_USEDEFAULT, // ������� ������ �� ��� �
		NULL, // ������� ������ �� ��� � (��� ������ � �, �� ������ �� �����)
		CW_USEDEFAULT, // ������ ������
		NULL, // ������ ������ (��� ������ � ������, �� ������ �� �����)
		(HWND)NULL, // ���������� ������������� ����
		NULL, // ���������� ����
		HINSTANCE(hInst), // ���������� ���������� ����������
		NULL); // ������ �� ������� �� WndProc
	if (!hMainWnd) {
		// � ������ ������������� �������� ������ (�������� ��������� � ��):
		MessageBox(NULL, "�� ���������� ������� ����!", "������", MB_OK);
		return NULL;
	}
	ShowWindow(hMainWnd, nCmdShow); // ���������� ������
	
	UpdateWindow(hMainWnd); // ��������� ������
	while (GetMessage(&msg, NULL, NULL, NULL)) { // ��������� ��������� �� �������, ���������� ��-�����, ��
		TranslateMessage(&msg); // �������������� ���������
		DispatchMessage(&msg); // ������� ��������� ������� ��
	}
	return NULL; // ���������� ��� ������ �� ����������
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
	HDC hDC; // ������ ���������� ���������� ������ �� ������
	PAINTSTRUCT ps; // ���������, ���-��� ���������� � ���������� ������� (�������, ���� � ��)
	RECT rect; // ���-��, ������������ ������ ���������� �������
	COLORREF colorText = RGB(0, 0, 0); // ����� ���� ������
	switch (uMsg) {
	case WM_PAINT: // ���� ����� ����������, ��:
		hDC = BeginPaint(hWnd, &ps); // �������������� �������� ����������
		GetClientRect(hWnd, &rect); // �������� ������ � ������ ������� ��� ���������
		SetTextColor(hDC, colorText); // ������������� ���� ������������ ����������
		worker(hWnd);
		MoveToEx(hDC, 0, 49 * 20 + 1, NULL);
		DrawText(hDC, mainMessage, -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER); // ������ �����\
		EndPaint(hWnd, &ps); // ����������� ��������
		break;
	case WM_DESTROY: // ���� ������ ���������, ��:
		PostQuitMessage(NULL); // ���������� WinMain() ��������� WM_QUIT
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam); // ���� ������� ������
	}
	return NULL; // ���������� ��������
}
