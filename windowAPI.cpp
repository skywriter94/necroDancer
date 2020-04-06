﻿#include "stdafx.h"
#include "playGround.h"
//API : Application Programming Interface


HINSTANCE	_hInstance;
HWND		_hWnd;

POINT _ptMouse;		//마우스 용 POINT
BOOL _leftButtonDown;	//중복 클릭 방지용

playGround _pg;

//함수의 프로토타입 선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void setWindowsSize(int x, int y, int width, int height);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	MSG			message;
	WNDCLASS	wndClass;

	_hInstance = hInstance;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = WINNAME;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	
	RegisterClass(&wndClass);

	_hWnd = CreateWindow(
		WINNAME,
		WINNAME,
		WS_OVERLAPPEDWINDOW,
		WINSTARTX,
		WINSTARTY,
		WINSIZEX,
		WINSIZEY,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);
	
	setWindowsSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);
	
	//방법 1
	//cvNamedWindow(WINNAME);							 // OPENCV타입 이름의 WINDOW 생성 
	//cvResizeWindow(WINNAME, WINSIZEX, WINSIZEY);	 // 사이즈 조정  
	//
	//moveWindow(WINNAME, WINSTARTX, WINSTARTY);
	//_hWnd = (HWND)cvGetWindowHandle(WINNAME);		 // _hwnd로 대입 
	//_hWnd = GetParent(_hWnd);	
	// 

	//방법2 	
	cv::namedWindow(WINNAME);
	HWND cvWnd = (HWND)cvGetWindowHandle(WINNAME);
	if (!cvWnd) return false;
	HWND oldParent = ::GetParent(cvWnd);
	::SetParent(cvWnd, oldParent);
	//::ShowWindow(oldParent, SW_HIDE);
	// resizeWindow(WINNAME, WINSIZEX + 100, WINSIZEY+ 50); //동영상 크기라서 안됌 
	moveWindow(WINNAME, -74, -120);		// 이동시키기 
	::SetParent(oldParent, _hWnd);
	ShowWindow(_hWnd, cmdShow);
	
	//메시지 루프 돌기이전에
	if (FAILED(_pg.init()))
	{

		return 0;
	}
	/*
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	*/
	//PeekMessage를 끄는 이유는 윈도우 자체 메시지 버튼에서 눌려졌을때만 메시지가 가야하는데
	//픽메세지는 계속해서 루프도니까 너무 화면이 심하게 껌뻑거려서 잠깐 끈거임
	//당연하겠지만 원래 우리가 이미지로 씌워서 버튼 사용할꺼면 픽메세지로 해도 무방!

	
	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TIMEMANAGER->update(60.0f);
			_pg.update();
			_pg.render();
		}
	}
		
	//루프문이 다돌면 플레이그라운드 해제
	_pg.release();
	UnregisterClass(WINNAME, hInstance);
	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _pg.MainProc(hWnd, iMessage, wParam, lParam);
}

//클라이언트 영역 재조정
void setWindowsSize(int x, int y, int width, int height)
{
	RECT winRect;

	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;
	winRect.bottom = height;

	AdjustWindowRect(&winRect, WINSTYLE, false);

	//실절적으로 클라이언트 영역 크기 셋팅 함수
	SetWindowPos(_hWnd, NULL, x, y,
		(winRect.right - winRect.left),
		(winRect.bottom - winRect.top),
		SWP_NOZORDER | SWP_NOMOVE);

}
