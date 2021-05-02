// Testiranje koda...
#include <iostream>
#include <ctime>
#include <stdexcept>
#include <sys/timeb.h>
#include <sys/types.h>
#include "bioritam.h"

using std::cout;  using std::cin;
using std::endl;  using std::exception;

int main(int argc, char** argv) {
   char* p = " nije";
   struct datum d, d1;
   //d1.dan = 16; d1.mjesec = 12; d1.godina = 1975;
   danas( d );
   bioritam b;
   cout << "Unesi datum za rodjendan (dan mjesec godina): ";
   cin >> d1.dan >> d1.mjesec >> d1.godina;
   if( PRESTUPNA( d.godina ) ) p = " je";
   cout << "godina " << d.godina << p << " prestupna" << endl;

   cout << "Dana od 1.1.1: " << danaOdHrista( d1 ) << "." << endl;
   cout << "Broj prestupnih od 0001 do " << d.godina << " je " << brPrestupnihDo( d.godina ) << endl;
   cout << "Proteklih dana u godini " << d.godina << " do datuma " << d.dan << "." << d.mjesec << " ima " 
             << danaOveGodine( d ) << "." << endl;
   try {
      b = bioritamZaDan( d, d1 );
   }
   catch( exception e ) {
      cout << e.what() << endl;
      return 1;
   }
   cout << "Bioritam za dan " << d.dan << "." << d.mjesec << "." << d.godina << ". je: " << endl
             << "Fizicki: " << (int)( b.fizicki * 100 ) << endl
             << "Emotivni: " << (int)( b.emotivni * 100 ) << endl
             << "Intelektualni: " << (int)( b.intelektualni ) * 100 << endl
             << "Dana od rodjenja: " << danaOdHrista( d ) - danaOdHrista( d1 ) << "." << endl;
   cout << "ceil(1.61) = " << ceil( 1.61 ) << endl
             << "floor(1.61) = " << floor( 1.61 ) << endl
             << "ceil(1.47) = " << ceil( 1.47 ) << endl
             << "floor(1.47) = " << floor( 1.47 ) << endl;


   tm sad;
   time_t momenat;
   time( &momenat );
   errno_t et = localtime_s( &sad, &momenat );
   cout << "errno_t return od localtime_s: " << et << endl;

   cout << "Godina: " << sad.tm_year + 1900 << endl
             << "Mjesec: " << sad.tm_mon + 1 << endl
             << "Dan: " << sad.tm_mday << endl
             << "DayLightSavings: " << sad.tm_isdst << endl;
   datum d3;
   danas( d3 );
   cout << "Dan: " << d3.dan << endl
             << "Mjesec: " << d3.mjesec << endl
             << "Godina: " << d3.godina << endl;
   datum rodj = { 31, 12, 1969 };
   __timeb64 momentum;
   _ftime( &momentum );
   __timeb64 sekmil = secMilOd( rodj, 23, 59 );
   cout << "Sekunde i milisekunde od 31. 12. 1969. 23:59:" << endl
        << "Sekundi: " << sekmil.time << endl
        << "Miliseknudi: " << sekmil.millitm << endl
        << "Sekundi od 1. 1. 1970: " << momentum.time << endl;


   return 0;
}