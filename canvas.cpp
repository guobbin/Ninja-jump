#include "stdafx.h"
#include <stdio.h>

#include "mmsystem.h"

#pragma comment(lib,"winmm.lib")
#define maxkc  10
#define maxbc  5
HINSTANCE hInst;
HBITMAP ninjaup, ninjastop, bg, gover, house1, house2, house3, ninjajump, dirc, np, dao, ins, bat,
bbomb, blood, bloodstrip, timestrip, number, gradenum, begin, dun, huo;
HDC		hdc, mdc, bufdc;
HWND	hWnd;
DWORD	tPre, tNow, tCheck, tShield;
int		h, dir, x, y, y1, y2, vhouse, vhousenow, num1, num2, num3, num4,
vx, vy, vynow, vg, H, shieldtime;

int HP = 10;
char buf[50];
bool    jump1, jump2, up, stop, power, down, alive, again, close, start, shield;
bool easy, ordinary, difficult = true;
struct KNIFE
{
	int y;//刀贴的高度
	int x;//25 375
	int y1;
	int blood;//0,1
	bool Left;
	bool Short;
	bool exist;
	bool broken;
}knife[10];
struct BAT {
	int x;
	int y;
	int y1;
	int num4;
	int num;
	bool Left;
	bool exist;
	bool bomb;
	bool bombed;
}Bat[5];
struct BLOOD {
	int x, y;
	int kinds;//1,2,3不同概率出现
	bool exist;
}Blood = { 0,-50,1,false };

struct FIRE {
	int x, y;
	bool exist;
}Fire = { 0,-50,false };
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				MyPaint(HDC hdc);
void                GameOver(HDC hdc);
void                GameBegin(HDC hdc);
void    paintPH();
void    paintts();
void    produceknife();
void    producebat();
void 	produceblood();
void    producefire();
void    paintbg();
void    paintpower();
void    paintjump1();
void    paintjump2();
void    paintup();
void    paintdown();
void    paintstop();
void    paintheight();
void    paintshield();
void    bgmusic();
void    jumpmusic();
void    runmusic();
void    stickmusic();
void    renew() {
	for (int i = 0; i < maxkc; i++) {
		knife[i].exist = false;
		knife[i].y = 0;
	}
	for ( int i = 0; i < maxbc; i++)
		Bat[i].exist = false;
	Blood.exist = false;
	Fire.exist = false;
	y2 = -50;
	vhouse = 1;
	vhousenow = 1;
	vx = 10;
	vy = 20;
	vynow = 20;
	vg = 1;
	//knifecount = 0;
	HP = 10;
	x = 15;
	y = 300;
	y1 = 0;
	dir = 0;
	h = 0;
	num1 = 0;
	num2 = 17;
	num3 = 0;
	num4 = 0;
	H = 0;
	jump1 = jump2 = up = down = false;
	stop = alive = true;
	again = close = start = false;
	shield = false;
}
void    paintnum(int numx, int numy, int num);
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;

	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	do {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			int T;
			tNow = GetTickCount();
			if (H < 2000)
				T = 30;
			else if (H < 4000)
				T = 17;
			else if (H < 6000)
				T = 16;
			else
				T = 15;
			if (tNow - tPre >= (unsigned int)T) {

				if (start)
				{
					if (alive)
						MyPaint(hdc);
					else
					{
						GameOver(hdc);
						if (again) {
							alive = true;
							renew();

						}
						if (close)
							exit(0);
					}
				}
				else {
					GameBegin(hdc);
				}
			}
		}
	} while (msg.message != WM_QUIT);
	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "canvas";
	wcex.hIconSm = NULL;
	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HBITMAP bmp;
	hInst = hInstance;

	hWnd = CreateWindow("canvas", "Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	MoveWindow(hWnd, 400, 0, 518, 800, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(hdc);
	bufdc = CreateCompatibleDC(hdc);

	bmp = CreateCompatibleBitmap(hdc, 530, 800);
	SelectObject(mdc, bmp);
	renew();

	np = (HBITMAP)LoadImage(NULL, "power.bmp", IMAGE_BITMAP, 296, 303, LR_LOADFROMFILE);
	ninjaup = (HBITMAP)LoadImage(NULL, "ninjau.bmp", IMAGE_BITMAP, 296, 303, LR_LOADFROMFILE);
	ninjastop = (HBITMAP)LoadImage(NULL, "ninjas.bmp", IMAGE_BITMAP, 296, 303, LR_LOADFROMFILE);
	dirc = (HBITMAP)LoadImage(NULL, "dir.bmp", IMAGE_BITMAP, 500, 250, LR_LOADFROMFILE);
	ins = (HBITMAP)LoadImage(NULL, "ins.bmp", IMAGE_BITMAP, 50, 100, LR_LOADFROMFILE);
	ninjajump = (HBITMAP)LoadImage(NULL, "jump.bmp", IMAGE_BITMAP, 1500, 600, LR_LOADFROMFILE);
	bg = (HBITMAP)LoadImage(NULL, "bg.bmp", IMAGE_BITMAP, 1500, 800, LR_LOADFROMFILE);
	gover = (HBITMAP)LoadImage(NULL, "gg.bmp", IMAGE_BITMAP, 1500, 800, LR_LOADFROMFILE);
	house1 = (HBITMAP)LoadImage(NULL, "house1.bmp", IMAGE_BITMAP, 500, 1600, LR_LOADFROMFILE);
	house2 = (HBITMAP)LoadImage(NULL, "house2.bmp", IMAGE_BITMAP, 500, 1600, LR_LOADFROMFILE);
	house3 = (HBITMAP)LoadImage(NULL, "house3.bmp", IMAGE_BITMAP, 500, 1600, LR_LOADFROMFILE);
	dao = (HBITMAP)LoadImage(NULL, "knife.bmp", IMAGE_BITMAP, 315, 280, LR_LOADFROMFILE);
	bbomb = (HBITMAP)LoadImage(NULL, "bbomb1.bmp", IMAGE_BITMAP, 300, 200, LR_LOADFROMFILE);
	bat = (HBITMAP)LoadImage(NULL, "bat.bmp", IMAGE_BITMAP, 500, 300, LR_LOADFROMFILE);
	blood = (HBITMAP)LoadImage(NULL, "blood2.bmp", IMAGE_BITMAP, 210, 175, LR_LOADFROMFILE);
	bloodstrip = (HBITMAP)LoadImage(NULL, "bloodstrip.bmp", IMAGE_BITMAP, 600, 50, LR_LOADFROMFILE);
	number = (HBITMAP)LoadImage(NULL, "number.bmp", IMAGE_BITMAP, 600, 100, LR_LOADFROMFILE);
	begin = (HBITMAP)LoadImage(NULL, "begin.bmp", IMAGE_BITMAP, 500, 800, LR_LOADFROMFILE);
	gradenum = (HBITMAP)LoadImage(NULL, "gradenum.bmp", IMAGE_BITMAP, 600, 120, LR_LOADFROMFILE);
	dun = (HBITMAP)LoadImage(NULL, "dun.bmp", IMAGE_BITMAP, 600, 300, LR_LOADFROMFILE);
	huo = (HBITMAP)LoadImage(NULL, "huo.bmp", IMAGE_BITMAP, 300, 100, LR_LOADFROMFILE);
	timestrip = (HBITMAP)LoadImage(NULL, "timestrip.bmp", IMAGE_BITMAP, 600, 30, LR_LOADFROMFILE);


	//MyPaint(hdc);
	return TRUE;
}
void GameOver(HDC hdc) {
	SelectObject(bufdc, gover);
	BitBlt(mdc, 0, 0, 500, 800, bufdc, (int(easy) * 0 + int(ordinary) * 1 + int(difficult) * 2) * 500, 0, SRCCOPY);
	//	BitBlt(mdc, 0, 0, 500, 800, bufdc, 0, 0, SRCCOPY);
	//	BitBlt(mdc, 0, 0, 500, 800, mdc, 0, 0, SRCCOPY);
	FILE*fp;
	int best;
	char filename[20];
	if (easy)
		strcpy(filename, "grade0.txt");
	if (ordinary)
		strcpy(filename, "grade1.txt");
	if (difficult)
		strcpy(filename, "grade2.txt");
	if (NULL == (fp = fopen(filename, "r")))
		exit(1);
	fscanf(fp, "%d", &best);
	if (best < H)
		best = H;
	fclose(fp);
	if (NULL == (fp = fopen(filename, "w")))
		exit(2);
	fprintf(fp, "%d", best);
	fclose(fp);
	//paintnum(265,375 , H);
	int l;
	int num = H;
	SelectObject(bufdc, gradenum);
	for (int i = 0; i < 6; i++) {
		l = num % 10;

		BitBlt(mdc, 305 - i * 40, 320, 60, 60, bufdc, l * 60, 60, SRCAND);
		BitBlt(mdc, 305 - i * 40, 320, 60, 60, bufdc, l * 60, 0, SRCPAINT);


		num = num / 10;
		if (!num)
			break;
	}
	paintnum(265, 210, best);
	tPre = GetTickCount();
	BitBlt(hdc, 0, 0, 500, 800, mdc, 0, 0, SRCCOPY);
}
void GameBegin(HDC hdc) {
	SelectObject(bufdc, begin);
	BitBlt(mdc, 0, 0, 500, 800, bufdc, 0, 0, SRCCOPY);
	//BitBlt(mdc, 0, 0, 500, 800, mdc, 0, 0, SRCCOPY);
	/*char str[20] = "";
	sprintf(str, "X  %d    ", x);
	TextOut(mdc, 0, 0, str, strlen(str));
	sprintf(str, "Y  %d    ", y);
	TextOut(mdc, 0, 20, str, strlen(str));
	tPre = GetTickCount();*/
	BitBlt(hdc, 0, 0, 500, 800, mdc, 0, 0, SRCCOPY);
}
void MyPaint(HDC hdc)
{

	paintbg();
	produceknife();
	produceblood();
	if (!easy)
		producefire();
	paintPH();
	paintheight();
	if (power)
		paintpower();

	if (jump1)//num1;
		paintjump1();

	if (jump2)//num3
		paintjump2();

	if (up)//num2
		paintup();

	if (num2 == 0)
		num2 = 17;
	num2--;

	if (stop) {
		paintstop();
	}//停止

	if (down)
		paintdown();
	//下降

	if (!easy)producebat();

	if (y>750 || HP <= 0) {
		alive = false;
		again = false;
		bgmusic();
	}//判断死亡

	if (shield) {
		paintshield();
		paintts();
	}

	/*char str0[20] = "";
	char str1[20] = "";
	char str2[20] = "";
	sprintf_s(str0, "HP  %d    ", HP);
	TextOut(mdc, 0, 0, str0, strlen(str0));
	sprintf_s(str1, "HEIGHT  %d    ", H);
	TextOut(mdc, 400, 0, str1, strlen(str1));*///普通文本打印

	tPre = GetTickCount();
	BitBlt(hdc, 0, 0, 500, 800, mdc, 0, 0, SRCCOPY);

}

void paintbg() {
	SelectObject(bufdc, bg);
	BitBlt(mdc, 0, 0, 500, 800, bufdc, (int(easy) * 0 + int(ordinary) * 1 + int(difficult) * 2) * 500, 0, SRCCOPY);
	if (easy)
		SelectObject(bufdc, house1);
	if (ordinary)
		SelectObject(bufdc, house2);
	if (difficult)
		SelectObject(bufdc, house3);
	BitBlt(mdc, 0, 0, 500, y1, bufdc, 0, 1600 - y1, SRCAND);
	BitBlt(mdc, 0, y1, 500, 800 - y1, bufdc, 0, 800, SRCAND);
	BitBlt(mdc, 0, 0, 500, y1, bufdc, 0, 800 - y1, SRCPAINT);
	BitBlt(mdc, 0, y1, 500, 800 - y1, bufdc, 0, 0, SRCPAINT);

	y1 += vhousenow;
	if (y1 >= 800)
		y1 = 0;
}

void paintpower() {
	SelectObject(bufdc, np);
	BitBlt(mdc, x, y, 72, 101, bufdc, 148 * dir + 75, num2 / 6 * 101, SRCAND);
	BitBlt(mdc, x, y, 72, 101, bufdc, 148 * dir + 1, num2 / 6 * 101, SRCPAINT);
	y += vhousenow;

	// SelectObject(bufdc,dirc);  
	if (dir == 0) {
		//BitBlt(mdc,70,y-50,100,50,bufdc,(vynow-20)*100,188,SRCAND);
		//BitBlt(mdc,70,y-50,100,50,bufdc,(vynow-20)*100,126,SRCPAINT);
		if (y % 5 == 0 || true) {
			SelectObject(bufdc, ins);
			BitBlt(mdc, 430, y - 40 * (vynow - 20) - 110, 48, 48, bufdc, 1, 51, SRCAND);
			BitBlt(mdc, 430, y - 40 * (vynow - 20) - 110, 48, 48, bufdc, 1, 1, SRCPAINT);
		}
	}
	else {
		//	BitBlt(mdc,330,y-50,100,50,bufdc,(vynow-20)*100,63,SRCAND);
		//	BitBlt(mdc,330,y-50,100,50,bufdc,(vynow-20)*100,0,SRCPAINT);
		if (y % 5 == 0 || true) {
			SelectObject(bufdc, ins);
			BitBlt(mdc, 20, y - 40 * (vynow - 20) - 110, 48, 48, bufdc, 1, 51, SRCAND);
			BitBlt(mdc, 20, y - 40 * (vynow - 20) - 110, 48, 48, bufdc, 1, 1, SRCPAINT);
		}
	}
	if (num2 % (17 - (vhousenow - 1) * 7) == 0)
	{
		vynow++;
		if (vynow == 25)
			vynow = 20;
	}
}

void paintjump1() {
	//bgmusic();

	for (int i = 0; i<maxkc; i++) {
		if (knife[i].exist &&knife[i].Left && ((knife[i].Short&&y - knife[i].y<65) ||
			(!knife[i].Short&&y - knife[i].y<140)) && y - knife[i].y>-15 && x == 15)
		{//alive=false;
			if (!shield) {

				if (knife[i].blood == 0 && !knife[i].broken) {
					if (knife[i].Short)
						HP--;
					else
						HP -= 2;
					knife[i].blood = 1;
					mciSendString("close hurt ", buf, strlen(buf), NULL);
					mciSendString("open hurt.wav type avivideo alias hurt", buf, strlen(buf), NULL);
					mciSendString("play hurt ", buf, strlen(buf), NULL);
				}
			}
			else {
				knife[i].broken = true;
				mciSendString("close broken ", buf, strlen(buf), NULL);
				mciSendString("open broken.wav type avivideo alias broken", buf, strlen(buf), NULL);
				mciSendString("play broken ", buf, strlen(buf), NULL);
			}
		}
		if (knife[i].exist && !knife[i].Left && ((knife[i].Short&&y - knife[i].y<40) ||
			(!knife[i].Short&&y - knife[i].y<70)) && y - knife[i].y>-90 && x == 405)
		{//alive=false;
			if (!shield) {
				if (knife[i].blood == 0 && !knife[i].broken) {
					if (knife[i].Short)
					{
						mciSendString("close hurt ", buf, strlen(buf), NULL);
						mciSendString("open hurt.wav type avivideo alias hurt", buf, strlen(buf), NULL);
						mciSendString("play hurt ", buf, strlen(buf), NULL);
						HP--;
					}
					else
						HP -= 2;
					knife[i].blood = 1;
				}
			}
			else
			{
				knife[i].broken = true;
				mciSendString("close broken ", buf, strlen(buf), NULL);
				mciSendString("open broken.wav type avivideo alias broken", buf, strlen(buf), NULL);
				mciSendString("play broken ", buf, strlen(buf), NULL);
			}
		}
	}//判断是否被刀
	if (x == 15)//起跳
		y -= 50;
	num1++;
	if (num1<20 && num1 % 3 == 0)
		vhousenow++;
	if (num1>20 && num1 % 3 == 0)
		vhousenow--;
	H += vhousenow;

	SelectObject(bufdc, ninjajump);
	BitBlt(mdc, x, y, 74, 143, bufdc, 75 + num1 / 4 * 150, 156, SRCAND);
	BitBlt(mdc, x, y, 74, 143, bufdc, 75 + num1 / 4 * 150, 6, SRCPAINT);

	x += vx;
	vynow = vynow - vg;
	y = y - vynow;
	if (x >= 415) {//着落
		x = 415;
		dir = 1;
		jump1 = false;
		stop = true;
		vynow = vy;
		y += 35;
		vhousenow = vhouse;
		num1 = 0;
		//   y+=100;//测试；
	}
}

void paintjump2() {
	for (int i = 0; i<maxkc; i++)
	{
		if (knife[i].exist && !knife[i].Left && ((knife[i].Short&&y - knife[i].y<65) ||
			(!knife[i].Short&&y - knife[i].y<140)) && y - knife[i].y>-15 && x == 415)
		{//alive=false;
			if (!shield) {
				if (knife[i].blood == 0 && !knife[i].broken) {
					if (knife[i].Short)
						HP--;
					else
						HP -= 2;
					knife[i].blood = 1;
					mciSendString("close hurt ", buf, strlen(buf), NULL);
					mciSendString("open hurt.wav type avivideo alias hurt", buf, strlen(buf), NULL);
					mciSendString("play hurt ", buf, strlen(buf), NULL);
				}
			}
			else {
				knife[i].broken = true;
				mciSendString("close broken ", buf, strlen(buf), NULL);
				mciSendString("open broken.wav type avivideo alias broken", buf, strlen(buf), NULL);
				mciSendString("play broken ", buf, strlen(buf), NULL);
			}
		}
		if (knife[i].exist&&knife[i].Left && ((knife[i].Short&&y - knife[i].y<40) ||
			(!knife[i].Short&&y - knife[i].y<70)) && y - knife[i].y>-90 && x == 25)
		{//alive=false;
			if (!shield) {
				if (knife[i].blood == 0 && !knife[i].broken) {

					if (knife[i].Short)
						HP--;
					else
						HP -= 2;
					knife[i].blood = 1;
				}

			}
			else {
				knife[i].broken = true;
			}
		}
	}//判断是否被刀
	if (x == 415)//起跳
		y -= 50;
	num3++;
	if (num3<20 && num3 % 3 == 0)
		vhousenow++;
	if (num3>20 && num3 % 3 == 0)
		vhousenow--;

	H += vhousenow;
	SelectObject(bufdc, ninjajump);
	BitBlt(mdc, x, y, 74, 143, bufdc, num3 / 4 * 150, 456, SRCAND);
	BitBlt(mdc, x, y, 74, 143, bufdc, num3 / 4 * 150, 306, SRCPAINT);
	x -= vx;
	vynow = vynow - 1;
	y = y - vynow;
	if (x <= 15) {//着落
		x = 15;
		dir = 0;
		jump2 = false;
		stop = true;
		vynow = vy;
		y += 35;
		vhousenow = vhouse;
		num3 = 0;
	}

}

void paintup() {
	for (int i = 0; i<maxkc; i++)
	{
		if (knife[i].exist&&y - knife[i].y >= 30 - vhousenow&&y - knife[i].y<30 &&
			((x == 15 && knife[i].Left) || (x == 415 && !knife[i].Left)) && knife[i].Short)
		{		  // alive=false;
			if (!shield) {
				if (knife[i].blood == 0 && !knife[i].broken) {
					if (knife[i].Short)
						HP--;
					else
						HP -= 2;
					knife[i].blood = 1;
					mciSendString("close hurt ", buf, strlen(buf), NULL);
					mciSendString("open hurt.wav type avivideo alias hurt", buf, strlen(buf), NULL);
					mciSendString("play hurt ", buf, strlen(buf), NULL);
				}
			}
			else {
				knife[i].broken = true;
				mciSendString("close broken ", buf, strlen(buf), NULL);
				mciSendString("open broken.wav type avivideo alias broken", buf, strlen(buf), NULL);
				mciSendString("play broken ", buf, strlen(buf), NULL);
			}
			break;
		}
		if (knife[i].exist&&y - knife[i].y >= 32 - vhousenow&&y - knife[i].y<32 &&
			((x == 15 && knife[i].Left) || (x == 415 && !knife[i].Left)) && !knife[i].Short)
		{		  // alive=false;
			if (!shield) {
				if (knife[i].blood == 0 && !knife[i].broken) {
					if (knife[i].Short)
						HP--;
					else
						HP -= 2;
					knife[i].blood = 1;
					mciSendString("close hurt ", buf, strlen(buf), NULL);
					mciSendString("open hurt.wav type avivideo alias hurt", buf, strlen(buf), NULL);
					mciSendString("play hurt ", buf, strlen(buf), NULL);
				}
			}
			else {
				knife[i].broken = true;
				mciSendString("close broken ", buf, strlen(buf), NULL);
				mciSendString("open broken.wav type avivideo alias broken", buf, strlen(buf), NULL);
				mciSendString("play broken ", buf, strlen(buf), NULL);
			}
			break;
		}
	}//判断是否被刀；
	SelectObject(bufdc, ninjaup);
	BitBlt(mdc, x, y, 72, 101, bufdc, 75 + dir * 148, num2 / 6 * 101, SRCAND);
	BitBlt(mdc, x, y, 72, 101, bufdc, 1 + dir * 148, num2 / 6 * 101, SRCPAINT);
	H += vhousenow;

}

void paintdown() {
	for (int i = 0; i<maxkc; i++)
	{
		if (knife[i].exist&&y - knife[i].y >= -45 - vhousenow&&y - knife[i].y<-45 &&
			((x == 15 && knife[i].Left) || (x == 415 && !knife[i].Left)) && knife[i].Short)
		{		  // alive=false;
			if (!shield) {
				if (knife[i].blood == 0 && !knife[i].broken) {
					if (knife[i].Short)
						HP--;
					else
						HP -= 2;
					knife[i].blood = 1;
					mciSendString("close hurt ", buf, strlen(buf), NULL);
					mciSendString("open hurt.wav type avivideo alias hurt", buf, strlen(buf), NULL);
					mciSendString("play hurt ", buf, strlen(buf), NULL);
				}
			}
			else {
				knife[i].broken = true;
				mciSendString("close broken ", buf, strlen(buf), NULL);
				mciSendString("open broken.wav type avivideo alias broken", buf, strlen(buf), NULL);
				mciSendString("play broken ", buf, strlen(buf), NULL);
			}
			break;
		}
		if (knife[i].exist&&y - knife[i].y >= -43 - vhousenow&&y - knife[i].y<-43 &&
			((x == 15 && knife[i].Left) || (x == 415 && !knife[i].Left)) && !knife[i].Short)
		{		  // alive=false;
			if (!shield) {
				if (knife[i].blood == 0 && !knife[i].broken) {
					if (knife[i].Short)
						HP--;
					else
						HP -= 2;
					knife[i].blood = 1;
					mciSendString("close hurt ", buf, strlen(buf), NULL);
					mciSendString("open hurt.wav type avivideo alias hurt", buf, strlen(buf), NULL);
					mciSendString("play hurt ", buf, strlen(buf), NULL);
				}
			}
			else {
				knife[i].broken = true;
				mciSendString("close broken ", buf, strlen(buf), NULL);
				mciSendString("open broken.wav type avivideo alias broken", buf, strlen(buf), NULL);
				mciSendString("play broken ", buf, strlen(buf), NULL);
			}
			break;
		}
	}//判断是否被刀；
	SelectObject(bufdc, ninjaup);
	BitBlt(mdc, x, y, 72, 101, bufdc, 75 + dir * 148, (2 - num2 / 6) * 101, SRCAND);
	BitBlt(mdc, x, y, 72, 101, bufdc, 1 + dir * 148, (2 - num2 / 6) * 101, SRCPAINT);
	y += 2 * vhousenow;
	H -= vhousenow;
	if (H <= 0)
		H = 0;


}

void paintstop() {
	SelectObject(bufdc, ninjastop);
	BitBlt(mdc, x, y, 72, 101, bufdc, 75 + dir * 148, (num2 / 6) * 101, SRCAND);
	BitBlt(mdc, x, y, 72, 101, bufdc, 1 + dir * 148, (num2 / 6) * 101, SRCPAINT);
	y += vhousenow;
}

void producebat() {
	int rate = 199;
	if (H > 1000)
		rate = 198;
	else if (H > 3000)
		rate = 196;
	else if (H > 6000)
		rate = 194;
	if (!easy) {
		rate = 193;
	}

	if ((1 + (int)(200.0*rand() / (RAND_MAX + 1.0)) > rate) && !power&&H>500)
	{
		for (int i = 0; i<maxbc; i++)
		{
			if (!Bat[i].exist) {
				if (1 + (int)(2.0*rand() / (RAND_MAX + 1.0))>1 && x == 415) {
					Bat[i].exist = true;
					Bat[i].x = -100;
					Bat[i].y = 1 + (int)(600 * rand() / (RAND_MAX + 1.0));
					Bat[i].y1 = 150;
					Bat[i].Left = true;
					Bat[i].bomb = false;
					Bat[i].bombed = false;
					Bat[i].num = 0;
					Bat[i].num4 = 0;
					break;
				}
				if (1 + (int)(2.0*rand() / (RAND_MAX + 1.0))>1 && x == 15) {
					Bat[i].exist = true;
					Bat[i].x = 450;
					Bat[i].y = 1 + (int)(600 * rand() / (RAND_MAX + 1.0));
					Bat[i].y1 = 0;
					Bat[i].Left = false;
					Bat[i].bomb = false;
					Bat[i].bombed = false;
					Bat[i].num = 0;
					Bat[i].num4 = 0;
					break;
				}

			}
		}

	}

	for (int i = 0; i<maxbc; i++)
	{
		if (Bat[i].exist) {
			if (
				!Bat[i].bomb && ((Bat[i].y + 30<y + 50 && Bat[i].y + 70>y && (!jump1 && !jump2) && ((Bat[i].x + 60>x&&Bat[i].x + 60<x + 80 && Bat[i].Left) || (Bat[i].x<x + 100 && Bat[i].x>x && !Bat[i].Left)))
					|| (Bat[i].y + 30<y + 150 && Bat[i].y + 70>y + 75 && (jump1 || jump2) && ((Bat[i].x + 80>x&&Bat[i].x + 60<x && Bat[i].Left) || (Bat[i].x<x + 50 && Bat[i].x>x && !Bat[i].Left))))) {
				Bat[i].bomb = true;
				mciSendString("close bomb ", buf, strlen(buf), NULL);
				mciSendString("open bomb.wav type avivideo alias bomb", buf, strlen(buf), NULL);
				mciSendString("play bomb ", buf, strlen(buf), NULL);

			}
			if (Bat[i].bomb && !Bat[i].bombed)
			{
				SelectObject(bufdc, bbomb);
				BitBlt(mdc, Bat[i].x - 25 + (int)(Bat[i].Left) * 50, Bat[i].y, 100, 98, bufdc, Bat[i].num / 3 * 100, 101, SRCAND);
				BitBlt(mdc, Bat[i].x - 25 + (int)(Bat[i].Left) * 50, Bat[i].y, 100, 98, bufdc, Bat[i].num / 3 * 100, 1, SRCPAINT);
				Bat[i].num++;
				if (Bat[i].num == 9) {
					if (!shield)
						HP -= 3;
					Bat[i].num = 0;
					Bat[i].bombed = true;
				}
			}

			if (!Bat[i].bomb)
			{
				SelectObject(bufdc, bat);
				BitBlt(mdc, Bat[i].x, Bat[i].y, 98, 72, bufdc, Bat[i].num4 / 16 * 100, Bat[i].y1 + 77, SRCAND);
				BitBlt(mdc, Bat[i].x, Bat[i].y, 98, 72, bufdc, Bat[i].num4 / 16 * 100, Bat[i].y1 + 2, SRCPAINT);

			}

			if (!Bat[i].bomb || (Bat[i].bombed&&Bat[i].bomb))
			{
				Bat[i].num4 += 4;
				Bat[i].y += vhousenow;
				if (!Bat[i].Left) {
					if (Bat[i].x > 350) {
						if (Bat[i].y < y)
							Bat[i].y += 2 * (difficult + 1);
						else
							Bat[i].y -= 2 * (difficult + 1);
						Bat[i].x -= 1;
						if (Bat[i].num4 == 48)
							Bat[i].num4 = 0;
						if (Bat[i].x <= 350)
							Bat[i].num4 = 48;
					}
					else {
						if (Bat[i].num4 < 64) {
							Bat[i].num4 -= 3;
						}
						else {
							Bat[i].x -= 10 * (difficult + 1);
							if (Bat[i].num4 == 80)
								Bat[i].num4 = 64;
						}
					}
				}
				else {
					if (Bat[i].x < 50) {
						if (Bat[i].y < y)
							Bat[i].y += 2 + difficult;
						else
							Bat[i].y -= 2 + difficult;
						Bat[i].x += 1;
						if (Bat[i].num4 == 48)
							Bat[i].num4 = 0;
						if (Bat[i].x >= 50)
							Bat[i].num4 = 48;
					}
					else {
						if (Bat[i].num4 < 64) {
							Bat[i].num4 -= 3;
						}
						else {
							Bat[i].x += 10 * (difficult + 1);
							if (Bat[i].num4 == 80)
								Bat[i].num4 = 64;
						}
					}
				}
			}
			if (Bat[i].x <= -100 || Bat[i].x>500 || Bat[i].y>800) {
				Bat[i].exist = false;
				Bat[i].num4 = 0;
				Bat[i].bombed = false;
				Bat[i].bomb = false;
			}
		}
	}
}

void produceknife() {
	int rate = 198;
	if (H > 1000)
		rate = 196;
	else if (H > 3000)
		rate = 194;
	else if (H>5000) rate = 194;
	if (!easy)
		rate = 194;

	if ((1 + (int)(200.0*rand() / (RAND_MAX + 1.0))>rate))
	{
		for (int i = 0; i<maxkc; i++)
		{
			if (!knife[i].exist&&knife[0].y >= 0 && knife[1].y >= 0 && knife[2].y >= 0 && knife[3].y >= 0 && knife[4].y >= 0
				&& knife[5].y >= 0 && knife[6].y >= 0 && knife[7].y >= 0 && knife[8].y >= 0 && knife[9].y >= 0) {
				knife[i].exist = true;
				knife[i].broken = false;
				knife[i].blood = 0;
				if(difficult)
					knife[i].y = -100;
				else
				    knife[i].y = -75;
				if (1 + (int)(10.0*rand() / (RAND_MAX + 1.0))>3 + 5 * !easy) {
					if (1 + (int)(2.0*rand() / (RAND_MAX + 1.0))>1) {
						knife[i].x = 25;
						knife[i].y1 = 70;
						knife[i].Left = true;
						knife[i].Short = true;
					}
					else {
						knife[i].x = 375;
						knife[i].y1 = 0;
						knife[i].Left = false;
						knife[i].Short = true;
					}
				}
				else {
					if ((1 + (int)(2.0*rand() / (RAND_MAX + 1.0))>1)) {
						knife[i].x = 25;
						knife[i].y1 = 105;
						knife[i].Left = true;
						knife[i].Short = false;
					}
					else {
						knife[i].x = 375;
						knife[i].y1 = 35;
						knife[i].Left = false;
						knife[i].Short = false;
					}
				}

				break;
			}
		}

	}

	for (int i = 0; i<maxkc; i++)
	{
		if (knife[i].exist)
		{
			SelectObject(bufdc, dao);
			if (!knife[i].broken) {
				BitBlt(mdc, knife[i].x, knife[i].y, 100, 35, bufdc, 1, knife[i].y1, SRCAND);
				BitBlt(mdc, knife[i].x, knife[i].y, 100, 35, bufdc, 106 + knife[i].blood * 105, knife[i].y1, SRCPAINT);
			}
			else {
				BitBlt(mdc, knife[i].x, knife[i].y, 100, 35, bufdc, 1, +knife[i].y1 + 140, SRCAND);
				BitBlt(mdc, knife[i].x, knife[i].y, 100, 35, bufdc, 106, knife[i].y1 + 140, SRCPAINT);
			}
			knife[i].y += vhousenow;
			if (knife[i].y >= 800) {
				knife[i].y = 0;
				knife[i].exist = false;
				knife[i].blood = 0;


			}
		}
	}

}

void 	produceblood() {
	//int rate = 199;
	int randnum = (1 + (int)(10.0*rand() / (RAND_MAX + 1.0)));
	if ((1 + (int)(500 * rand() / (RAND_MAX + 1.0)) > 499) && H>1000 && HP<10)
	{
		if (!Blood.exist) {
			Blood.exist = true;
			Blood.y = -50;
			Blood.x = (1 + (int)(300.0*rand() / (RAND_MAX + 1.0))) + 100;
			if (randnum>8)
				Blood.kinds = 3;
			else if (randnum>5)
				Blood.kinds = 2;
			else	   Blood.kinds = 1;
		}
	}
	if (Blood.exist)
	{
		Blood.y += vhousenow;
		SelectObject(bufdc, blood);
		BitBlt(mdc, Blood.x, Blood.y, 70, 87, bufdc, 70 * (Blood.kinds - 1), 88, SRCAND);
		BitBlt(mdc, Blood.x, Blood.y, 70, 87, bufdc, 70 * (Blood.kinds - 1), 0, SRCPAINT);

		if (Blood.y > 800) {
			Blood.exist = false;
		}
		if (Blood.exist && /*((jump1&&*/Blood.x + 35 > x + 40 && Blood.x < x + 75 && Blood.y<y + 150 && Blood.y + 40>y + 50/*)||())*/)
		{
			Blood.exist = false;
			if (HP + Blood.kinds > 10)
				HP = 10;
			else HP += Blood.kinds;
			mciSendString("close blood", buf, strlen(buf), NULL);
			mciSendString("open blood.wav type avivideo alias blood", buf, strlen(buf), NULL);
			mciSendString("play blood", buf, strlen(buf), NULL);//blood
		}

	}

}

void producefire()
{
	if ((1 + (int)(500 * rand() / (RAND_MAX + 1.0)) > 499) && !shield)
	{
		if (!Fire.exist) {
			Fire.exist = true;
			Fire.y = -50;
			Fire.x = (1 + (int)(300.0*rand() / (RAND_MAX + 1.0))) + 100;
		}
	}
	if (Fire.exist)
	{
		Fire.y += vhousenow;
		static int num = 0;
		SelectObject(bufdc, huo);
		BitBlt(mdc, Fire.x, Fire.y, 50, 50, bufdc, num / 3 * 50, 50, SRCAND);
		BitBlt(mdc, Fire.x, Fire.y, 50, 50, bufdc, num / 3 * 50, 0, SRCPAINT);
		num++;
		if (num == 18)
			num = 0;

		if (Fire.y > 800) {
			Fire.exist = false;
		}
		if (Fire.exist &&Fire.x + 35 > x + 40 && Fire.x < x + 75 && Fire.y<y + 150 && Fire.y + 40>y + 50)
		{
			Fire.exist = false;
			shield = true;
			tShield = GetTickCount();
			mciSendString("close fire ", buf, strlen(buf), NULL);
			mciSendString("open fire.wav type avivideo alias fire", buf, strlen(buf), NULL);
			mciSendString("play fire ", buf, strlen(buf), NULL);//run
		}

	}
}

void 	paintheight() {
	SelectObject(bufdc, number);
	BitBlt(mdc, 330, 10, 50, 50, bufdc, 500, 50, SRCAND);
	BitBlt(mdc, 330, 10, 50, 50, bufdc, 500, 0, SRCPAINT);
	paintnum(430, 10, H);
}

void paintshield()
{
	if (tNow>tShield)
		shieldtime = 8000 - (tNow - tShield);
	if (tNow - tShield >= 8000) {
		shield = false;

	}
	static int num = 0;
	int py = y - 25 + (int(jump1 || jump2)) * 55 - (int((jump1&&x>300) || (jump2&&x<150))) * 10 + (int((jump1&&x<45) || (jump2&&x>380))) * 20;

	SelectObject(bufdc, dun);
	BitBlt(mdc, x - 20, py, 150, 150, bufdc, num / 3 * 150, 150, SRCAND);
	BitBlt(mdc, x - 20, py, 150, 150, bufdc, num / 3 * 150, 0, SRCPAINT);
	paintnum(430, 10, H);
	num++;
	if (num == 12)
		num = 0;
}

void bgmusic()
{
	if (start&&alive) {
		mciSendString("close black", buf, strlen(buf), NULL);
		mciSendString("open bgm.wav type avivideo alias bgm", buf, strlen(buf), NULL);
		mciSendString("play bgm repeat", buf, strlen(buf), NULL);//bgM
	}
	else {
		mciSendString("close bgm", buf, strlen(buf), NULL);
		mciSendString("open black.wav type avivideo alias black", buf, strlen(buf), NULL);
		mciSendString("play black repeat", buf, strlen(buf), NULL);//bgM
	}

}

void jumpmusic() {
	TCHAR cmd[256]; // 媒体命令
	TCHAR volume[256]; // 音量(字符串)
	int nVolume; // 音量(整型)

	wsprintf(cmd, "status %s volume", "jump.wav"); // szPath 为音乐的路径
	mciSendString(cmd, volume, sizeof(volume), 0); // 获取当前音量至 volume 字符串中
	nVolume = atoi(volume); // 字符串转化为整型
	wsprintf(cmd, "setaudio %s volume to %i", "jump.wav", nVolume + 100); // 生成媒体命令，设置音量减 50
	mciSendString(cmd, "", NULL, NULL); // 执行媒体命令
	mciSendString("close jump", buf, strlen(buf), NULL);
	mciSendString("open jump.wav type avivideo alias jump", buf, strlen(buf), NULL);
	mciSendString("play jump", buf, strlen(buf), NULL);//jump
}

void runmusic()
{
}

void stickmusic()
{
	mciSendString("close stick ", buf, strlen(buf), NULL);
	mciSendString("open stick.wav type avivideo alias stick", buf, strlen(buf), NULL);
	mciSendString("play stick ", buf, strlen(buf), NULL);//run
}

void paintnum(int numx, int numy, int num)
{
	int l;
	SelectObject(bufdc, number);
	for (int i = 0; i < 6; i++) {
		l = num % 10;

		BitBlt(mdc, numx - i * 13, numy, 15, 20, bufdc, l * 50, 50, SRCAND);
		BitBlt(mdc, numx - i * 13, numy, 15, 20, bufdc, l * 50, 0, SRCPAINT);



		num = num / 10;
		if (!num)
			break;
	}
}

void    paintPH() {
	SelectObject(bufdc, bloodstrip);
	BitBlt(mdc, 50, 10, 200, 25, bufdc, 400, 25, SRCAND);
	BitBlt(mdc, 50, 10, 200, 25, bufdc, 400, 0, SRCPAINT);
	BitBlt(mdc, 50, 10, 180 * HP / 10, 25, bufdc, 200, 25, SRCAND);
	BitBlt(mdc, 50, 10, 180 * HP / 10, 25, bufdc, 200, 0, SRCPAINT);
	BitBlt(mdc, 45 + 180 * HP / 10, 10, 10, 25, bufdc, 0, 25, SRCAND);
	BitBlt(mdc, 45 + 180 * HP / 10, 10, 10, 25, bufdc, 0, 0, SRCPAINT);
	paintnum(245, 10, HP);
}

void   paintts() {
	SelectObject(bufdc, timestrip);
	BitBlt(mdc, 150, 600, 200, 15, bufdc, 400, 15, SRCAND);
	BitBlt(mdc, 150, 600, 200, 15, bufdc, 400, 0, SRCPAINT);
	BitBlt(mdc, 150, 600, (int)(180.0 * ((double)shieldtime) / 8000.0), 15, bufdc, 200, 15, SRCAND);
	BitBlt(mdc, 150, 600,(int)( 180.0 * ((double)shieldtime) / 8000.0), 15, bufdc, 200, 0, SRCPAINT);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_KEYDOWN:
		if (start)
		{
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;

			case VK_UP:
				if (stop || power) {
					stop = false;
					power = false;
					up = true;
					mciSendString("open run.wav type avivideo alias run", buf, strlen(buf), NULL);
					mciSendString("play run repeat", buf, strlen(buf), NULL);//run
				}
				break;
			case VK_DOWN:
				if (stop || power) {
					stop = false;
					power = false;
					down = true;
					mciSendString("open run.wav type avivideo alias run", buf, strlen(buf), NULL);
					mciSendString("play run repeat", buf, strlen(buf), NULL);//run
				}
				break;
			case VK_LEFT:
				if (!jump1&&dir == 1 && !jump2&&y > 100) {
					stop = false;
					up = false;
					down = false;
					power = true;
					mciSendString("close run", buf, strlen(buf), NULL);
				}
				break;
			case VK_RIGHT:
				if (!jump2&&dir == 0 && !jump1&&y > 100) {
					stop = false;
					up = false;
					down = false;
					power = true;
					mciSendString("close run", buf, strlen(buf), NULL);
				}
				break;
			}
		}
		break;
	case WM_KEYUP:
		if (start) {
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;

			case VK_UP:
				if (up) {
					up = false;
					stop = true;
					mciSendString("close run", buf, strlen(buf), NULL);
				}
				break;
			case VK_DOWN:
				if (down) {
					down = false;
					stop = true;
					mciSendString("close run", buf, strlen(buf), NULL);
				}
				break;
			case VK_LEFT:
				if (!jump1&&dir == 1 && !up && !down && !jump2&&power) {
					power = false;
					jump1 = false;
					jump2 = true;
					stop = false;
					up = false;
					down = false;
					jumpmusic();

				}
				break;
			case VK_RIGHT:
				if (!jump2&&dir == 0 && !up && !down && !jump1&&power) {
					power = false;
					jump1 = true;
					jump2 = false;
					stop = false;
					up = false;
					down = false;
					jumpmusic();

				}
				break;
			}
		}
		break;

	case WM_MOUSEMOVE:

		if ((!alive && !again) || !start)
		{
			bgmusic();
			x = LOWORD(lParam);
			if (x > 500)
				x = 500;
			else if (x < 0)
				x = 0;

			y = HIWORD(lParam);
			if (y > 800)
				y = 800;
			else if (y < 0)
				y = 0;
		}

		break;
	case WM_LBUTTONDOWN:
		if (!start) {

			if (x<100 && x>40 && y>650 && y<680) {

				start = true;
				easy = true;
				ordinary = false;
				difficult = false;
				x = 15;
				y = 300;
				if (difficult) {
					vhouse = 2;
					vhousenow = 2;
				}
				else {
					vhouse = 1;
					vhousenow = 1;
				}
				stickmusic();
				bgmusic();

			}
			if (x<200 && x>130 && y>650 && y<680) {
				start = true;
				easy = false;
				ordinary = true;
				difficult = false;
				x = 15;
				y = 300;
				if (difficult) {
					vhouse = 2;
					vhousenow = 2;
				}
				else {
					vhouse = 1;
					vhousenow = 1;
				}
				stickmusic();
				bgmusic();

			}
			if (x<300 && x>230 && y>650 && y<680) {
				start = true;
				easy = false;
				ordinary = false;
				difficult = true;
				x = 15;
				y = 300;
				if (difficult) {
					vhouse = 2;
					vhousenow = 2;
				}
				else {
					vhouse = 1;
					vhousenow = 1;
				}
				stickmusic();
				bgmusic();

			}
			if (x<470 && x>330 && y>650 && y<680) {
				exit(0);
				stickmusic();
			}

		}
		if (!alive && !again) {
			if (x < 380 && x>120 && y < 500 && y>430)
				again = true;
			if (x < 320 && x>170 && y < 640 && y>580)
				close = true;

		}
		break;
	case WM_DESTROY:
		DeleteDC(mdc);
		DeleteDC(bufdc);
		DeleteObject(ninjaup);
		DeleteObject(ninjajump);
		DeleteObject(ninjastop);
		DeleteObject(gover);
		DeleteObject(dirc);
		DeleteObject(dirc);
		DeleteObject(np);
		DeleteObject(dao);
		DeleteObject(ins);
		DeleteObject(bat);
		DeleteObject(house1);
		DeleteObject(bg);
		ReleaseDC(hWnd, hdc);

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

