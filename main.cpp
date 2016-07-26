/*
 *		��� ��������� ������� ������ ������ (Jeff Molofee) � 2000 �.
 *		�������������� Shawn T. ��� ��������� ���������� (%3.2f, num).
 *		�������� ������� ��������� ������ (Fredric Echols) �� ���������
 *		� ����������� �������� ����, �� ���������� ��� ��������!
 *		���� �� ����� ��� ��������� ��������, ����������, ����� ��� �����.
 *		�������� ��� ���� nehe.gamedev.net
 */

#include <windows.h>		// ������������ ���� ��� Windows
#include <stdio.h>			// ������������ ���� ������������ �����/������
#include <gl\gl.h>			// ������������ ���� ���������� OpenGL32
#include <gl\glu.h>			// ������������ ���� ���������� GLu32
#include <gl\glaux.h>		// ������������ ���� ���������� GLaux
#include <math.h>     // ������������ ���� ��� �������������� ���������� ( ����� )
#include <stdarg.h>   // ������������ ���� ��� ������� ��� ������ � ����������
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


                      //  ����������� ���������� ( ����� )

HDC			hDC=NULL;		// ������� �������� GDI ����������
HGLRC		hRC=NULL;		// ���������� �������� �����������
HWND		hWnd=NULL;		// �������� ����� ������ ����
HINSTANCE	hInstance;		// �������� ��������� ����������

bool	keys[256];			// ������ ����������� ��� ������������ ����������
bool	active=TRUE;		// ���� "��������" ����. ��������������� �������� (TRUE) �� ���������.
bool	fullscreen=TRUE;	// ���� "�� ������ �����". ��������������� � ������������� ����� �� ���������.

GLuint  base;      // ���� ������ ����������� ��� �����
GLfloat  cnt1;     // ������ ������� ��� ������������ � ������������ ������
GLfloat  cnt2;     // ������ ������� ��� ������������ � ������������ ������

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// ���������� ��� WndProc


GLvoid BuildFont(GLvoid)  // ���������� ������ ���������� ������
{
  HFONT  font;            // ������������� �����

  base = glGenLists(96);  // ������� ����� ��� 96 �������� ( ����� )

  font = CreateFont(  -18,        // ������ ����� ( ����� )
        0,        // ������ �����
        0,        // ���� ���������
        0,        // ���� �������
        FW_BOLD,      // ������ ������
        FALSE,        // ������
        FALSE,        // �������������
        FALSE,        // ��������������
        ANSI_CHARSET,      // ������������� ������ ��������
        OUT_TT_PRECIS,      // �������� ������
        CLIP_DEFAULT_PRECIS,    // �������� ���������
        ANTIALIASED_QUALITY,    // �������� ������
        FF_DONTCARE|DEFAULT_PITCH,  // ��������� � ���
        "Courier New");      // ��� ������
  SelectObject(hDC, font);        // ������� �����, ��������� ���� ( ����� )

  wglUseFontBitmaps(hDC, 32, 96, base); // ��������� 96 �������� ������� � ������� ( ����� )
}

GLvoid KillFont(GLvoid)            // �������� ������
{
   glDeleteLists(base, 96);        // �������� ���� 96 ������� ����������� ( ����� )
}


GLvoid glPrint(const char *fmt, ...)        // �������� ������� "������" GL
{
  char    text[256];      // ����� ��� ����� ������
  va_list    ap;          // ��������� �� ������ ����������
  if (fmt == NULL)     // ���� ��� ������
    return;            // ������ �� ������

  va_start(ap, fmt);           // ������ ������ ����������
      vsprintf(text, fmt, ap); // � ��������������� �������� � �������� ����
  va_end(ap);                  // ��������� ���������� � ������
  glPushAttrib(GL_LIST_BIT);      // ����������� ���� ������ ����������� ( ����� )
  glListBase(base - 32);          // ������ ���� ������� � 32 ( ����� )
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); // ����� �������� ����������� ( ����� )
  glPopAttrib(); // ������� ����� ������ ����������� ( ����� )
}

int InitGL(GLvoid)            // ��� ��������� ��������� OpenGL �����
{
  glShadeModel(GL_SMOOTH);    // ��������� ������� ���������
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // ������ ���
  glClearDepth(1.0f);         // ��������� ������ �������
  glEnable(GL_DEPTH_TEST);    // ���������� ����� �������
  glDepthFunc(GL_LEQUAL);     // ��� ����� �������
  // ������������� ������� ���������� �����������
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  BuildFont();            // ��������� �����

  return TRUE;            // ������������� ��������
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// �������� ������� � ������������������� ���� GL
{
	if (height==0)										// ������������� ������� �� 0
	{
		height=1;										// ������������ ������ �������
	}

	glViewport(0,0,width,height);						// ��������� ������� ���� �����������

	glMatrixMode(GL_PROJECTION);						// �������� ������� ��������
	glLoadIdentity();									// ������������� ������� ��������

	// ��������� ����������� ������ ����
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// �������� ������� ������ �����������
	glLoadIdentity();									// ������������� ������� ������ �����������
}

int DrawGLScene(GLvoid) // ����� �� ����� �������� ���
{
    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ����� � ������ �������
  glLoadIdentity(); // ����� ���������

glTranslatef(-7.4f,-1.5f,-12.0f); // ������������ �� ���� ������� ������
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
glPrint("%0.0f",(float)x-1);  // ������ ������ GL �� �����
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
glPrint("%0.0f",(float)y-1);  // ������ ������ GL �� �����
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
glPrint("%0.0f",(float)y);  // ������ ������ GL �� �����
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
  // ���������������� ������ �� ������
glRasterPos2f(0.0f, -0.2f);
glPrint("Maksimalus srauto greitis m/s% 5.2f",termik.Vmax/10 );  // ������ ������ GL �� �����
glRasterPos2f(0.0f, -0.4f);
glPrint("Minimalus srauto greitis m/s% 5.2f",termik.Vmin/10 );  // ������ ������ GL �� �����
glRasterPos2f(0.0f, -0.6f);
glPrint("Minimumo X koordinate %5.2f",termik.Xmin );  // ������ ������ GL �� �����
glRasterPos2f(0.0f, -0.8f);
glPrint("Minimumo Y koordinate  %5.2f",termik.Ymin );  // ������ ������ GL �� �����
glRasterPos2f(0.0f, -1.0f);
glPrint("Maksimumo X koordinate  %5.2f",termik.Xmax );  // ������ ������ GL �� �����
glRasterPos2f(0.0f, -1.2f);
glPrint("Maksimumo Y koordinate  %5.2f",termik.Ymax );  // ������ ������ GL �� �����
glRasterPos2f(0.0f, -1.4f);
glPrint("Terminio srauto skersmuo [m]%5.2f",(termik.R*600)/5);  // ������ ������ GL �� �����
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
glPrint("Terminio srauto polinkis [Laip] %5.0f",termik.naklon );  // ������ ������ GL �� �����
glRasterPos2f(0.1f, -0.4f);
glPrint("Vejo kriptis [Laip]  %5.0f", termik.wind );  // ������ ������ GL �� �����
glRasterPos2f(0.1f, -0.6f);
glPrint("Terminio srauto aukstis [m] %5.0f",termik.H );  // ������ ������ GL �� �����


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
glPrint("x %1.0f",termik.X );  // ������ ������ GL �� �����
glRasterPos3f(0.0f, -2.0f,0.0f);
glPrint("y %1.0f",termik.Y );  // ������ ������ GL �� �����
glRasterPos3f(0.0f, 0.0f,-2.0f);
glPrint("z 0" );  // ������ ������ GL �� �����   



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


  return TRUE;   // ��� �������
}


GLvoid KillGLWindow(GLvoid)								// ������� ������� ���������� ����
{
	if (fullscreen)										// �� � ����� �������� �������?
	{
		ChangeDisplaySettings(NULL,0);					// ���� ��, �� ������������ �� ������� ����
		ShowCursor(TRUE);								// ���������� ������ ����
	}

	if (hRC)											// �������� ����������� ��� ����?
	{
		if (!wglMakeCurrent(NULL,NULL))					// �� ����� ���������� RC � DC ���������?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// �� ����� ������� RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// ������������� RC � NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// �� ����� ����������� �������� DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// ������������� DC � NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// �� ����� ���������� ����?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// ������������� hWnd � NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// ����� ����������������� �����
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// ������������� hInstance � NULL
	}
    KillFont();            // ���������� �����
}

/*	��� ����� ��������� ������� ���� ���� OpenGL.  ���������:							*
 *	title			- ���������, ������������ ������� ����								*
 *	width			- ������ GL ���� ��� �������������� ������							*
 *	height			- ������ GL ���� ��� �������������� ������							*
 *	bits			- ���������� ��� �� ���� (8/16/24/32)								*
 *	fullscreenflag	- ������������� �������������� (TRUE) ��� ��������(FALSE) �������	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// �������� ���������� ������� ������� �����
	WNDCLASS	wc;						// ��������� ������� Windows
	DWORD		dwExStyle;				// ����������� ����� ����
	DWORD		dwStyle;				// ����� ����
	RECT		WindowRect;				// �������� �������� �������� ������ � ������� ������� ����� ��������������
	WindowRect.left=(long)0;			// ������������� �������� ���� (Left) � 0
	WindowRect.right=(long)width;		// ������������� �������� ����� (Right) � ��������� ������ (Width)
	WindowRect.top=(long)0;				// ������������� �������� ���� (Top) � 0
	WindowRect.bottom=(long)height;		// ������������� �������� ��� (Bottom) � ��������� ������ (Height)

	fullscreen=fullscreenflag;			// ������������� ���������� ���� Fullscreen

	hInstance			= GetModuleHandle(NULL);				// ����������� Instance ��� ������ ����
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// �������������� �� �������, � �������� DC ��� ����.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// ��� �������������� ������ ����
	wc.cbWndExtra		= 0;									// ��� �������������� ������ ����
	wc.hInstance		= hInstance;							// ��������� Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// ������� ������ �� ���������
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// ������� ������� �������
	wc.hbrBackground	= NULL;									// ��� ��� GL �� ���������
	wc.lpszMenuName		= NULL;									// ��� �� ����� ����
	wc.lpszClassName	= "OpenGL";								// ��������� ��� ������

	if (!RegisterClass(&wc))									// ���������� ���������������� ����� ����
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// ��������� FALSE
	}
	
	if (fullscreen)												// ���������� �������� �������������� �����?
	{
		DEVMODE dmScreenSettings;								// ����� ����������
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// ��������, ��� ������ �������
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// ������ ��������� devmode
		dmScreenSettings.dmPelsWidth	= width;				// ������� ������ ������
		dmScreenSettings.dmPelsHeight	= height;				// ������� ������ ������
		dmScreenSettings.dmBitsPerPel	= bits;					// ������� ���������� ��� �� �������
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// ���������� ���������� ��������� ����� � �������� ����������.  � �������: CDS_FULLSCREEN ����������� �� ������ �����Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// ���� ������ �� ���������, ��������� ��� �����������. ����� ��� ������������ ������� �����.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// ������ ������� �����.  Fullscreen = FALSE
			}
			else
			{
				// �������� ���������, ��� ���������� ��������� ������.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// ���������� FALSE
			}
		}
	}

	if (fullscreen)												// ��� �� � ������������� ������?
	{
		dwExStyle=WS_EX_APPWINDOW;								// ����������� ����� ����
		dwStyle=WS_POPUP;										// ����� ����
		ShowCursor(FALSE);										// ������ ������ ����
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// ����������� ����� ����
		dwStyle=WS_OVERLAPPEDWINDOW;							// ����� ����
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// ���������� ����, ����� ��� ��������������� ���������� �������

	// ������� ����
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// ����������� ����� ��� ����
								"OpenGL",							// ������������ ������
								title,								// ��������� ����
								dwStyle |							// ������������ ����� ����
								WS_CLIPSIBLINGS |					// ��������� ����� ����
								WS_CLIPCHILDREN,					// ��������� ����� ����
								0, 0,								// �������������� ����
								WindowRect.right-WindowRect.left,	// ���������� ������ ����
								WindowRect.bottom-WindowRect.top,	// ���������� ������ ����
								NULL,								// ��� ������������� ����
								NULL,								// ��� ����
								hInstance,							// Instance
								NULL)))								// �� �������� ��������� �� WM_CREATE
	{
		KillGLWindow();								// ������������� �������
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ������� FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd �������� Windows ����� ��������� �� �����
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// ������ ����������� ������� ��������
		1,											// ����� ������
		PFD_DRAW_TO_WINDOW |						// ������ ������ ������������ ����
		PFD_SUPPORT_OPENGL |						// ������ ������ ������������ OpenGL
		PFD_DOUBLEBUFFER,							// ������ �������������� ������� �����������
		PFD_TYPE_RGBA,								// ������ RGBA �������
		bits,										// ����� ������� �����
		0, 0, 0, 0, 0, 0,							// ���� ����� ������������
		0,											// ��� ����� �����������
		0,											// ��� ������ ������������
		0,											// ��� ������ ����������
		0, 0, 0, 0,									// ���� ���������� ������������
		16,											// 16������ Z-����� (����� �������)  
		0,											// ��� ������ ��������� (stencil buffer)
		0,											// ��� ���������������� ������
		PFD_MAIN_PLANE,								// ������� ��������� ���������
		0,											// ���������������
		0, 0, 0										// ���� ����� ������������
	};
	
	if (!(hDC=GetDC(hWnd)))							// �� �������� �������� ����������?
	{
		KillGLWindow();								// ������������� �������
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ������� FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Windows ����� ��������������� ������ �������?
	{
		KillGLWindow();								// ������������� �������
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ������� FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// �� ����� ���������� ������ �������?
	{
		KillGLWindow();								// ������������� �������
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ������� FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// �� ����� �������� �������� �����������?
	{
		KillGLWindow();								// ������������� �������
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ������� FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// �������� ������������ �������� �����������
	{
		KillGLWindow();								// ������������� �������
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ������� FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// �������� ����
	SetForegroundWindow(hWnd);						// ������ �������� ���������
	SetFocus(hWnd);									// ������������� ����� ����������� �� ����
	ReSizeGLScene(width, height);					// ������������� ���� GL ���� � ������������

	if (!InitGL())									// �������������� ���� GL ����
	{
		KillGLWindow();								// ������������� �������
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ��������� FALSE
	}

	return TRUE;									// �������� ���������� ��������������
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// ����� ��� ����� ����
							UINT	uMsg,			// ��������� ��� ����� ����
							WPARAM	wParam,			// �������������� ���������� ���������
							LPARAM	lParam)			// �������������� ���������� ���������
{
	switch (uMsg)									// �������� ��������� ����
	{
		case WM_ACTIVATE:							// ��������� �� ���������� �� ����������� ����
		{
			if (!HIWORD(wParam))					// �������� ��������� ������������
			{
				active=TRUE;						// ��������� �������
			}
			else
			{
				active=FALSE;						// ��������� ������ �� �������
			}

			return 0;								// ���������� � ����� ���������
		}

		case WM_SYSCOMMAND:							// ������������ ��������� �������
		{
			switch (wParam)							// �������� ������ �������
			{
				case SC_SCREENSAVE:					// ��������� ���������� �����������?
				case SC_MONITORPOWER:				// ������� �������� �������������� � ����� ���������� �������?
				return 0;							// �� ������ ����� ���������
			}
			break;									// �����
		}

		case WM_CLOSE:								// �� �������� ��������� � �������� ���������?
		{
			PostQuitMessage(0);						// ������� ��������� � ������
			return 0;								// ���������
		}

		case WM_KEYDOWN:							// ������� ���� ������?
		{
			keys[wParam] = TRUE;					// ���� ��� ��� ������� ��� TRUE
			return 0;								// ���������
		}

		case WM_KEYUP:								// ������� ���� ��������?
		{
			keys[wParam] = FALSE;					// ���� ��� ��� ������� ��� FALSE
			return 0;								// ���������
		}

		case WM_SIZE:								// ���������� ���� OpenGL
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=������, HiWord=������
			return 0;								// ���������
		}
	}

	// ���������� ��� ������ ��������� � DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// ���������� Instance
					LPSTR		lpCmdLine,			// ��������� ��������� ������
					int			nCmdShow)			// �������� ��������� ����
{
	MSG		msg;									// ��������� ��������� ����
	BOOL	done=FALSE;								// ��������� ���������� ������ �� �����
	
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




	// �������� ������������ ����� ����� ����������� �� ������������
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// ������� �����
	}

	// �������� ���� ���� OpenGL
	if (!CreateGLWindow("Modelirovanie",1200,768,32,fullscreen))
	{
		return 0;									// ������� ���� ���� �� ���� �������
	}

	while(!done)									// ����, ������� ������������ ���� done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// ���� ��������� ���������?
		{
			if (msg.message==WM_QUIT)				// �� �������� ��������� � ������?
			{
				done=TRUE;							// ���� ��� done=TRUE
			}
			else									// ���� ���,���������� �������� � ����������� ����
			{
				TranslateMessage(&msg);				// ��������� ���������
				DispatchMessage(&msg);				// �������� ���������
			}
		}
		else										// ���� ��������� ���
		{
			// ������ �����.  ������� ������� ������ ESC � ��������� � ������ �� DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// �������?  ���� �������� ��������� � ������?
			{
				done=TRUE;							// ESC ��� DrawGLScene ������������ "�����"
			}
			else									// �� ����� ��������, ��������� �����
			{
                SwapBuffers(hDC);					// ����������� ������ (Double Buffering)
				if (keys[VK_F1])						// ���� ������ ������ F1?
				{
					keys[VK_F1]=FALSE;					// ���� ��� - ��������� �������� FALSE
					KillGLWindow();						// ������� ������� ���� OpenGL	
					fullscreen=!fullscreen;				// // ���������� ����� "������ �����"/"�������"
					// ������ �������� ���� ���� OpenGL
					if (!CreateGLWindow("NeHe's Display List Tutorial",1024,768,16,fullscreen))
					{
						return 0;						// �����, ���� ���� �� ���� �������

					}
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// ������� ����
	return (msg.wParam);							// ������ �� ���������
}
