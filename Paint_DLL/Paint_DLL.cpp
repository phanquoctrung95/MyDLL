// Paint_DLL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Paint_DLL.h"
#include"DrawLib.h"
#include<windowsx.h>
#include<vector>
#include<fstream>

using namespace std;
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PAINT_DLL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINT_DLL));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINT_DLL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAINT_DLL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
void readFile(FILE *fin, int &type, int &x, int &y, int &z, int &t);
void writeFile(FILE *fout, int type, int x, int y, int z, int t);
vector<CShape*> shapes;
vector<CShape*> prototypes;
bool isDrawing = FALSE;
int currentX;
int currentY;
int lastX;
int lastY;
CShape* shape;
int quantum = 0;
CLine* line;
CRectangle* rect;
int whichShape = -1;
int flag = 0;
int check = 0;
FILE *fout;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int x;
	int y;
	
	switch (message)
	{		
	case WM_CREATE:{
					   
					   //openning file to writting					 
					   fopen_s(&fout, "data.dat", "rb+");
					   
					   prototypes.push_back(new CLine);
					   prototypes.push_back(new CRectangle);
					   prototypes.push_back(new CCircle);
	}
		break;
	case WM_LBUTTONDOWN:
		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);
		if (check == 0){
			fwrite(&flag, sizeof(int), 1, fout);// chua vi tri dau tien de luu so luong hinh	
			check = -1;
		}
		switch (whichShape){
			case LINE:{
						  int a = LINE;						  
						  shape = prototypes[LINE]->Create();
						  shape->SetData(x, y, x+100, y+30);
						  shapes.push_back(shape);
						  InvalidateRect(hWnd, NULL, TRUE);				
						  writeFile(fout, a, x, y, x+100, y+30);
						  quantum++;
			}
				break;
			case RECT:{
						  int a = RECT;
						  shape = prototypes[RECT]->Create();
						  shape->SetData(x, y, x+200, y+100);					
						  shapes.push_back(shape);
						  InvalidateRect(hWnd, NULL, TRUE);
						  writeFile(fout, a, x, y, x+200, y+100);
						  quantum++;
			}
				break;
			case CIRCLE:{
							int a = CIRCLE;
							shape = prototypes[CIRCLE]->Create();
							shape->SetData(x - 100, y, x + 100, y + 200);
							shapes.push_back(shape);
							InvalidateRect(hWnd, NULL, TRUE);
							writeFile(fout, a, x - 100, y, x + 100, y + 200);
							quantum++;
			}
				break;
			default:{
						if (!isDrawing) {
							isDrawing = TRUE;
							currentX = x;
							currentY = y;
						}
			}
				break;
		}
	case WM_MOUSEMOVE:
	{
						 x = GET_X_LPARAM(lParam);
						 y = GET_Y_LPARAM(lParam);
						 WCHAR buffer[200];
						 wsprintf(buffer, L"%d %d", x, y);
						 SetWindowText(hWnd, buffer);
						 if (isDrawing) {
							 lastX = x;
							 lastY = y;							 
						 }
						 InvalidateRect(hWnd, NULL, TRUE);
	} break;

	case WM_LBUTTONUP: {
						   if (whichShape == -1){
							   int a = LINE;
							   x = GET_X_LPARAM(lParam);
							   y = GET_Y_LPARAM(lParam);

							   CLine* line = new CLine;
							   line->SetData(currentX, currentY, x, y);
							   shapes.push_back(line);
							   writeFile(fout, a, currentX, currentY, x, y);
							   isDrawing = FALSE;
							   quantum++;
							   InvalidateRect(hWnd, NULL, TRUE);
						   }
	}break;
	case WM_COMMAND:
		
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		HDC hdc;
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_DRAW_DRAWLINE:
			whichShape = LINE;	
			break;
		case ID_DRAW_DRAWRECT:
			whichShape=RECT;			
			break;
		case ID_DRAW_DRAWCIRCLE:
				whichShape = CIRCLE;			
			break;
		case ID_DRAW_DEAFAULT:
			whichShape = -1;
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_FILE_CLEAN:
			hWnd = 0;
			break;
		case ID_FILE_RESTORE:{
								 check = 1;
								 int type;
								 int x, y, z, t;
								 int loop = 1;
								 int size = 0;
								 fread(&size, sizeof(size), 1, fout);
								 if (size > 0){
									while (loop <= size){
										readFile(fout, type, x, y, z, t);
									shape = prototypes[type]->Create();
									shape->SetData(x, y, z, t);
									shapes.push_back(shape);
									loop++;
									}
								 }
								 InvalidateRect(hWnd, NULL, TRUE);
		}			
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:{
					  PAINTSTRUCT ps;		
					  HDC hdc = BeginPaint(hWnd, &ps);

					  for (int i = 0; i <shapes.size(); i++) {
						  shapes[i]->Draw(hdc);
					  }

					  if (isDrawing) {
						  MoveToEx(hdc, currentX, currentY, NULL);
						  LineTo(hdc, lastX, lastY);
					  }
					  SwapBuffers(hdc);
					  ReleaseDC(hWnd, hdc);
					  EndPaint(hWnd, &ps);					  
	}
		break;
	case WM_DESTROY:		
		rewind(fout);// luu so luong hinh vao vi tri chua san
		if (quantum > 0){
		fwrite(&quantum, sizeof(int), 1, fout);
		}
		fclose(fout);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)	
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void writeFile(FILE *f,int type, int x, int y, int z, int t){
	fwrite(&type, sizeof(type), 1, f);
	fwrite(&x, sizeof(x), 1, f);
	fwrite(&y, sizeof(y), 1, f);
	fwrite(&z, sizeof(z), 1, f);
	fwrite(&t, sizeof(t), 1, f);
}
void readFile(FILE *fin,int &type, int &x, int &y, int &z, int &t){
	fread(&type, sizeof(type), 1, fin);
	fread(&x, sizeof(x), 1, fin);
	fread(&y, sizeof(y), 1, fin);
	fread(&z, sizeof(z), 1, fin);
	fread(&t, sizeof(t), 1, fin);
}
