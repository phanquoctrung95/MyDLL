#include <windows.h>
#pragma once

#ifdef DRAWLIBRARY_EXPORTS
#define DRAWLIBRARY_API __declspec(dllexport) 
#else
#define DRAWLIBRARY_API __declspec(dllimport) 
#endif


namespace DrawLibrary
{
	class Functions
	{
	public:
		static DRAWLIBRARY_API void DrawLine(HDC hdc, int left, int top, int right, int bottom);
		static DRAWLIBRARY_API void DrawRect(HDC hdc, int left, int top, int right, int bottom);
		static DRAWLIBRARY_API void DrawCircle(HDC hdc, int left, int top, int right, int bottom);
	};
}
extern "C" DRAWLIBRARY_API void DrawLine(HDC hdc, int left, int top, int right, int bottom);
extern "C" DRAWLIBRARY_API void DrawRect(HDC hdc, int left, int top, int right, int bottom);
extern "C" DRAWLIBRARY_API void DrawCircle(HDC hdc, int left, int top, int right, int bottom);
extern "C" DRAWLIBRARY_API int Add(int a, int b);

class CShape {
public:
	virtual void Draw(HDC hdc) = 0;
	virtual CShape* Create() = 0;
	virtual void SetData(int a, int b, int c, int d) = 0;
};

class CLine : public CShape {
public:

	int x1;
	int y1;
	int x2;
	int y2;
	void Draw(HDC hdc) {
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x2, y2);
	}

	CShape* Create() { return new CLine; }

	void SetData(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}
};

class CRectangle : public CShape {
public:
	int x1;
	int y1;
	int x2;
	int y2;
	void Draw(HDC hdc) {
		Rectangle(hdc, x1, y1, x2, y2);
	}

	CShape* Create() { return new CRectangle; }

	void SetData(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}
};
class CCircle : public CShape {
public:

	int left;
	int top;
	int right;
	int bottom;
	void Draw(HDC hdc) {
		Ellipse(hdc, left, top, right, bottom);
	}

	CShape* Create() { return new CCircle; }

	void SetData(int a, int b, int c, int d) {
		left = a;
		top = b;
		right = c;
		bottom = d;
	}
};
