#include "MinMax.h"

vector<Tablero> moves(Tablero t, int x, int y){
    vector<Tablero> result;
    Tablero aux=t;
    if(x-1>=0&&y-1>=0){
        if(t[x-1][y-1]=='0'){
            aux[x][y]='0';
            aux[x-1][y-1]='2';
            result.push_back(aux);
        }
    }
    if(x+1<=7&&y-1>=0){
        if(t[x+1][y-1]=='0'){
            aux[x][y]='0';
            aux[x+1][y-1]='2';
            result.push_back(aux);
        }
    }
    return result;
}

vector<Tablero> eats(Tablero t, int x, int y){
    vector<Tablero> result;
    Tablero aux;
    if(x-2>=0&&y-2>=0){
        if(t[x-1][y-1]=='1'&&t[x-2][y-2]=='0'){
            aux=t;
            aux[x][y]='0';
            aux[x-1][y-1]='0';
            aux[x-2][y-2]='2';
            result.push_back(aux);
        }
    }
    if(x+2<=7&&y-2>=0){
        if(t[x+1][y-1]=='1'&&t[x+2][y-2]=='0'){
            aux=t;
            aux[x][y]='0';
            aux[x+1][y-1]='0';
            aux[x+2][y-2]='0';
            result.push_back(aux);
        }
    }
    return result;
}

MinMax::MinMax(Tablero b, int d, int md, bool m):key(b),depth(d),Max(m),maxDepth(md){
    //cout<<"Creado"<<endl;
    vector<Tablero> options1,options2,options;
    vector<Tablero> aux;
    if(depth<maxDepth){
        for(int i=0;i<key.size();++i){
            for(int j=0; j<key[i].size();++j){
                if(key[i][j]=='2'){
                    aux=moves(key,i,j);
                    for(size_t i=0;i<aux.size();++i)
                        options1.push_back(aux[i]);
                    aux=eats(key,i,j);
                    for(size_t i=0;i<aux.size();++i)
                        options2.push_back(aux[i]);
                }
            }
        }
    }
    if(options2.size()!=0)
        options=options2;
    else
        options=options1;
    for(size_t i=0;i<options.size();++i){
        MinMax* m = new MinMax(options[i],depth+1,maxDepth,!Max);
        sons.push_back(m);
    }
    evaluate();
}

void MinMax::evaluate(){
    if(sons.size()==0){
        int result=0;
        for(size_t i=0;i<key.size();++i){
            for(size_t j=0;j<key.size();++j){
                if(key[i][j]=='1')
                    --result;
                if(key[i][j]=='2')
                    ++result;
            }
        }
        val=result;
    }
    else{
        for(size_t i=0;i<sons.size();++i)
            sons[i]->evaluate();
        if(Max){
            int m=-10000,aux;
            for(size_t i=0;i<sons.size();++i){
                aux=sons[i]->val;
                if(aux>m)
                    m=aux;
            }
            val=m;
        }
        else{
            int m=100000,aux;
            for(size_t i=0;i<sons.size();++i){
                aux=sons[i]->val;
                if(aux<m)
                    m=aux;
            }
            val=m;
        }
    }
}

Tablero MinMax::choose(){
    Tablero result;
    int aux=-10000;
    for(size_t i=0;i<sons.size();++i){
        if(sons[i]->val>aux){
            aux=sons[i]->val;
            result=sons[i]->key;
        }
    }
    return result;
}

MinMax::~MinMax()
{
    //dtor
}
