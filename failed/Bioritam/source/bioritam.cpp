// bioritam.cpp
// implementacija (definicije) funkcija deklarisanih u bioritam.h
//    fizicki        23 dana
//    emotivni       28 dana
//    intelektualni  33 dana

#include "DXUT.h"
#include <cmath>
#include <ctime>
#include <stdexcept>
#include <sys/timeb.h>
#include <sys/types.h>
#include "bioritam.h"

unsigned int danaOdHrista( const datum& d ) {
   return ( d.godina - 1) * 365 + brPrestupnihDo( d.godina - 1 ) + danaOveGodine( d );
}

unsigned int brPrestupnihDo( const unsigned int& godina ) {
   return godina / 4 - godina / 100 + godina / 400;
}

unsigned int danaOveGodine( const datum& d) {
   unsigned int dana = 0;
   for( unsigned int i = 0; i < d.mjesec - 1; ++i )
      dana += mjeseci[i];
   dana += d.dan - 1; // aktuelni dan nije prosao pa oduzimamo 1
   if( PRESTUPNA( d.godina ) && dana > 31 + 28 )   // ako je godina prestupna i dan je poslije 28 februara dodajemo jos 1 dan
      ++dana;
   return dana;
}

bioritam bioritamZaDan( const datum& d, const datum& r ) {
   bioritam b;
   unsigned int dana, danaF, danaE, danaI;
   if( d.godina < r.godina ||
       ( d.godina == r.godina && d.mjesec < r.mjesec ) ||
       ( d.godina == r.godina && d.mjesec == r.mjesec && d.dan < r.dan ) )
       throw std::domain_error( "bioritamZaDan: Datum mora biti poslije rodjendana." );
   dana = danaOdHrista( d ) - danaOdHrista( r );
   danaF = dana % 23; danaE = dana % 28; danaI = dana % 33;
   b.fizicki = (float)sin( 2.0 * PI_ * danaF / 23.0 );
   b.emotivni = (float)sin( 2 * PI_ * danaE / 28.0 );
   b.intelektualni = (float)sin( 2.0 * PI_ * danaI / 33.0 );
   return b;
}

int danas( datum& d ) {
   tm sad;
   time_t momenat;
   time( &momenat );
   errno_t et = localtime_s( &sad, &momenat );
   if ( et != 0 ) return et;
   d.dan = sad.tm_mday;
   d.mjesec = sad.tm_mon + 1;
   d.godina = sad.tm_year + 1900;
   return 0;
}

void popuniX( datum* x, bioritam* b, const datum& d, const datum& r,
              unsigned int lijevoDesno = 16 )
{
   unsigned int dan, mjesec, godina, d1, d2,
                poslednji, danaF, danaE, danaI;
   int dana;
   // Popunjavanje nizova x i b lijevo
   dan = d.dan; mjesec = d.mjesec; godina = d.godina;
   d1 = danaOdHrista( d ); d2 = danaOdHrista( r );
   if ( d2 > d1 )
      throw std::domain_error("popuniX: Datum mora biti poslije rodjendana.");
   dana = d1 - d2;
   for( int i = lijevoDesno; i != -1; --i ) {
      x[i].dan = dan; x[i].mjesec = mjesec; x[i].godina = godina;
      danaF = dana % 23; danaE = dana % 28; danaI = dana % 33;
      b[i].fizicki = (float)sin( 2.0 * PI_ * danaF / 23 );
      b[i].emotivni = (float)sin( 2.0 * PI_ * danaE / 28 );
      b[i].intelektualni = (float)sin( 2.0 * PI_ * danaI / 33 );
      if( --dana < 0 )
          b[i].fizicki = b[i].emotivni = b[i].intelektualni = 0.0f;
      if( --dan == 0 ) {
         if( --mjesec == 0 ) {
            mjesec = 12;
            if( --godina == 0 )
               throw std::domain_error("popuniX: A dokle brate?");
         }
         dan = mjeseci[ mjesec - 1 ];
         if( PRESTUPNA( godina ) && mjesec == 2 )
            dan = 29;
      }
   }
   // Popunjavanje nizova x i b desno
   dan = d.dan; mjesec = d.mjesec; godina = d.godina;
   dana = d1 - d2;
   for( unsigned int i = lijevoDesno + 1; i != 2 * lijevoDesno + 1; ++i ) {
      if( PRESTUPNA( godina ) && mjesec == 2 )
         poslednji = 29;
      else
         poslednji = mjeseci[ mjesec - 1 ];
      if( ++dan == poslednji + 1 ) {
         if( ++mjesec == 13 )
            mjesec = 1;
         dan = 1;
      }
      ++dana;
      danaF = dana % 23; danaE = dana % 28; danaI = dana % 33;
      b[i].fizicki = (float)sin( 2.0 * PI_ * danaF / 23 );
      b[i].emotivni = (float)sin( 2.0 * PI_ * danaE / 28 );
      b[i].intelektualni = (float)sin( 2.0 * PI_ * danaI / 33 );
      x[i].dan = dan; x[i].mjesec = mjesec; x[i].godina = godina;
   }
}

struct __timeb64 secMilOd( const datum& d, const unsigned short sati,
                          const unsigned short minuti )
{
    datum unixStart = { 1, 1, 1970 };
    __timeb64 momenat;
    int d1 = danaOdHrista( unixStart );
    int d2 = danaOdHrista( d );
    int d3 = d2 - d1;
    _ftime( &momenat );
    if( d3 > 0 )
        momenat.time -= d3 * 24 * 60 * 60 + sati * 60 * 60 + minuti * 60;
    else
        momenat.time += abs( d3 ) * 24 * 60 * 60 - sati * 60 * 60 - minuti * 60;
    return momenat;
}
