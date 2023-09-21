#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <iostream>
    //float a=0.4f;

struct VERTICE
{
	  float x, y, z;
	  float r, g, b;
};


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

VERTICE *creaSolRev(float (*func)(float),float radio, float x_min, float x_max, int M_PuntosPorRodaja,int N_Rodajas)
{
    // son M_PuntosPorRodaja - 1 fascetas
    // con M_PuntosPorRodaja y el ultimo coincide con el primero( indices 0 y M_PuntosPorRodaja - 1 )
    //

    float deltaTheta  = 8.0 * atan(1.0) / ( M_PuntosPorRodaja - 1 );
    float deltaX = ( x_max - x_min ) / ( N_Rodajas -1 );
    float theta;
    float x,y,z;
    float r_yz;

    //float radio2 = radio * radio;

    VERTICE* vertices = (VERTICE*) malloc( ( 2 + N_Rodajas * M_PuntosPorRodaja ) * sizeof(VERTICE));

    int k_rod, k_t, k_indice;

    for( k_rod = 0; k_rod < N_Rodajas; k_rod++)
    {
        x     = x_min + k_rod * deltaX;
        r_yz  = (*func)(x);//sqrt( radio2 - x * x);

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

          vertices[k_indice + k_t].r =1.0f ;//( k_rod % 3 ) == 0 ? 1.0f : 0; //(float)((double) rand() / (RAND_MAX+1));
          vertices[k_indice + k_t].g =1.0f ;//( k_rod % 3 ) == 0 ? 1.0f : 0; //(float)((double) rand() / (RAND_MAX+1));
          vertices[k_indice + k_t].b = 1.0f;//( k_rod % 3 ) == 0 ? 1.0f : 0; //(float)((double) rand() / (RAND_MAX+1));
        }

        vertices[k_indice + k_t]     = {-radio,0.0f,0.0f,1.0f,1.0f,1.0f};
        vertices[k_indice + k_t + 1] = { radio,0.0f,0.0f,1.0f,1.0f,1.0f};
    }
    return vertices;
}

VERTICE *creaTriangSolRev(float (*func)(float),float radio, float x_min, float x_max, int M_PuntosPorRodaja,int N_Rodajas, int *num_vertices, int *num_triangulos)
{
    int caras               = (N_Rodajas-1) * ( M_PuntosPorRodaja - 1);
    int caras_tapa          =  M_PuntosPorRodaja - 1;
    *num_triangulos         = 2 * caras + 2 * caras_tapa;
    int num_vertices_base   = N_Rodajas * M_PuntosPorRodaja + 2;
    int num_vertices_triang = 3 * (*num_triangulos);
    *num_vertices           = num_vertices_triang;
    //int vertices_caras_lat  = M_PuntosPorRodaja * N_Rodajas;

    int k_rod, k_cara;
    int k_ver_rod;     // número de vertices antes de la rodaja
    int i_ver_rod;     // vertice en la rodaja
    int k_ver_triang;  // indice del vertice en la colección de vertices de los triangulos
    //int k_vertice_tapa;

    VERTICE* vertices_base   = creaSolRev(func, radio, x_min, x_max, M_PuntosPorRodaja, N_Rodajas);

    VERTICE* vertices_triang = (VERTICE*) malloc( num_vertices_triang * sizeof(VERTICE));

    k_ver_triang = 0;

    for(k_rod = 0; k_rod < N_Rodajas - 1; k_rod++)
    {
        k_ver_rod = k_rod * M_PuntosPorRodaja;
        for( i_ver_rod = 0; i_ver_rod < M_PuntosPorRodaja - 1; i_ver_rod++)
        {
            vertices_triang[k_ver_triang++] = vertices_base[k_ver_rod                     + i_ver_rod    ];
            vertices_triang[k_ver_triang++] = vertices_base[k_ver_rod + M_PuntosPorRodaja + i_ver_rod    ];
            vertices_triang[k_ver_triang++] = vertices_base[k_ver_rod + M_PuntosPorRodaja + i_ver_rod + 1];

            vertices_triang[k_ver_triang++] = vertices_base[k_ver_rod                     + i_ver_rod    ];
            vertices_triang[k_ver_triang++] = vertices_base[k_ver_rod + M_PuntosPorRodaja + i_ver_rod + 1];
            vertices_triang[k_ver_triang++] = vertices_base[k_ver_rod                     + i_ver_rod + 1];
        }
    }

    for(k_cara = 0; k_cara < M_PuntosPorRodaja - 1; k_cara++)
    {
            vertices_triang[k_ver_triang++] = vertices_base[num_vertices_base - 2             ];
            vertices_triang[k_ver_triang++] = vertices_base[k_cara                            ];
            vertices_triang[k_ver_triang++] = vertices_base[k_cara + 1                        ];

            vertices_triang[k_ver_triang++] = vertices_base[num_vertices_base - 1             ];
            vertices_triang[k_ver_triang++] = vertices_base[num_vertices_base - 3 - k_cara    ];
            vertices_triang[k_ver_triang++] = vertices_base[num_vertices_base - 3 - k_cara - 1];

    }

}

    int **creaIndices(int M_PuntosPorRodaja,int N_Rodajas, int *num_vertices, int *num_triangulos)
{
    int caras               = (N_Rodajas-1) * ( M_PuntosPorRodaja - 1);
    int caras_tapa          =  M_PuntosPorRodaja - 1;
    *num_triangulos         = 2 * caras + 2 * caras_tapa;
    int num_vertices_base   = (N_Rodajas) * M_PuntosPorRodaja + 2;
    //int num_vertices_triang = 3 * (*num_triangulos);
    *num_vertices           = num_vertices_base;//num_vertices_triang;
    //int vertices_caras_lat  = M_PuntosPorRodaja * N_Rodajas;

    int k_rod, k_cara;
    int k_ver_rod;     // número de vertices antes de la rodaja
    int i_ver_rod;     // vertice en la rodaja
    int k_ver_triang;  // indice del vertice en la colección de vertices de los triangulos
    //int k_vertice_tapa;

    //int* vertices_triang = (int*) malloc( num_vertices_triang * sizeof(int));

    int** vertices_triang = (int**)malloc(*num_triangulos * sizeof(int*));
    for (int i = 0; i < *num_triangulos; i++) {
        vertices_triang[i] = (int*)malloc(3 * sizeof(int));
    }


    k_ver_triang = 0;

    for(k_rod = 0; k_rod < N_Rodajas - 1; k_rod++)
    {
        k_ver_rod = k_rod * M_PuntosPorRodaja;
        for( i_ver_rod = 0; i_ver_rod < M_PuntosPorRodaja - 1; i_ver_rod++)
        {
            vertices_triang[k_ver_triang][0] = k_ver_rod                     + i_ver_rod    ;
            vertices_triang[k_ver_triang][1] = k_ver_rod + M_PuntosPorRodaja + i_ver_rod    ;
            vertices_triang[k_ver_triang][2] = k_ver_rod + M_PuntosPorRodaja + i_ver_rod + 1;
            k_ver_triang++;
            vertices_triang[k_ver_triang][0] = k_ver_rod                     + i_ver_rod    ;
            vertices_triang[k_ver_triang][1] = k_ver_rod + M_PuntosPorRodaja + i_ver_rod + 1;
            vertices_triang[k_ver_triang][2] = k_ver_rod                     + i_ver_rod + 1;
            k_ver_triang++;

        }
    }

    for(k_cara = 0; k_cara < M_PuntosPorRodaja-1 ; k_cara++)
    {
            vertices_triang[k_ver_triang][0] = num_vertices_base - 2             ;
            vertices_triang[k_ver_triang][1] = k_cara                            ;
            vertices_triang[k_ver_triang][2] = k_cara + 1                        ;
            k_ver_triang++;
            vertices_triang[k_ver_triang][0] = num_vertices_base - 1             ;
            vertices_triang[k_ver_triang][1] = num_vertices_base - 3 - k_cara    ;
            vertices_triang[k_ver_triang][2] = num_vertices_base - 3 - k_cara - 1;
            k_ver_triang++;

    }

    //*num_vertices = M_PuntosPorRodaja * N_Rodajas;
    //return vertices_base;

    return vertices_triang;

}

int *creaIndicesArr(int M_PuntosPorRodaja,int N_Rodajas, int *num_vertices, int *num_triangulos)
{
    int caras               = (N_Rodajas-1) * ( M_PuntosPorRodaja - 1);
    int caras_tapa          =  M_PuntosPorRodaja - 1;
    *num_triangulos         = 2 * caras + 2 * caras_tapa;
    int num_vertices_base   = (N_Rodajas) * M_PuntosPorRodaja + 2;
    //int num_vertices_triang = 3 * (*num_triangulos);
    *num_vertices           = num_vertices_base;//num_vertices_triang;
    //int vertices_caras_lat  = M_PuntosPorRodaja * N_Rodajas;

    int k_rod, k_cara;
    int k_ver_rod;     // número de vertices antes de la rodaja
    int i_ver_rod;     // vertice en la rodaja
    int k_ver_triang;  // indice del vertice en la colección de vertices de los triangulos
    //int k_vertice_tapa;

    //int* vertices_triang = (int*) malloc( num_vertices_triang * sizeof(int));

    int* vertices_triang = (int*)malloc(*num_triangulos *3* sizeof(int*));

    k_ver_triang = 0;

    for(k_rod = 0; k_rod < N_Rodajas - 1; k_rod++)
    {
        k_ver_rod = k_rod * M_PuntosPorRodaja;
        for( i_ver_rod = 0; i_ver_rod < M_PuntosPorRodaja - 1; i_ver_rod++)
        {
            vertices_triang[k_ver_triang++] = k_ver_rod                     + i_ver_rod    ;
            vertices_triang[k_ver_triang++] = k_ver_rod + M_PuntosPorRodaja + i_ver_rod    ;
            vertices_triang[k_ver_triang++] = k_ver_rod + M_PuntosPorRodaja + i_ver_rod + 1;

            vertices_triang[k_ver_triang++] = k_ver_rod                     + i_ver_rod    ;
            vertices_triang[k_ver_triang++] = k_ver_rod + M_PuntosPorRodaja + i_ver_rod + 1;
            vertices_triang[k_ver_triang++] = k_ver_rod                     + i_ver_rod + 1;

        }
    }

    for(k_cara = 0; k_cara < M_PuntosPorRodaja-1 ; k_cara++)
    {
            vertices_triang[k_ver_triang++] = num_vertices_base - 2             ;
            vertices_triang[k_ver_triang++] = k_cara                            ;
            vertices_triang[k_ver_triang++] = k_cara + 1                        ;
            vertices_triang[k_ver_triang++] = num_vertices_base - 1             ;
            vertices_triang[k_ver_triang++] = num_vertices_base - 3 - k_cara    ;
            vertices_triang[k_ver_triang++] = num_vertices_base - 3 - k_cara - 1;

    }

    //*num_vertices = M_PuntosPorRodaja * N_Rodajas;
    //return vertices_base;

    return vertices_triang;

}
/*

float func_const( float x )
{
    return a;
}
int main()
{
    int M         = 3;
    int N_vueltas = 3;
    //float       a = 1.0f;
    int num_vertices,num_triang;
    VERTICE* v2;
    int k;
    int* v;

    v2=creaSolRev(func_const,1.1* a, -a,a,M,N_vueltas);

    v = creaIndicesArr(M,N_vueltas,&num_vertices,&num_triang);

    std::cout<<"numero de vertices: "<<num_vertices<<"\n";
    std::cout<<"numero de triangulos: "<<num_triang<<"\n";
    for(k = 0; k < num_triang; k++){


        std::cout<<v[k][0]<<" , "<<v[k][1]<<" , "<<v[k][2]<<"\n";
}

for(k=0;k<num_vertices;k++){
    std::cout<<v2[k].r<<" , "<<v2[k].g<<" , "<<v2[k].b<<"\n";
}

    return 0;
}
*/
