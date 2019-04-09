#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include<GL/glut.h>
#include <vector>
#include <utility>
#include <queue>
///#include <stdlib.h>
#include <time.h>

#define KEY_ESC 27
#define KEY_IZQ 97
#define KEY_DER 100
#define KEY_UP 119
#define KEY_DWN 115
#define KEY_ADD 9 ///tab
#define KEY_REMOV 8 ///retro
#define KEY_INTRO 13

using namespace std;
void dfs(int,int,int,int);
void A_sterisco(int,int,int,int);
struct tri{
public:
    int i;
    int j;
    float dis;
    tri(int x,int y,float d):i(x),j(y),dis(d){};
};

class node{
public:
    int first;
    int second;
    vector<tri> vecinos;
    bool enable = true;
    pair<int,int> shorter;

    node(int x,int y): first(x),second(y){};
    node(){}
    ~node(){}
    void addEdge(int nodi, int nodj, float dis){
        tri t(nodi,nodj,dis);
        vecinos.push_back(t);
    }
};

vector<vector<node*> > puntos;
vector<node*> pinta,punts,solu;
int x,y,b;
pair<int,int> selectin(0,0);
vector<pair<int,int> > selected;
void displayPoint()
{
//    glLineWidth(2.0f);
    glPointSize(5.0f);
   // glColor3b(200,100,150);
	glBegin(GL_POINTS);
        glColor4f(1.0f,1.0f,1.0f,0.5f);
        for(int i=0;i<576;++i){
            if(!punts[i]->enable)continue;
            glVertex2i(punts[i]->first,punts[i]->second);
        }
    glEnd();
    glPushMatrix();
        glPointSize(7.0f);
        glBegin(GL_POINTS);
            glColor3f(0.9f,0.8f,0.2f);///
            for(int i=0;i<solu.size();++i){
                glVertex2i(solu[i]->first,solu[i]->second);
            }
        glEnd();
    glPopMatrix();
    glPushMatrix();
        glPointSize(5.5f);
        glBegin(GL_POINTS);
            glColor3f(0.9f,0.8f,0.2f);///
            for(int i=0;i<pinta.size();++i){
                glVertex2i(pinta[i]->first,pinta[i]->second);
            }
        glEnd();
    glPopMatrix();
    glPushMatrix();
        glPointSize(9.0f);
        glBegin(GL_POINTS);
            glColor3f(1.0f,1.0f,0.0f);
            glVertex2i(puntos[selectin.first][selectin.second]->first,puntos[selectin.first][selectin.second]->second);
        glEnd();
    glPopMatrix();
}
void displayEdges(){
    glLineWidth(1.5f);
    int ii,jj;
    glPushMatrix();
        glColor3f(0.5f,0.5f,0.5f);
        glBegin(GL_LINES);
            for(int i=0;i<576;++i){
                if(!punts[i]->enable)continue;
                size_t ss = punts[i]->vecinos.size();
                for(int j=0;j<ss;++j){
                    ii = punts[i]->vecinos[j].i;
                    jj = punts[i]->vecinos[j].j;
                    if(!puntos[ii][jj]->enable)continue;
                    glVertex2i(punts[i]->first,punts[i]->second);
                    glVertex2i(puntos[ii][jj]->first,puntos[ii][jj]->second);
                }
            }
        glEnd();
    glPopMatrix();
    glPushMatrix();
        size_t ss = pinta.size();
        glColor3f(0.95f, 0.5f, 0.0f);
        glLineWidth(3.0f);
        glBegin(GL_LINES);
            for(size_t i=0; i < ss;++i){
                glVertex2i(pinta[i]->first,pinta[i]->second);
                if(i+1 >= ss)continue;
                glVertex2i(pinta[i+1]->first,pinta[i+1]->second);
            }
        glEnd();
    glPopMatrix();
    glPushMatrix();
        ss = solu.size();
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
            for(size_t i=0; i < ss;++i){
                glVertex2i(solu[i]->first,solu[i]->second);
                if(i+1 >= ss)continue;
                glVertex2i(solu[i+1]->first,solu[i+1]->second);
            }
        glEnd();
    glPopMatrix();
}
void OnMouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
    }
}

void OnMouseMotion(int x, int y)
{
     //opcional
	 //hacer algo x,z cuando se mueve el mouse
}



void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-300.0f,  300.0f,-300.0f, 300.0f, -1.0f, 1.0f);

	//dibuja el gizmo
    displayEdges();
    displayPoint();
	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.2f, 0.2f, 0.45f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

}
void load_path();
GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:{
		exit(0);
		break;}
    case KEY_ADD:{
        if(selected.size()==2)selected.erase(selected.begin());
        selected.push_back(selectin);
        pinta.push_back(puntos[selectin.first][selectin.second]);
        //cout<<"add"<<endl;
        break;}
	case KEY_REMOV:{
        puntos[selectin.first][selectin.second]->enable = false;
        //cout<<"rem"<<endl;
        break;}
    case KEY_IZQ:{
        if(selectin.first-1>=0)
            selectin.first-=1;
      //  cout<<"izq"<<endl;
        break;}
	case KEY_DER:{
	    if(selectin.first+1<=23)
            selectin.first+=1;
    //    cout<<"der"<<endl;
        break;}
    case KEY_UP:{
        if(selectin.second+1<=23)
            selectin.second+=1;
  //      cout<<"sube"<<endl;
        break;}
    case KEY_DWN:{
        if(selectin.second-1>=0)
            selectin.second-=1;
//        cout<<"baha"<<endl;
        break;}
    case KEY_INTRO:{
        solu.clear();
        pinta.clear();
        dfs(selected[0].first,selected[0].second,selected[1].first,selected[1].second);
//        A_sterisco(selected[0].first,selected[0].second,selected[1].first,selected[1].second);
        load_path();
        cout<<"intro"<<endl;
        break;}
	default:
		break;
	}
}
//
//el programa principal
//
void load_path(){
    node* curr = puntos[selected[1].first][selected[1].second];
    solu.push_back(curr);
    while (true){
       // cout<<curr->first<<" , "<<curr->second<<endl;
        solu.push_back(puntos[curr->shorter.first][curr->shorter.second]);
        curr = puntos[curr->shorter.first][curr->shorter.second];
        if(curr == puntos[selected[0].first][selected[0].second]){
            solu.push_back(curr);
           // cout<<"solu sal"<<endl;
            break;
        }
    }
}
void creargrafo(){
    for(int i=0;i<576;i+=24){
        vector<node*> aux;
        for(int j=0;j<576;j+=24){
            y = rand()%20;
            y += j;
            x = rand()%20;
            x += i;
            node* n = new node(x-288,y-288);
            punts.push_back(n);
            aux.push_back(n);
        }
        puntos.push_back(aux);
    }
    float dist=0.0;
    for(int i=0;i<24;++i){
       // cout<<"va i, "<<i<<endl;
        for(int j=0;j<24;++j){
          //  cout<<"va j, "<<j<<endl;
            if(i-1 >=0 && j+1 <=23){
           //     cout<<"entra, "<<1<<endl;
                dist = sqrt(pow(puntos[i][j]->first - puntos[i-1][j+1]->first,2)+pow(puntos[i][j]->second - puntos[i-1][j+1]->second,2));
                puntos[i][j]->addEdge(i-1,j+1,dist);}
            if(j+1 <=23){
           //     cout<<"entra, "<<2<<endl;
                dist = sqrt(pow(puntos[i][j]->first - puntos[i][j+1]->first,2)+pow(puntos[i][j]->second - puntos[i][j+1]->second,2));
                puntos[i][j]->addEdge(i,j+1,dist);}
            if(i+1<=23 && j+1 <=23){
            //    cout<<"entra, "<<3<<endl;
                dist = sqrt(pow(puntos[i][j]->first - puntos[i+1][j+1]->first,2)+pow(puntos[i][j]->second - puntos[i+1][j+1]->second,2));
                puntos[i][j]->addEdge(i+1,j+1,dist);}
            if(i-1>=0){
          //      cout<<"entra, "<<4<<endl;
                dist = sqrt(pow(puntos[i][j]->first - puntos[i-1][j]->first,2)+pow(puntos[i][j]->second - puntos[i-1][j]->second,2));
                puntos[i][j]->addEdge(i-1,j,dist);}
            if(i+1<=23){
           //     cout<<"entra, "<<5<<endl;
                dist = sqrt(pow(puntos[i][j]->first - puntos[i+1][j]->first,2)+pow(puntos[i][j]->second - puntos[i+1][j]->second,2));
                puntos[i][j]->addEdge(i+1,j,dist);}
            if(i-1>=0 && j-1>=0){
           //     cout<<"entra, "<<6<<endl;
                dist = sqrt(pow(puntos[i][j]->first - puntos[i-1][j-1]->first,2)+pow(puntos[i][j]->second - puntos[i-1][j-1]->second,2));
                puntos[i][j]->addEdge(i-1,j-1,dist);}
            if(j-1>=0){
          //      cout<<"entra, "<<7<<endl;
                dist = sqrt(pow(puntos[i][j]->first - puntos[i][j-1]->first,2)+pow(puntos[i][j]->second - puntos[i][j-1]->second,2));
                puntos[i][j]->addEdge(i,j-1,dist);}
            if(i+1<=23 && j-1>=0){
           //     cout<<"entra, "<<8<<endl;
                dist = sqrt(pow(puntos[i][j]->first - puntos[i+1][j-1]->first,2)+pow(puntos[i][j]->second - puntos[i+1][j-1]->second,2));
                puntos[i][j]->addEdge(i+1,j-1,dist);}
        }
    }
    cout<<"creado, "<<endl;
}
void dfs(int i1, int i2, int o1,int o2){
    int ii,ji,oi,oj;
    ii = puntos[i1][i2]->first;
    ji = puntos[i1][i2]->second;
    oi = puntos[i1][i2]->first;
    oj = puntos[i1][i2]->second;
    bool vised[24][24];
    queue<pair<int,int> > q;
    q.push(make_pair(i1,i2));
    vised[i1][i2]=1;
    pinta.push_back(puntos[i1][i2]);
    while(!q.empty()){
        int x=q.front().first,y=q.front().second;
        node* aux = puntos[x][y];
        if(x == o1 && y==o2){
            pinta.push_back(puntos[o1][o2]);
            break;
        }
        size_t ss = aux->vecinos.size();
        q.pop();
        for(size_t i=0;i<ss;++i){
            int xa=aux->vecinos[i].i, ya=aux->vecinos[i].j;
            if(! puntos[xa][ya]->enable) continue;
            if(vised[xa][ya]==0){
                q.push(make_pair(xa,ya));
                vised[xa][ya] = 1;
                puntos[xa][ya]->shorter.first = x;
                puntos[xa][ya]->shorter.second = y;
                pinta.push_back(aux);
            }
        }
    }
 //   cout<<"salio"<<endl;
}
class compare{
public:
    bool operator ()(tri& A, tri& B){
        return (A.dis)>(B.dis);
    }
};
float distA(const node* A,const tri&b,const node* C){
    node*aux=puntos[b.i][b.j];
    float di = sqrt(pow(aux->first - C->first,2)+pow(aux->second - C->second,2));
    return b.dis + di;
}
void A_sterisco(int i1, int i2, int o1,int o2){
    node* obj = puntos[o1][o2];
    priority_queue<tri,vector<tri>,compare> q;
    int ii,ji,oi,oj;
    ii = puntos[i1][i2]->first;
    ji = puntos[i1][i2]->second;
    oi = puntos[i1][i2]->first;
    oj = puntos[i1][i2]->second;
    bool vised[24][24];
    node* aux = puntos[i1][i2];
    tri t(i1,i2,sqrt(pow(aux->first - obj->first,2)+pow(aux->second - obj->second,2)));
    q.push(t);
    vised[i1][i2]=1;
    pinta.push_back(puntos[i1][i2]);
    while(!q.empty()){
        int x=q.top().i,y=q.top().j;
        aux = puntos[x][y];
        if(x == o1 && y==o2){
            pinta.push_back(puntos[o1][o2]);
            break;
        }
        size_t ss = aux->vecinos.size();
        pinta.push_back(aux);
        q.pop();
        for(size_t i=0;i<ss;++i){
            int xa = aux->vecinos[i].i , ya = aux->vecinos[i].j;
            if(! puntos[xa][ya]->enable) continue;
            if(vised[xa][ya]==0){
                float D1 = distA(aux,aux->vecinos[i],obj);
                t.dis = D1; t.i = xa ; t.j=ya;
                q.push(t);
                vised[xa][ya] = 1;
                puntos[xa][ya]->shorter.first = x;
                puntos[xa][ya]->shorter.second = y;
//                aux->shorter.first = aux->first;
  //              aux->shorter.second = aux->second;
            }
        }
    }
   // cout<<"salioA"<<endl;
}
int main(int argc, char** argv) {
    srand(time(NULL));
    creargrafo();
	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(50, 50); //posicion de la ventana
	glutCreateWindow("Grafo"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);


	//qt = new quadTree();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
