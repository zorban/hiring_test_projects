#ifndef SAMO_BIORITAM_H
#define SAMO_BIORITAM_H

// bioritam.h
//    fizicki        23 dana
//    emotivni       28 dana
//    intelektualni  33 dana

// Periodi u sekundama
#define PERF_SEK     1987200      // Fizicki       23 * 24 * 60 * 60
#define PERE_SEK     2419200      // Emotivni      28 * 24 * 60 * 60
#define PERI_SEK     2851200      // Intelektualni 33 * 24 * 60 * 60

#define PI_         3.14159265358979323846

// true ako je x prestupna godina
#define PRESTUPNA( x ) ( ( !( x % 4 ) && x % 100 ) || !( x % 400 ) )

// Niz sa brojevima dana po mjesecima za neprestupne godine
static const unsigned int mjeseci[] = {
   31, 28, 31, 30,
   31, 30, 31, 31,
   30, 31, 30, 31
};

// Struktura koja sadrzi datum
struct datum {
   unsigned int dan, mjesec, godina;
};

// Struktura koja sadrzi bioritme
struct bioritam {
   float fizicki, emotivni, intelektualni;
};

// Broj dana od 1.1.1. do datum
unsigned int danaOdHrista( const datum& );

// Broj prestupnih godina do godine u parametru
unsigned int brPrestupnihDo( const unsigned int& );

// Broj dana u godini datum.godina do dana datum.dan datum.mjesec (aktuelni dan se ne racuna)
unsigned int danaOveGodine( const datum& );

// Racuna bioritam za dan (prvi parametar) ako je rodjendan (drugi parametar)
// Vraca float-e izmedju -1 i 1
bioritam bioritamZaDan( const datum&, const datum& );

// Funkcija koja postavlja argument na danasnji datum
// i vraca gresku pri pozivu funkcije localtime_s
int danas( datum& );

// Konstanta (broj dana) za popunjavanje kalendara lijevo i desno od aktuelnog datuma
// default je 16 jer je 2 * 16 + 1 period najduzeg (intelektualnog) bioritma
#ifndef LD
#define LD     16
#endif
#ifndef BR_EL
#define BR_EL ( 2 * LD + 1 )
#endif
// Popunjava niz datuma (prvi parametar) i niz odgovarajucih bioritama (drugi parametar)
// lijevo i desno po (peti parametar) elemenata kalendarskim danima i
// odgovarajucim bioritmima lijevo i desno od datuma (treci parametar)
// na osnovu rodjendana (cetvrti parametar)
// niz alocirati sa najmanje [2 * (treci parametar) + 1] elemenata
void popuniX( datum*, bioritam*, const datum&, const datum&, unsigned int );


// Vraca broj proteklih seknudi i milisekundi (timeb struktura)
// od datuma (parametri: datum, sati 0-23, minuti 0-59)
// do aktuelnog trenutka (trenutak poziva funkcije)
// sve se racuna po GMT+0 vremenu
struct __timeb64 secMilOd( const datum&, const unsigned short, const unsigned short );


#endif // SAMO_BIORITAM_H