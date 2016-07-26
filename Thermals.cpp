/*
 *		Эта программа создана Джефом Молофи (Jeff Molofee) в 2000 г.
 *		Модифицировано Shawn T. для поддержки параметров (%3.2f, num).
 *		ОГРОМНОЕ спасибо Фредерику Эхолзу (Fredric Echols) за улучшение
 *		и оптимизацию базового кода, за увеличение его гибкости!
 *		Если вы нашли эту программу полезной, пожалуйста, дайте мне знать.
 *		Посетите мой сайт nehe.gamedev.net
 */

#include <windows.h>		// Заголовочный файл для Windows
#include <stdio.h>			// Заголовочный файл стандартного ввода/вывода
#include <gl\gl.h>			// Заголовочный файл библиотеки OpenGL32
#include <gl\glu.h>			// Заголовочный файл библиотеки GLu32
#include <gl\glaux.h>		// Заголовочный файл библиотеки GLaux
#include <math.h>     // Заголовочный файл для математической библиотеки ( НОВОЕ )
#include <stdarg.h>   // Заголовочный файл для функций для работы с переменным
#include <conio.h>
#include <cstdlib>
#include <iostream>

using namespace std;
#define PI (3.141592653589793)   

//**************************************************************************************************************
float rx=0;
float ry=0;
float rz=0;
float currentTime=0;
float oldTime=0;
float vektor[6];
float quan=0;;
float zin;

double p[4][4] = {
                 {0,20,80,128}, 
                 {0,0,50,80}, 
                 {0,0,0,20}, 
                 {0,0,0,0}
                 };
double temp[4][4];
double in[4][4];
double data[6][6];
double lin[4];
class thermal
{
public: 
double Vmax;
double Vmin;
double Xmax;
double Ymax;
double Xmin;
double Ymin;

double X;
double Y;
double Z;

double R;
double wind;
double naklon;
double H;
double Time;


double Xn(double chast)
{
return ((Xmax-Xmin)/chast);
}


double Yn(double chast)
{
return ((Ymax-Ymin)/chast);
}





};
thermal termik;


class  col
{
public: double color;
public: double R,G,B;

public: void getcol(double color)
       {
      if (color<0) color=0;
      
      if (color>128) color=128;
    
      if (color>=0 && color<=32) {R=0.0f; G=color/32; B=1.0f;}
    
      if (color>32 && color<=64) {R=0.0f; G=1.0f; B=(64-color)/32;}
      
      if (color>64 && color<=96) {R=(color-64)/32; G=1.0f; B=0.0f;}
      
      if (color>96 && color<=128) {R=1.0f; G=(128-color)/32; B=0.0f;}
      }
};
col rgb;



double cubicInterpolate (double p[4], double x) {
	return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

double bicubicInterpolate (double p[4][4], double x, double y) {
	double arr[4];
	arr[0] = cubicInterpolate(p[0], y);
	arr[1] = cubicInterpolate(p[1], y);
	arr[2] = cubicInterpolate(p[2], y);
	arr[3] = cubicInterpolate(p[3], y);
	return cubicInterpolate(arr, x);
}

double tricubicInterpolate (double p[4][4][4], double x, double y, double z) {
	double arr[4];
	arr[0] = bicubicInterpolate(p[0], y, z);
	arr[1] = bicubicInterpolate(p[1], y, z);
	arr[2] = bicubicInterpolate(p[2], y, z);
	arr[3] = bicubicInterpolate(p[3], y, z);
	return cubicInterpolate(arr, x);
}



class CachedBicubicInterpolator
{
	private: double a00, a01, a02, a03;
	private: double a10, a11, a12, a13;
	private: double a20, a21, a22, a23;
	private: double a30, a31, a32, a33;

	public: void updateCoefficients (double p[4][4]) {
		a00 = p[1][1];
		a01 = -.5*p[1][0] + .5*p[1][2];
		a02 = p[1][0] - 2.5*p[1][1] + 2*p[1][2] - .5*p[1][3];
		a03 = -.5*p[1][0] + 1.5*p[1][1] - 1.5*p[1][2] + .5*p[1][3];
		a10 = -.5*p[0][1] + .5*p[2][1];
		a11 = .25*p[0][0] - .25*p[0][2] - .25*p[2][0] + .25*p[2][2];
		a12 = -.5*p[0][0] + 1.25*p[0][1] - p[0][2] + .25*p[0][3] + .5*p[2][0] - 1.25*p[2][1] + p[2][2] - .25*p[2][3];
		a13 = .25*p[0][0] - .75*p[0][1] + .75*p[0][2] - .25*p[0][3] - .25*p[2][0] + .75*p[2][1] - .75*p[2][2] + .25*p[2][3];
		a20 = p[0][1] - 2.5*p[1][1] + 2*p[2][1] - .5*p[3][1];
		a21 = -.5*p[0][0] + .5*p[0][2] + 1.25*p[1][0] - 1.25*p[1][2] - p[2][0] + p[2][2] + .25*p[3][0] - .25*p[3][2];
		a22 = p[0][0] - 2.5*p[0][1] + 2*p[0][2] - .5*p[0][3] - 2.5*p[1][0] + 6.25*p[1][1] - 5*p[1][2] + 1.25*p[1][3] + 2*p[2][0] - 5*p[2][1] + 4*p[2][2] - p[2][3] - .5*p[3][0] + 1.25*p[3][1] - p[3][2] + .25*p[3][3];
		a23 = -.5*p[0][0] + 1.5*p[0][1] - 1.5*p[0][2] + .5*p[0][3] + 1.25*p[1][0] - 3.75*p[1][1] + 3.75*p[1][2] - 1.25*p[1][3] - p[2][0] + 3*p[2][1] - 3*p[2][2] + p[2][3] + .25*p[3][0] - .75*p[3][1] + .75*p[3][2] - .25*p[3][3];
		a30 = -.5*p[0][1] + 1.5*p[1][1] - 1.5*p[2][1] + .5*p[3][1];
		a31 = .25*p[0][0] - .25*p[0][2] - .75*p[1][0] + .75*p[1][2] + .75*p[2][0] - .75*p[2][2] - .25*p[3][0] + .25*p[3][2];
		a32 = -.5*p[0][0] + 1.25*p[0][1] - p[0][2] + .25*p[0][3] + 1.5*p[1][0] - 3.75*p[1][1] + 3*p[1][2] - .75*p[1][3] - 1.5*p[2][0] + 3.75*p[2][1] - 3*p[2][2] + .75*p[2][3] + .5*p[3][0] - 1.25*p[3][1] + p[3][2] - .25*p[3][3];
		a33 = .25*p[0][0] - .75*p[0][1] + .75*p[0][2] - .25*p[0][3] - .75*p[1][0] + 2.25*p[1][1] - 2.25*p[1][2] + .75*p[1][3] + .75*p[2][0] - 2.25*p[2][1] + 2.25*p[2][2] - .75*p[2][3] - .25*p[3][0] + .75*p[3][1] - .75*p[3][2] + .25*p[3][3];
	}

	public: double getValue (double x, double y) {
		double x2 = x * x;
		double x3 = x2 * x;
		double y2 = y * y;
		double y3 = y2 * y;

		return (a00 + a01 * y + a02 * y2 + a03 * y3) +
		       (a10 + a11 * y + a12 * y2 + a13 * y3) * x +
		       (a20 + a21 * y + a22 * y2 + a23 * y3) * x2 +
		       (a30 + a31 * y + a32 * y2 + a33 * y3) * x3;
	}
};
CachedBicubicInterpolator Bicubic;
//**************************************************************************************************************


                      //  количеством аргументов ( НОВОЕ )

HDC			hDC=NULL;		// Частный контекст GDI устройства
HGLRC		hRC=NULL;		// Постоянный контекст отображения
HWND		hWnd=NULL;		// Содержит хэндл нашего окна
HINSTANCE	hInstance;		// Содержит экземпляр приложения

bool	keys[256];			// Массив применяемый для подпрограммы клавиатуры
bool	active=TRUE;		// Флаг "Активное" окна. Устанавливается истинным (TRUE) по умолчанию.
bool	fullscreen=TRUE;	// Флаг "На полный экран". Устанавливается в полноэкранный режим по умолчанию.

GLuint  base;      // база списка отображения для фонта
GLfloat  cnt1;     // Первый счетчик для передвижения и закрашивания текста
GLfloat  cnt2;     // Второй счетчик для передвижения и закрашивания текста

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Объявление для WndProc


GLvoid BuildFont(GLvoid)  // Построение нашего растрового шрифта
{
  HFONT  font;            // Идентификатор фонта

  base = glGenLists(96);  // Выделим место для 96 символов ( НОВОЕ )

  font = CreateFont(  -18,        // Высота фонта ( НОВОЕ )
        0,        // Ширина фонта
        0,        // Угол отношения
        0,        // Угол Наклона
        FW_BOLD,      // Ширина шрифта
        FALSE,        // Курсив
        FALSE,        // Подчеркивание
        FALSE,        // Перечеркивание
        ANSI_CHARSET,      // Идентификатор набора символов
        OUT_TT_PRECIS,      // Точность вывода
        CLIP_DEFAULT_PRECIS,    // Точность отсечения
        ANTIALIASED_QUALITY,    // Качество вывода
        FF_DONTCARE|DEFAULT_PITCH,  // Семейство и Шаг
        "Courier New");      // Имя шрифта
  SelectObject(hDC, font);        // Выбрать шрифт, созданный нами ( НОВОЕ )

  wglUseFontBitmaps(hDC, 32, 96, base); // Построить 96 символов начиная с пробела ( НОВОЕ )
}

GLvoid KillFont(GLvoid)            // Удаление шрифта
{
   glDeleteLists(base, 96);        // Удаление всех 96 списков отображения ( НОВОЕ )
}


GLvoid glPrint(const char *fmt, ...)        // Заказная функция "Печати" GL
{
  char    text[256];      // Место для нашей строки
  va_list    ap;          // Указатель на список аргументов
  if (fmt == NULL)     // Если нет текста
    return;            // Ничего не делать

  va_start(ap, fmt);           // Разбор строки переменных
      vsprintf(text, fmt, ap); // И конвертирование символов в реальные коды
  va_end(ap);                  // Результат помещается в строку
  glPushAttrib(GL_LIST_BIT);      // Протолкнуть биты списка отображения ( НОВОЕ )
  glListBase(base - 32);          // Задать базу символа в 32 ( НОВОЕ )
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); // Текст списками отображения ( НОВОЕ )
  glPopAttrib(); // Возврат битов списка отображения ( НОВОЕ )
}

int InitGL(GLvoid)            // Все начальные настройки OpenGL здесь
{
  glShadeModel(GL_SMOOTH);    // Разрешить плавное затенение
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Черный фон
  glClearDepth(1.0f);         // Установка буфера глубины
  glEnable(GL_DEPTH_TEST);    // Разрешение теста глубины
  glDepthFunc(GL_LEQUAL);     // Тип теста глубины
  // Действительно хорошие вычисления перспективы
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  BuildFont();            // Построить шрифт

  return TRUE;            // Инициализация окончена
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Изменить размеры и проинициализировать окно GL
{
	if (height==0)										// Предотвращаем деление на 0
	{
		height=1;										// Приравниваем высоту единице
	}

	glViewport(0,0,width,height);						// Обновляем текущий порт отображения

	glMatrixMode(GL_PROJECTION);						// Выбираем матрицу проекции
	glLoadIdentity();									// Устанавливаем матрицу проекции

	// Вычисляем соотношение сторон окна
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Выбираем матрицу модели отображения
	glLoadIdentity();									// Устанавливаем матрицу модели отображения
}

int DrawGLScene(GLvoid) // Здесь мы будем рисовать все
{
    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка экран и буфера глубины
  glLoadIdentity(); // Сброс просмотра

glTranslatef(-7.4f,-1.5f,-12.0f); // Передвижение на одну единицу вглубь
glRotatef(0.0f, 1.0f, 0.0f, 0.0f); 
//glRotatef(rx, 0.0f, 1.0f, 0.0f); 
rx=rx+1;
if (rx==360) rx=0;



//******************************************************
termik.Vmax=0;
termik.Vmin=128;

for (int x=0;x<6;x++)
     {
      for (int y=0;y<6;y++)
          {
          if (x>0 && x<5 && y>0 && y<5) data[x][y]=p[x-1][y-1];
          
                  
          if (x==0 && y==0 ) data[x][y]=(data[x+1][y+1]+data[x+2][y+2])/2;
          if (x==5 && y==5 ) data[x][y]=(data[x-1][y-1]+data[x-2][y-2])/2;
          if (x==0 && y==5 ) data[x][y]=(data[x+1][y-1]+data[x+2][y-2])/2;
          if (x==5 && y==0 ) data[x][y]=(data[x-1][y+1]+data[x-2][y+2])/2;
          
          if (x==0 && y>0 && y<5 ) data[x][y]=(data[x+1][y]+data[x+2][y])/2;
          if (y==0 && x>0 && x<5 ) data[x][y]=(data[x][y+2]+data[x][y+2])/2;

          if (x==5 && y>0 && y<5 ) data[x][y]=(data[x-1][y]+data[x-2][y])/2;
          if (y==5 && x>0 && x<5 ) data[x][y]=(data[x][y-1]+data[x][y-2])/2;
     
          }
     }
       

for (int i=0;i<=2;i++)
{ 
  for (int o=0;o<=2;o++)
  {   
      for (int x=0;x<=3;x++)
      {
       for (int y=0;y<=3;y++)
           {
           in[x][y]=data[x+i][y+o];
           }
       }
    
    Bicubic.updateCoefficients(in);
    glBegin(GL_POINTS);
    for (double x=0;x<1;x=x+0.006)
    {
     for (double y=0;y<1;y=y+0.006)
     {
      float color= Bicubic.getValue (x,y);
      if (termik.Vmax<color) 
      {
      termik.Vmax=color;
      termik.Xmax=x+i;
      termik.Ymax=y+o;
      }
      if (termik.Vmin>color && color>10) 
      
      {
      termik.Vmin=color;
      termik.Xmin=x+i;
      termik.Ymin=y+o;
      }
      
      rgb.getcol(color);
      glColor3f(rgb.R,rgb.G,rgb.B);
      glVertex3f(x*2+i*2,y*2+o*2,0);
     }
    }
    glEnd();
   }
}


//******************************************************
glColor3f(1.0f,1.0f,1.0f);
for (int x=0;x<4;x++)
{
glRasterPos3f(x*2-0.2, -0.2f,0.0f);
glPrint("%0.0f",(float)x-1);  // Печать текста GL на экран
}
glRasterPos3f(6.1,-0.05,0.0f);
glPrint("X");

glBegin(GL_LINES);
     glVertex3d(0, 0, 0); 
     glVertex3d(6.0, 0.0, 0.0);
glEnd();

glBegin(GL_LINES);
     glVertex3d(0, 2.0, 0); 
     glVertex3d(6.0, 2.0, 0.0);
glEnd();


glBegin(GL_LINES);
     glVertex3d(0, 4.0, 0); 
     glVertex3d(6.0, 4.0, 0.0);
glEnd();


glBegin(GL_LINES);
     glVertex3d(0, 6.0, 0); 
     glVertex3d(6.0, 6.0, 0.0);
glEnd();



for (int y=0;y<4;y++)
{
glRasterPos3f(-0.2, y*2-0.2,0.0f);
glPrint("%0.0f",(float)y-1);  // Печать текста GL на экран
}
glRasterPos3f(-0.1,6.1,0.0f);
glPrint("Y");

glBegin(GL_LINES);
     glVertex3d(0, 0, 0); 
     glVertex3d(0.0, 6.0, 0.0);
glEnd();

glBegin(GL_LINES);
     glVertex3d(2, 0, 0); 
     glVertex3d(2.0, 6.0, 0.0);
glEnd();


glBegin(GL_LINES);
     glVertex3d(4, 0, 0); 
     glVertex3d(4.0, 6.0, 0.0);
glEnd();


glBegin(GL_LINES);
     glVertex3d(6, 0, 0); 
     glVertex3d(6.0, 6.0, 0.0);
glEnd();


glLineWidth(3.0);
for (int i=0;i<128;i++)
{
    rgb.getcol(i);
    glBegin(GL_LINES);
    glColor3f(rgb.R,rgb.G,rgb.B);
     glVertex3d((float)i/30, -0.3, 0); 
     glVertex3d((float)i/30, -0.5, 0.0);
     glEnd();
}
glLineWidth(1.0);
  glColor3f(1,1,1);
for (int y=0;y<=12;y++)
{
glRasterPos3f((float)y/3, -0.7,0.0f);
glPrint("%0.0f",(float)y);  // Печать текста GL на экран
}
glRasterPos3f(4.4f,-0.7,0.0f);
glPrint("[m/s]");
//******************************************************






//******************************************************
termik.R=sqrt((termik.Xmax-termik.Xmin)*(termik.Xmax-termik.Xmin) + (termik.Ymax-termik.Ymin)*(termik.Ymax-termik.Ymin));
termik.R=termik.R;

//******************************************************

//******************************************************
glTranslatef(0.0f,-1.0f,0.0f);

glColor3f(1.0f,1.0f,1.0f);
glBegin(GL_LINES);
     glVertex3d(0, 0, 0); 
     glVertex3d(6.0, 0.0, 0.0);
     
     glVertex3d(6.0, 0, 0); 
     glVertex3d(6.0, -1.5, 0.0);
     
     glVertex3d(6.0, -1.5, 0.0);
     glVertex3d(0.0, -1.5, 0.0);
     
     glVertex3d(0.0, -1.5, 0.0);
     glVertex3d(0, 0, 0);
     glEnd();
glTranslatef(0.0f,1.0f,0.0f);


glTranslatef(0.1f,-1.0f,0.0f);
glColor3f(1.0f,1.0f,1.0f);
  // Позиционирование текста на экране
glRasterPos2f(0.0f, -0.2f);
glPrint("Maksimalus srauto greitis m/s% 5.2f",termik.Vmax/10 );  // Печать текста GL на экран
glRasterPos2f(0.0f, -0.4f);
glPrint("Minimalus srauto greitis m/s% 5.2f",termik.Vmin/10 );  // Печать текста GL на экран
glRasterPos2f(0.0f, -0.6f);
glPrint("Minimumo X koordinate %5.2f",termik.Xmin );  // Печать текста GL на экран
glRasterPos2f(0.0f, -0.8f);
glPrint("Minimumo Y koordinate  %5.2f",termik.Ymin );  // Печать текста GL на экран
glRasterPos2f(0.0f, -1.0f);
glPrint("Maksimumo X koordinate  %5.2f",termik.Xmax );  // Печать текста GL на экран
glRasterPos2f(0.0f, -1.2f);
glPrint("Maksimumo Y koordinate  %5.2f",termik.Ymax );  // Печать текста GL на экран
glRasterPos2f(0.0f, -1.4f);
glPrint("Terminio srauto skersmuo [m]%5.2f",(termik.R*600)/5);  // Печать текста GL на экран
glTranslatef(-0.1f,1.0f,0.0f);
//******************************************************

//

//

//******************************************************
glTranslatef(6.3f,-4.0f,0.0f);
glBegin(GL_LINES);
     glVertex3d(0, 0, 0); 
     glVertex3d(6.0, 0.0, 0.0);
     
     glVertex3d(6.0, 0, 0); 
     glVertex3d(6.0, -1.5, 0.0);
     
     glVertex3d(6.0, -1.5, 0.0);
     glVertex3d(0.0, -1.5, 0.0);
     
     glVertex3d(0.0, -1.5, 0.0);
     glVertex3d(0, 0, 0);
     glEnd();
     
     termik.wind=240;
     termik.naklon=10;
     termik.H=2000;
     termik.Z=0;
     
glRasterPos2f(0.1f, -0.2f);
glPrint("Terminio srauto polinkis [Laip] %5.0f",termik.naklon );  // Печать текста GL на экран
glRasterPos2f(0.1f, -0.4f);
glPrint("Vejo kriptis [Laip]  %5.0f", termik.wind );  // Печать текста GL на экран
glRasterPos2f(0.1f, -0.6f);
glPrint("Terminio srauto aukstis [m] %5.0f",termik.H );  // Печать текста GL на экран


glTranslatef(-6.3f,0.5f,0.0f);


//******************************************************
glColor3f(1.0f,1.0f,1.0f);
float avgTime=0;
avgTime = currentTime-oldTime;
oldTime=currentTime;
currentTime = clock ();  
avgTime = sqrt((avgTime*avgTime+(currentTime-oldTime)*(currentTime-oldTime))/2);
glRasterPos3f(8.0f, 1.0f,0.0f);
glPrint("Velinimo laikas [ms] %1.0f",avgTime);
//******************************************************


float vincr=0.3;
float rincr=0.1;
float rdiskr=1;
float dH;
float dR;

glRasterPos3f(8.0f, 0.8f,0.0f);
glPrint("Interpoliacijos apskritimo diskretiskumas %1.0f",rdiskr/360);

glRasterPos3f(8.0f, 1.2f,0.0f);
glPrint("Interpoliacijos tasku kiekis %1.0f",quan);

glRasterPos3f(8.0f, 1.4f,0.0f);
glPrint("Apskr kiekis %1.0f",quan/360);

dH=termik.H/(termik.H/600/vincr);
glRasterPos3f(8.0f, 1.6f,0.0f);
glPrint("Atstumas tarp elementu [m] %1.0f",dH);

dR=(float)termik.R/((float)termik.R/600/rincr);
glRasterPos3f(8.0f, 1.8f,0.0f);
glPrint("Atstumas tarp apskritimu [m] %1.0f",dR);

glRasterPos3f(8.0f, 2.0f,0.0f);
glPrint("Kylancio oro srauto aukstis [m] %1.0f",termik.H);

glRasterPos3f(8.0f, 2.2f,0.0f);
glPrint("Modeliavimo laikas [ms] %1.0f",termik.Time);



glTranslatef(10.0f,7.0f,0.0f);
glBegin(GL_POINTS);
float vysota=1;
 for (float radius=2;radius>0;radius=radius-0.3)
 {
rgb.getcol(cubicInterpolate(lin,1-radius/termik.R));  
  for (float angle=0;angle<90;angle=angle+0.1)
  {
  glColor3f(rgb.R,rgb.G,rgb.B);
  glVertex3f(radius*sin(angle*PI/180.0)+vysota*tan(termik.naklon*PI/180.0)*cos(termik.wind*PI/180.0),(radius)*cos(angle*PI/180.0)+vysota*tan(termik.naklon*PI/180.0)*sin(termik.wind*PI/180.0),vysota);
  quan++;
  }
 }
 termik.naklon=termik.naklon+5*vincr;
glEnd();
glTranslatef(-10.0f,-6.5f,0.0f);


glTranslatef(10.0f,2.5f,0.0f);
glRotatef(90, 1.0f, 0.0f, 0.0f); 
//glRotatef(rx+90, 0.0f, 0.0f, 1.0f);
glColor3f(1.0f,1.0f,1.0f);

glBegin(GL_LINES);
     glVertex3d(0, 0, 0); 
     glVertex3d(2.0, 0.0, 0.0);
     glVertex3d(0, 0, 0); 
     glVertex3d(0.0, -2.0, 0.0);
     glVertex3d(0, 0, 0); 
     glVertex3d(0.0, 0.0, -2.0);
     glEnd();



termik.X=5*termik.Time/1000;
termik.Y=1*termik.Time/1000;
glRasterPos3f(2.0f, 0.0f,0.0f);
glPrint("x %1.0f",termik.X );  // Печать текста GL на экран
glRasterPos3f(0.0f, -2.0f,0.0f);
glPrint("y %1.0f",termik.Y );  // Печать текста GL на экран
glRasterPos3f(0.0f, 0.0f,-2.0f);
glPrint("z 0" );  // Печать текста GL на экран   



//******************************************************     


   
     

     
//******************************************************     

lin[0]=0;
lin[1]=termik.Vmin;
lin[2]=termik.Vmax;
lin[1]=termik.Vmin;

quan=0;
glBegin(GL_POINTS);
float Rconst=termik.R/2;
termik.Time=clock();//termik.R=(Rconst-vysota/5);   
 Rconst=Rconst-sin(termik.Time/1000);
 lin[2]=termik.Vmax-100*sin(termik.Time/1000);;
for (float vysota=0;vysota>-termik.H/600;vysota=vysota-vincr)
{
 termik.R=(Rconst+(cos(Rconst+vysota/2)/1))/2;

  for (float radius=termik.R;radius>0;radius=radius-rincr)
 {
rgb.getcol(cubicInterpolate(lin,1-radius/termik.R));  
  for (float angle=0;angle<360;angle=angle+rdiskr)
  {
  glColor3f(rgb.R,rgb.G,rgb.B);
  glVertex3f(radius*sin(angle*PI/180.0)+vysota*tan(termik.naklon*PI/180.0)*cos(termik.wind*PI/180.0),(radius)*cos(angle*PI/180.0)+vysota*tan(termik.naklon*PI/180.0)*sin(termik.wind*PI/180.0),vysota);
  quan++;
  }
 }
 termik.naklon=termik.naklon+5*vincr+cos(termik.Time/1000);
}
glEnd();





//******************************************************


  return TRUE;   // Все отлично
}


GLvoid KillGLWindow(GLvoid)								// Должным образом уничтожаем окно
{
	if (fullscreen)										// Мы в полно экранном режииме?
	{
		ChangeDisplaySettings(NULL,0);					// Если да, то перключаемся на рабочий стол
		ShowCursor(TRUE);								// Показываем курсор мыши
	}

	if (hRC)											// Контекст отображения уже есть?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Мы можем освободить RC и DC контексты?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Мы можем удалить RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Устанавливаем RC в NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Мы можем высвободить контекст DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Устанавливаем DC в NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Мы можем уничтожить окно?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Устанавливаем hWnd в NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Можем разрегистрировать класс
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Устанавливаем hInstance в NULL
	}
    KillFont();            // Уничтожить шрифт
}

/*	Эта часть программы создает наше окно OpenGL.  Параметры:							*
 *	title			- Заголовок, отображаемый наверху окна								*
 *	width			- Ширина GL окна или полноэкранного режима							*
 *	height			- Высота GL окна или полноэкранного режима							*
 *	bits			- Количество бит на цвет (8/16/24/32)								*
 *	fullscreenflag	- Использование полноэкранного (TRUE) или оконного(FALSE) режимов	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Содержит результаты подбора глубины цвета
	WNDCLASS	wc;						// Структура классов Windows
	DWORD		dwExStyle;				// Расширенный стиль окна
	DWORD		dwStyle;				// Стиль окна
	RECT		WindowRect;				// Получает значения верхнего левого и нижнего правого углов прямоугольника
	WindowRect.left=(long)0;			// Устанавливает значение лево (Left) в 0
	WindowRect.right=(long)width;		// Устанавливает значение право (Right) в требуемую ширину (Width)
	WindowRect.top=(long)0;				// Устанавливает значение верх (Top) в 0
	WindowRect.bottom=(long)height;		// Устанавливает значение низ (Bottom) в требуемую высоту (Height)

	fullscreen=fullscreenflag;			// Устанавливаем глобальный флвг Fullscreen

	hInstance			= GetModuleHandle(NULL);				// Захватываем Instance для нашего окна
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Перерисовываем по размеру, и получаем DC для окна.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// Нет дополнительных данных окна
	wc.cbWndExtra		= 0;									// Нет дополнительных данных окна
	wc.hInstance		= hInstance;							// Установим Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Згрузим иконку по умолчанию
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Згрузим стрелку курсора
	wc.hbrBackground	= NULL;									// Фон для GL не требуется
	wc.lpszMenuName		= NULL;									// Нам не нужны меню
	wc.lpszClassName	= "OpenGL";								// Установим имя класса

	if (!RegisterClass(&wc))									// Попытаемся зарегистрировать класс окна
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Возращаем FALSE
	}
	
	if (fullscreen)												// Попытаться включить полноеэкранный режим?
	{
		DEVMODE dmScreenSettings;								// Режим устройства
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Убедимся, что памать очищена
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Размер структуры devmode
		dmScreenSettings.dmPelsWidth	= width;				// Выбрана ширина экрана
		dmScreenSettings.dmPelsHeight	= height;				// Выбрана высота экрана
		dmScreenSettings.dmBitsPerPel	= bits;					// Выбрано количество бит на пиксель
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Попытаемся установить выбранный режим и получить резутьтаты.  К седению: CDS_FULLSCREEN избавляется от кнопки стариGets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// Если режиим не включился, предложим две возможности. Выйти или использовать оконный режим.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Выбран оконный режим.  Fullscreen = FALSE
			}
			else
			{
				// Показать сообщение, что приложение закончило работу.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Возвращаем FALSE
			}
		}
	}

	if (fullscreen)												// Так мы в полноэкранном режиме?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Расширенный стиль окна
		dwStyle=WS_POPUP;										// Стиль окна
		ShowCursor(FALSE);										// Скрыть курсор мыши
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Расширенный стиль окна
		dwStyle=WS_OVERLAPPEDWINDOW;							// Стиль окна
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Подстроить окно, чтобы оно соответствовало требуемому размеру

	// Создать окно
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Расширенный стиль для окна
								"OpenGL",							// Наименование класса
								title,								// Заголовок окна
								dwStyle |							// Определенный стиль окна
								WS_CLIPSIBLINGS |					// Требуемый стиль окна
								WS_CLIPCHILDREN,					// Требуемый стиль окна
								0, 0,								// Местоположение окна
								WindowRect.right-WindowRect.left,	// Вычисление ширины окна
								WindowRect.bottom-WindowRect.top,	// Вычисление высоты окна
								NULL,								// Нет родительского окна
								NULL,								// Нет меню
								hInstance,							// Instance
								NULL)))								// Не посылать сообщение по WM_CREATE
	{
		KillGLWindow();								// Инициализация дисплея
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Вернуть FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd сообщает Windows какие параметры мы хотим
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Размер дескриптора формата пикселей
		1,											// Номер версии
		PFD_DRAW_TO_WINDOW |						// Формат должен поддерживать окно
		PFD_SUPPORT_OPENGL |						// Формат должен поддерживать OpenGL
		PFD_DOUBLEBUFFER,							// Должна поддерживаться двойная буферизация
		PFD_TYPE_RGBA,								// Запрос RGBA формата
		bits,										// Выбор глубины цвета
		0, 0, 0, 0, 0, 0,							// Биты цвета игнорируются
		0,											// Нет альфа буферизации
		0,											// Бит сдвига игнорируется
		0,											// Нет буфера накопления
		0, 0, 0, 0,									// Биты накопления игнорируются
		16,											// 16битный Z-бувер (Буфер глубины)  
		0,											// Нет буфера трафарета (stencil buffer)
		0,											// Нет вспомогательного буфера
		PFD_MAIN_PLANE,								// Главная плоскость рисования
		0,											// Зарезервировано
		0, 0, 0										// Слой масок игнорируется
	};
	
	if (!(hDC=GetDC(hWnd)))							// Мы получили контекст устройства?
	{
		KillGLWindow();								// Инициализация дисплея
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Вернуть FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Windows нашла соответствующий формат пикселя?
	{
		KillGLWindow();								// Инициализация дисплея
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Вернуть FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Мы можем установить формат пикселя?
	{
		KillGLWindow();								// Инициализация дисплея
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Вернуть FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Мы можем получить контекст изображения?
	{
		KillGLWindow();								// Инициализация дисплея
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Вернуть FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Пытаемся активировать контекст изображения
	{
		KillGLWindow();								// Инициализация дисплея
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Вернуть FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Показать окно
	SetForegroundWindow(hWnd);						// Слегка увеличим приоритет
	SetFocus(hWnd);									// Устанавливаем фокус клавииатуры на окно
	ReSizeGLScene(width, height);					// Устанавливаем наше GL окно с перспективой

	if (!InitGL())									// Инициализируем наше GL окно
	{
		KillGLWindow();								// Инициализация дисплея
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Возращает FALSE
	}

	return TRUE;									// Успешное завершение инициализациии
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Хэндл для этого окна
							UINT	uMsg,			// Сообщение для этого окна
							WPARAM	wParam,			// Дополнительная информация сообщения
							LPARAM	lParam)			// Дополнительная информация сообщения
{
	switch (uMsg)									// Проверим сообщения окна
	{
		case WM_ACTIVATE:							// Наблюдаем за сообщением об активизации окна
		{
			if (!HIWORD(wParam))					// Проверим состояние минимизациии
			{
				active=TRUE;						// Программа активна
			}
			else
			{
				active=FALSE;						// Программа больше не активна
			}

			return 0;								// Вернууться к циклу сообщения
		}

		case WM_SYSCOMMAND:							// Перехватваем системную команду
		{
			switch (wParam)							// Проверка ывбора системы
			{
				case SC_SCREENSAVE:					// Пытаеттся включиться скринсэйвер?
				case SC_MONITORPOWER:				// Монитор пытается переключитться в режим сохранения энергии?
				return 0;							// Не давать этому случиться
			}
			break;									// Выход
		}

		case WM_CLOSE:								// Мы получили сообщение о закрытии программы?
		{
			PostQuitMessage(0);						// Послать сообщение о выходе
			return 0;								// Вернуться
		}

		case WM_KEYDOWN:							// Клавиша была нажата?
		{
			keys[wParam] = TRUE;					// Если это так пометим это TRUE
			return 0;								// Вернуться
		}

		case WM_KEYUP:								// Клавиша была отпущена?
		{
			keys[wParam] = FALSE;					// Если это так пометим это FALSE
			return 0;								// Вернуться
		}

		case WM_SIZE:								// Изменились окна OpenGL
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=ширина, HiWord=высота
			return 0;								// Вернуться
		}
	}

	// Пересылаем все прочие сообщения в DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Предыдущий Instance
					LPSTR		lpCmdLine,			// Параметры командной строки
					int			nCmdShow)			// Показать состояние окна
{
	MSG		msg;									// Структура сообщения окна
	BOOL	done=FALSE;								// Булевская переменная выхода из цикла
	
/////////////////////////////////////////////////////////////////////////////////////////////////

double temp[4][4];
for (int y=0;y<4;y++)
{
for (int i=0;i<4;i++)
 {
 temp[i][y]=p[i][y];
 }
}

for (int i=0;i<4;i++)
 {       
for (int y=0;y<4;y++)
{
 p[y][i]=temp[i][y];
 }
}


////////////////////////////////////////////////////////////////////////////////////////////////




	// Запросим пользователя какой режим отображения он предпочитает
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Оконный режим
	}

	// Создадим наше окно OpenGL
	if (!CreateGLWindow("Modelirovanie",1200,768,32,fullscreen))
	{
		return 0;									// Выходим если окно не было создано
	}

	while(!done)									// Цикл, который продолжается пока done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Есть ожидаемое сообщение?
		{
			if (msg.message==WM_QUIT)				// Мы получили сообщение о выходе?
			{
				done=TRUE;							// Если так done=TRUE
			}
			else									// Если нет,продолжаем работать с сообщениями окна
			{
				TranslateMessage(&msg);				// Переводим сообщение
				DispatchMessage(&msg);				// Отсылаем сообщение
			}
		}
		else										// Если сообщений нет
		{
			// Рисуем сцену.  Ожидаем нажатия кнопки ESC и сообщения о выходе от DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Активно?  Было получено сообщение о выходе?
			{
				done=TRUE;							// ESC или DrawGLScene просигналили "выход"
			}
			else									// Не время выходить, обновляем экран
			{
                SwapBuffers(hDC);					// Переключаем буферы (Double Buffering)
				if (keys[VK_F1])						// Была нажата кнопка F1?
				{
					keys[VK_F1]=FALSE;					// Если так - установим значение FALSE
					KillGLWindow();						// Закроем текущее окно OpenGL	
					fullscreen=!fullscreen;				// // Переключим режим "Полный экран"/"Оконный"
					// Заново создадим наше окно OpenGL
					if (!CreateGLWindow("NeHe's Display List Tutorial",1024,768,16,fullscreen))
					{
						return 0;						// Выйти, если окно не было создано

					}
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Закроем окно
	return (msg.wParam);							// Выйдем из программы
}
