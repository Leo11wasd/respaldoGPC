#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <iostream>

struct VERTICE
{
	  float x, y, z;
	  float r, g, b;
};

float suma(float x){
    return x+.5;
}

float linea(float x){
    return (x/2)+x;
}



VERTICE *creaSolRev2(float x_min, float x_max, int N_Rodajas, int M_PuntosPorRodaja,float(*func)(float))
{
    float deltaTheta  = 8.0 * atan(1.0) / ( M_PuntosPorRodaja - 1 );
    float deltaX = ( x_max - x_min ) / ( N_Rodajas -1 );
    float theta;
    float x,y,z;
    float r_yz;

    //float radio2 = radio * radio;

    VERTICE* vertices = (VERTICE*) malloc( ((N_Rodajas * M_PuntosPorRodaja)+2) * sizeof(VERTICE));

    int k_rod, k_t, k_indice,pos;
    pos=0;
    for( k_rod = 0; k_rod < N_Rodajas; k_rod++)
    {
        x     = x_min + k_rod * deltaX;
        r_yz  = func(x);

        k_indice = k_rod * M_PuntosPorRodaja;

        for(k_t = 0; k_t < M_PuntosPorRodaja; k_t++)
        {
          theta = k_t * deltaTheta;
          y = r_yz * cos(theta);
          z = r_yz * sin(theta);

          //printf("%d %f %f %f \n",k_indice+k_t,x,y,z);

          vertices[pos].x = x;
          vertices[pos ].y = y;
          vertices[pos].z = z;

          vertices[pos].r =((double) rand() / (RAND_MAX+1));
          vertices[pos].g = ((double) rand() / (RAND_MAX+1));
          vertices[pos].b = ((double) rand() / (RAND_MAX+1));
          pos++;
        }
    }

    //ponemos los vertices pivote para las tapas
    x=x_min;
    y=0;
    z=0;
    vertices[pos].x = x;
    vertices[pos ].y = y;
    vertices[pos].z = z;

    vertices[pos].r =((double) rand() / (RAND_MAX+1));
    vertices[pos].g = ((double) rand() / (RAND_MAX+1));
    vertices[pos].b = ((double) rand() / (RAND_MAX+1));
    pos++;


    x=x_max+10;
    y=0;
    z=0;
    vertices[pos].x = x;
    vertices[pos ].y = y;
    vertices[pos].z = z;

    vertices[pos].r =((double) rand() / (RAND_MAX+1));
    vertices[pos].g = ((double) rand() / (RAND_MAX+1));
    vertices[pos].b = ((double) rand() / (RAND_MAX+1));
    pos++;

    return vertices;


}

VERTICE *creaResorte(float a, int M, int N_Vueltas)
{
    float menos_pi    = - 4.0 * atan(1.0);
    float deltaTheta  = ( 8.0 * atan(1.0) * N_Vueltas ) / ( M - 1 );
    float deltaPhi    =   4.0 * atan(1.0) / ( M - 1 );
    float theta, phi;

    float x,y,z;
    float r_yz;

    VERTICE* vertices = (VERTICE*) malloc( M * sizeof(VERTICE));

    int k;
    for( k = 0; k < M; k++)
    {
        theta = deltaTheta * k;
        phi   = menos_pi + deltaPhi * k;
        x     = a * cos(phi);
        r_yz  = a * sin(phi);
        y     = r_yz * cos(theta);
        z     = r_yz * sin(theta);

        //printf("%d %f %f %f \n",k,x,y,z);

        vertices[k].x = x;
        vertices[k].y = y;
        vertices[k].z = z;

        vertices[k].r = 1.0; //((double) rand() / (RAND_MAX+1));
        vertices[k].g = 1.0; //((double) rand() / (RAND_MAX+1));
        vertices[k].b = 1.0; //((double) rand() / (RAND_MAX+1));

    }

    return vertices;
}

VERTICE *creaSolRev(float radio, float x_min, float x_max, int N_Rodajas, int M_PuntosPorRodaja)
{
    float deltaTheta  = 8.0 * atan(1.0) / ( M_PuntosPorRodaja - 1 );
    float deltaX = ( x_max - x_min ) / ( N_Rodajas -1 );
    float theta;
    float x,y,z;
    float r_yz;

    float radio2 = radio * radio;

    VERTICE* vertices = (VERTICE*) malloc( N_Rodajas * M_PuntosPorRodaja * sizeof(VERTICE));

    int k_rod, k_t, k_indice;

    for( k_rod = 0; k_rod < N_Rodajas; k_rod++)
    {
        x     = x_min + k_rod * deltaX;
        r_yz  = sqrt( radio2 - x * x);

        k_indice = k_rod * M_PuntosPorRodaja;

        for(k_t = 0; k_t < M_PuntosPorRodaja; k_t++)
        {
          theta = k_t * deltaTheta;
          y = r_yz * cos(theta);
          z = r_yz * sin(theta);

          //printf("%d %f %f %f \n",k_indice+k_t,x,y,z);

          vertices[k_indice + k_t].x = x;
          vertices[k_indice + k_t].y = y;
          vertices[k_indice + k_t].z = z;

          vertices[k_indice + k_t].r =((double) rand() / (RAND_MAX+1));
          vertices[k_indice + k_t].g = ((double) rand() / (RAND_MAX+1));
          vertices[k_indice + k_t].b = ((double) rand() / (RAND_MAX+1));
        }
    }

    return vertices;


}
int** generaCaras(int N_rodajas, int M_puntos) {
    int k, j, c;
    int** caras = new int*[2 * M_puntos * (N_rodajas - 1)];

    for (int i = 0; i < 2 * M_puntos * (N_rodajas - 1); i++) {
        caras[i] = new int[3];
    }

    c = 0;

    for (k = 0; k < N_rodajas - 1; k++) {  // El bucle debe ser N_rodajas - 1, ya que no necesitas calcular las caras de la última rodaja
        for (j = 0; j < M_puntos ; j++) {  // Similarmente, M_puntos - 1 para evitar cálculos innecesarios
            caras[c][0] = (k * M_puntos) + j;

            if(j==M_puntos-1){
                caras[c][1]=(k * M_puntos);
            }
            else{
                caras[c][1] = (k * M_puntos) + j + 1;
            }

            caras[c][2] = ((k + 1) * M_puntos) + j;
            c++;

            caras[c][0] = (k * M_puntos) + j + 1;

            if(j==M_puntos-1){
                caras[c][1]=k * M_puntos;
            }
            else{
                caras[c][1] = ((k + 1) * M_puntos) + j + 1;
            }

            caras[c][2] = ((k + 1) * M_puntos) + j;
            c++;
        }
    }

    return caras;
}


int* generaCarasArr(int N_rodajas, int M_puntos) {
    int k,i, j, c,vertices_base,num_caras,extras;
    num_caras=2* M_puntos * (N_rodajas - 1);// caras "rectangulares"
    extras=6*(M_puntos);
    vertices_base=3*num_caras ;
    int* caras=new int[vertices_base+extras];

    c = 0;

    for (k = 0; k < N_rodajas - 1; k++) {  // El bucle debe ser N_rodajas - 1, ya que no necesitas calcular las caras de la última rodaja
        for (j = 0; j < M_puntos ; j++) {  // Similarmente, M_puntos - 1 para evitar cálculos innecesarios
            caras[c] = (k * M_puntos) + j;
            c++;
            if(j==M_puntos-1){
                caras[c]=(k * M_puntos);
            }
            else{
                caras[c] = (k * M_puntos) + j + 1;
            }
            c++;
            caras[c] = ((k + 1) * M_puntos) + j;
            c++;

            caras[c] = (k * M_puntos) + j + 1;
            c++;
            if(j==M_puntos-1){
                caras[c]=k * M_puntos;
            }
            else{
                caras[c] = ((k + 1) * M_puntos) + j + 1;
            }
            c++;
            caras[c] = ((k + 1) * M_puntos) + j;
            c++;
        }
    }
// juntar las tapas

    for (i=0;i<2;i++){
            for (j=0;j<M_puntos;j++){
                caras[c]=(N_rodajas*M_puntos)+i;//pivote
                c++;
                caras[c]=((j+1)%M_puntos)+(M_puntos*(N_rodajas-1)*i);
                c++;
                caras[c]=j+(M_puntos*(N_rodajas-1)*i);
                c++;
            }
    }
    return caras;
}






float funcion1(float a){
    return a*a;
}

/*
int main()
{
    int M = 3;
    int N = 2;
    float       a = 1.0f;
    //VERTICE *v;
    int k,s,i,j,total;
    total=(6*(M))+(6* M * (N - 1));

    //v = creaSolRev2(1,10,M,N,*funcion1);
    int* cara;
    cara=generaCarasArr(N,M);
    for (int i=0;i<total;i++){
            if(i%3==0){
                std::cout << "\n";
            }
            std::cout << cara[i] << " ";
    }
    std::cout << std::endl;

    delete[] cara;
    //v = creaSolRev2(a,M,N_vueltas);
    //for(k = 0; k < M*N; k++)
      //printf("%d %10.6f %10.6f %10.6f , %f %f %f\n",k,v[k].x,v[k].y,v[k].z,v[k].r,v[k].g,v[k].b);


 //int rows =2 * M * (N - 1);
//int cols =3;

    //for (int i = 0; i < rows; i++) {
    //    for (int j = 0; j < cols; j++) {
    //        std::cout << cara[i][j] << " ";
    //    }
  //      std::cout << std::endl;
    //}
    return 0;
}
*/


