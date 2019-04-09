#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "include/Fichita.h"
#include "MinMax.h"

#define KEY_ESC 27
#define KEY_NEXT 9
using namespace std;
MinMax* mym;
bool player = true;
vector<Fichita *> fichas;
Tablero elTablero(8);

void loadFichitas(){
    fichas.clear();
    bool color = 0;
    int cont =0;
    int ii;
    int jj;
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            if(elTablero[i][j]!=0){
                if(elTablero[i][j]== '1')
                    fichas.push_back(new Fichita(j,i,0));
                else if(elTablero[i][j]=='2')
                    fichas.push_back(new Fichita(j,i,1));
               // cout<<elTablero[i][j]<<" <-> "<<j<<","<<i<<endl;
            }
        }
    }
}/*
void updateFichitas(){
    fichas.clear();
    Fichita* aux;
    int ii, jj;
    for(int i = 0; i < elTablero.size(); ++i){
        for(int j = 0; j < elTablero[i].size(); ++j){
            if(elTablero[i][j] == '1'){
                ii = (75*i + 37)-300;
                jj = (75*j + 37)-300;
                aux = new Fichita(jj,ii,0);
                fichas.push_back(aux);
            }
            else if(elTablero[i][j] == '2'){
                ii = (75*i + 37)-300;
                jj = (75*j + 37)-300;
                aux = new Fichita(jj,ii,1);
                fichas.push_back(aux);
            }
        }
    }
   // fichas = nuevo;
}
*/
void drawFicha(){
    ///cout<<fichas.size()<<endl;
    for(size_t i =0;i<fichas.size();++i){
        glPushMatrix();
            glPointSize(30.0f);
            if(fichas[i]->color)
                glColor3f(0.0f,0.0f,1.0f);
            else
                glColor3f(1.0f,0.0f,0.0f);
            glBegin(GL_POINTS);
//                cout<<fichas[i]->posX<<" , "<<fichas[i]->posY<<" -> "<<fichas[i]->color <<endl;
                glVertex2i(((fichas[i]->posX)*75) -300+37,((fichas[i]->posY)*75)-300+37);
            glEnd();
        glPopMatrix();
    }
}
int l = 75;
void tablero(){
    int x=-300,y=300,X=300,Y=-300;
    for(int f=0;f<8;++f){
        for(int c=0;c<8;++c){
            if((f+c+1) % 2 == 0){
                glColor3f(0.0f,0.0f,0.0f);
                glBegin(GL_QUADS);
                    glVertex2i(x, y);
                    glVertex2i(x, y-l);
                    glVertex2i(x+l, y-l);
                    glVertex2i(x+l, y);
                glEnd();
            }
            else{
                glColor3f(255.0f,255.0f,255.0f);
                glBegin(GL_QUADS);
                    glVertex2i(x, y);
                    glVertex2i(x, y-l);
                    glVertex2i(x+l, y-l);
                    glVertex2i(x+l, y);
                glEnd();
            }
            x+=l;
        }
        y-=l;
        x=-300;
    }
}
void glPaint(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glLoadIdentity();
	glPushMatrix();
        glColor3f(0.7f,0.35f,0.0f);
        glLineWidth(3.0f);
        glBegin(GL_LINES);
            glVertex2i(-300,300);glVertex2i(-300,-300);
            glVertex2i(-300,-300);glVertex2i(300,-300);
            glVertex2i(300,-300);glVertex2i(300,300);
            glVertex2i(300,300);glVertex2i(-300,300);
        glEnd();
	glPopMatrix();
	tablero();
	drawFicha();
	glutSwapBuffers();
	glutPostRedisplay();
}
void printTab(){
    for(size_t i=0;i<elTablero.size();++i){
            cout<<"|";
            for(size_t j=0;j<elTablero[i].size();++j)
                cout<<elTablero[7-i][j]<<"|";
            cout<<endl;
        }
        cout<<endl;
}
void parametrizar(int &x, int &y){
    if(x >= 400){
        x = x - 400;
    }
    else{
        x = (400-x)*(-1);
    }
    if(y <= 400){
        y = 400 - y;
    }
    else{
        y = (y-400)*(-1);
    }
}

float euclidiana(int x, int y, int a, int b){
    return sqrt( pow((x - a),2) + pow((y - b), 2) );
}

int buscar(int x, int y){
    int cx;
    int cy;
    for(int i = 0; i < fichas.size(); ++i){
        int cx = fichas[i]->posX*75 -300+37;
        int cy = fichas[i]->posY*75 -300+37;
        if(euclidiana(x,y, cx, cy) < 37.5){
            return i;
        }
    }
    return -1;
}
void comer(int cx,int cy,int& x,int& y){
    int mx = (cx+x)/2,my = (cy+y)/2;
    cout<<"comer: "<<mx<<"-"<<my<<endl;
    if(elTablero[my][mx]=='2'){
        elTablero[my][mx] = '0';
        elTablero[cy][cx] = '0';
        elTablero[y][x] = '1';
    }
    printTab();
}

int ficha_seleccionada = -1;
void OnMouseClick(int button, int state, int x, int y){
    parametrizar(x, y);
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        int posicion  = buscar(x,y);
        if(posicion != -1){
            ficha_seleccionada = posicion;
        }
        cout<<posicion<<"<-< pos"<<endl;
    }
//    printTab();
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        int posicion  = buscar(x,y);
        if(ficha_seleccionada != -1){
            if(posicion == -1){
                x += 300; x /= 75;
                y += 300; y /= 75;
                int cx = fichas[ficha_seleccionada]->posX;
                int cy = fichas[ficha_seleccionada]->posY;
                cout<<x<<","<<y<<" | "<<cx<<","<<cy<<endl;
                if(x != cx && y != cy && y-1 == cy && (x-1==cx || x+1 == cx)){
                    elTablero[cy][cx] = '0';
                    elTablero[y][x] = '1';
                    fichas[ficha_seleccionada]->posX = x;
                    fichas[ficha_seleccionada]->posY = y;
                }
                if(x != cx && y != cy && y-2 == cy && (x-2==cx || x+2 == cx)){
                    comer(cx,cy,x,y);
                    fichas[ficha_seleccionada]->posX = x;
                    fichas[ficha_seleccionada]->posY = y;
                }
            }
        }
    }
    loadFichitas();
}

void init_GL(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
    glOrtho(-400.0f, 400.0f, -400.0f, 400.0f, -1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;
	default:{
        mym = new MinMax(elTablero,0,3,1);
        elTablero = mym->choose();
        cout<<"sss"<<endl;
        printTab();
        loadFichitas();
		break;
		}
	}
}

int main(int argc, char** argv) {
    for(int i=0;i<8;++i){
        elTablero[i].assign(8,'0');
    }
    for(int i=0; i<8; ++i){
        for(int j=0; j<3; ++j){
            if((i+j)%2){
                elTablero[j][i]='1';
            }
        }
    }
    for(int i=0; i<8; ++i){
        for(int j=5; j<8; ++j){
            if((i+j)%2){
                elTablero[j][i]='2';
            }
        }
    }
    for(size_t i=0;i<elTablero.size();++i){
        cout<<"|";
        for(size_t j=0;j<elTablero[i].size();++j){
            cout<<elTablero[7-i][j]<<"|";
        }
        cout<<endl;
    }
    loadFichitas();
	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800); //tamaño de la ventana
	glutInitWindowPosition(10, 10); //posicion de la ventana
	glutCreateWindow("DAMAS"); //titulo de la ventana

   // mym = new MinMax(elTablero,0,4,1);

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);

	glutMainLoop(); //bucle de rendering
	return 0;
}
