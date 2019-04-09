#include <vector>

#ifndef FICHITA_H
#define FICHITA_H

class Fichita
{
public:
    int posX , posY;
//    int posiblesMovimientos;
    char color; /// 0 rojo, 1 azul;

    Fichita (int x,int y,bool col);
    ~Fichita();
};

#endif // FICHITA_H
