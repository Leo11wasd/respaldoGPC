// planos.cpp : Defines the entry point for the console application.
//

/* En este c�digo usted debe agregar las sentencias
   necesarias ( funciones, operadores y script de pruebas )
   para:
   // EJER 1) operador de entrada del clsVector3D
   // EJER 2) operador Producto Cruz de dos clsVector3D
   // EJER 3) Construir el plano a partir de los tres puntos v1, v2 y v3.
   // EJER 4) Impresi�n del plano A * x + B * y + C * z + D = 0.
   // EJER 5) Lectura del plano como A * x + B * y + c * z + D = 0.
   // EJER 6) Obtener el punto que resulta de la intersecci�n de tres planos

  En los casos de los operadores de lectura agregue lectura de input streams
  definidos en base a cadenas de caracteres, de tal forma que pueda probar
  de manera r�pida sus operadores.

*/
using namespace std;
#include <iostream>
// ============================================
class clsVector3D
{
private:
  double dbl_x, dbl_y, dbl_z;

public:
  clsVector3D()
  {
    dbl_x = dbl_y = dbl_z = 0.0;
  }
  void x(double unX) { dbl_x = unX; }
  double x() { return dbl_x; }

  void y(double unY) { dbl_y = unY; }
  double y() { return dbl_y; }

  void z(double unZ) { dbl_z = unZ; }
  double z() { return dbl_z; }

  friend ostream &operator<<(ostream &os, clsVector3D &p)
  {
    os << "( " << p.x() << "," << p.y() << "," << p.z() << " )";
    return os;
  }

  friend clsVector3D operator+(clsVector3D a, clsVector3D b)
  {
    clsVector3D s;
    s.x(a.x() + b.x());
    s.y(a.y() + b.y());
    s.z(a.z() + b.z());

    return s;
  }

  friend clsVector3D operator*(double esc, clsVector3D v)
  {
    clsVector3D w;

    w.x(esc * v.x());
    w.y(esc * v.y());
    w.z(esc * v.z());

    return w;
  }

  friend clsVector3D operator*(clsVector3D v, double esc)
  {
    return esc * v;
  }

  // EJER 1) operador de entrada del clsVector3D:
  friend istream &operator>>(istream &is, clsVector3D &v)
  {
    typedef enum
    {
      S0,
      S1,
      S2,
      S3,
      S4,
      SF,
      SERROR
    } enumLeeListaTopo;

    enumLeeListaTopo s;
    char c;
    int x, y, z;

    s = S0;
    while (s != SF && s != SERROR)
    {
      is >> c;
      while (c == ' ')
        is >> c;
      is.putback(c);
      try
      {
        switch (s)
        {
        case S0:
          if (is.eof())
            s = SF;
          else
          {
            is >> x;
            s = S1;
          }
          break;
        case S1:
          is >> c;
          s = (c == ',') ? S2 : SERROR;
          break;
        case S2:
          is >> y;
          s = S3;
          break;
        case S3:
          is >> c;
          s = (c == ',') ? S4 : SERROR;
          break;
        case S4:
          is >> z;
          v.dbl_x = x;
          v.dbl_y = y;
          v.dbl_z = z;
          s = S0;
          break;
        }
      }
      catch (...)
      {
        s = SERROR;
      }
    }

    return is;
  }
  // EJER 2) operador Producto Cruz de dos clsVector3D
  friend clsVector3D operator*(clsVector3D &v1, clsVector3D &v2)
  {
    // por lo pronto un dummy que regresa el ( -1.0,-2.0,-3.0 )
    clsVector3D w;
    w.dbl_x = v1.dbl_y + v2.dbl_z - v2.dbl_y * v1.dbl_z;
    w.dbl_y = v2.dbl_x * v1.dbl_z - v1.dbl_x * v2.dbl_z;
    w.dbl_z = v1.dbl_x * v2.dbl_y - v1.dbl_y * v2.dbl_x;
    return w;
  }
  friend ostream &operator<<(ostream &os, const clsVector3D &v)
  {
    os << "( " << v.dbl_x << "," << v.dbl_y << "," << v.dbl_z << " )";
    return os;
  }
};
// =============================================
class clsPlano3D
{
private:
  double dbl_A, dbl_B, dbl_C, dbl_D; // representa el plano Ax + By + Cz + D = 0.
public:
  clsPlano3D()
  {
    dbl_A = dbl_B = dbl_C = dbl_D = 0.0;
  }

  // EJER 3)  Construir el plano a partir de los tres puntos v1, v2 y v3.
  clsPlano3D(clsVector3D v1, clsVector3D v2, clsVector3D v3)
  { // por lo pronto un dummy que entrega el 5x + 5y + 5z + 5 = 0.
    dbl_A = dbl_B = dbl_C = dbl_D = 5.0;
  }

  void A(double unA) { dbl_A = unA; }
  double A() { return dbl_A; }

  void B(double unB) { dbl_B = unB; }
  double B() { return dbl_B; }

  void C(double unC) { dbl_C = unC; }
  double C() { return dbl_C; }

  // EJER 4) Impresi�n del plano A * x + B * y + c * z + D = 0.
  friend ostream &operator<<(ostream &os, clsPlano3D &p)
  {
    os << p.dbl_A<<"x + "<<p.dbl_B<<"y + "<<p.dbl_C<<"z + "<<p.dbl_D <<" = 0"<< endl;
    return os;
  }

  // EJER 5) Lectura del plano como A * x + B * y + c * z + D = 0.
  friend istream &operator>>(istream &is, clsPlano3D &p)
  {
    clsPlano3D w;
    p = w;
    return is;
  }

  // EJER 6) Obtener el punto que resulta de la intersecci�n de tres planos
  friend clsVector3D interseccion(clsPlano3D P1, clsPlano3D P2, clsPlano3D P3)
  {
    // va el dummy con el ( 100.0 ,200.0, 300.0)
    clsVector3D a;
    a.x(100.0);
    a.y(200.0);
    a.z(300.0);
    return a;
  }
};
// =============================================
int main()
{
  clsVector3D a, b, c;

  a.x(1.0);
  a.y(2.0);
  a.z(3.0);

  b.x(-2.0);
  b.y(-5.0);
  b.z(-7.0);

  cout << a << " + " << b << " = " << a + b << endl;

  cout << 5.0 << " * " << a << " = " << 5.0 * a << endl;

  cout << a << " * " << 5.0 << " = " << a * 5.0 << endl;

  cin >> c;

  cout << "c = " << c << endl;

  cout << " a * b = " << a << " * " << b << " = " << a * b << endl;

  clsPlano3D q1, q2, q3;

  cout << q1 << endl;

  cout << "Interseccion " << interseccion(q1, q2, q3) << endl;
  return 0;
}
// ========================================================
