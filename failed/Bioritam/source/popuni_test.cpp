#include <iostream>
#include <iomanip>
#include <ios>
#include <ctime>
#include "bioritam.h"

using std::cout;           using std::cin;
using std::endl;           using std::exception;
using std::setprecision;   using std::streamsize;

int mainka(int argc, char** argv) {
   datum x[ BR_EL ];
   datum d, r;
   bioritam b[ BR_EL ];
   danas( d );
   r.dan = 16;
   r.mjesec = 12;
   r.godina = 1975;
   popuniX( x, b, d, r, LD );
   streamsize prec = cout.precision();
   cout << setprecision( 3 );
   for( int i = 0; i != BR_EL; ++i )
      cout << x[i].dan << ".\t" << x[i].mjesec << ".\t"
           << x[i].godina << ".\t\t" << b[i].fizicki << ".\t" 
           << b[i].emotivni << ".\t"
           << b[i].intelektualni << "." << endl;
   cout << setprecision( prec );
   return 0;
}