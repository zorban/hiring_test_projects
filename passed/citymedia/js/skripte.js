/* meni skripte */

/*
	struktura menija:

	--menu
		--node0
			--node00
				--node000
				--node001
				--parent = node0
				--leaf000 = "list 000"
			--node01
				--node010
				--leaf010 = "list 010"
				--parent = node0
			--leaf00 = "list 00"
			--parent = meni
		--node1
			--leaf10 = "list 10"
			--leaf11 = "list 11"
			--parent = meni
		--node2
			--parent = meni
		--leaf0 = "list 0"
		--leaf1 = "list 1"
		--leaf2 = "list 2"
		--parent = null
*/

// primjer menija
var json1 = '{ "node0" : { "leaf0_0": "Jedna mlada dama šeta uvek sama.", "leaf0_1": "Tko je to, što je to?", "leaf0_2": "Node0 - Leaf0_2" }, "node1":{ "node1_0": { "node1_0_0": { "leaf1_0_0_0": "Listić žuti - četvrto koljeno.", "leaf1_0_0_1": "Brat listića leaf1_0_0_0 (četvrto koljeno)." } } }, "leaf0": "Stari mudri list", "leaf1": "Brat starog mudrog lista leaf0.", "leaf2": "Brat starog mudrog lista leaf0 i ostalih listova istoga nivoa.", "leaf3": "Brat starog mudrog lista leaf1 i braće ostale svoje (mnogobrojne).", "leaf4": "Sestra (jedinica:)) listova 1, 2 i 3." }';

// Konstruktor
// parametar json__ je JSON string
// parametar okvir_duzina je broj stavki u meniju
function meni_objekat( json__, okvir_duzina ) {

	// nije moglo da se inicijalizuje prije definisanja funkcije "dodaj_tate" ??
	// zato idemo na dno našega konstruktora
	// this.meni = this.dodaj_tate( JSON.parse( json ), {} );

	// Ucitava meni iz JSON string-a
	this.ucitaj = function( json__ ) {
		this.meni = this.dodaj_tate( JSON.parse( json__ ), null, "" );
	}


	// Svakom objektu dodaje polje koje sadrzi parent objekat i ime aktuelnog objekta
	// da bismo lakše birali aktivni objekat kad idemo lijevo (ka roditelju)
	this.dodaj_tate = function( a, tata, sin_ime ) {
		for( b in a )
			if( typeof( a[b] ) == "object" && b != "tata__" )
				this.dodaj_tate( a[b], a, b );
		a.tata__ = tata;
		a.sin_ime__ = sin_ime;
		return a;
	}

	// Ovo je neki čobanin - budala
	this.stampaj_meni = function( a ) {
		for( b in a )
			if( typeof( a[b] ) == "object" && b != "tata__" ) {
				console.log( "node -> " + b );
				this.stampaj_meni( a[b] );
			}
			else if( b == "tata__" )
				console.log( "node -> " + b );
			else if( typeof( b ) == "string" )
				console.log( "leaf -> " + b + " = " + a[b] );
		return 0;
	}

	// Vadi potrebna polja iz aktivnog objekta
	this.polja = function( a ) {
		var i = 0;
		var c = new Array();
		for( b in a ) {
			// console.log( b + " -> " + a[b] );
			if( b != "tata__" && b != "sin_ime__" )
				c[i++] = b;
		}
		return c;
	}

	// Idemo jedno polje gore (ako ima gdje) u meniju
	this.gore = function() {
		//var indeks = this.polja_aktivnog_objekta.indexOf( this.aktivno_polje );
		var indeks = this.indeksOd( this.polja_aktivnog_objekta, this.aktivno_polje );
		if( indeks > 0 )
			this.aktivno_polje = this.polja_aktivnog_objekta[--indeks];

		if( indeks < this.okvir_start_indeks )
			this.okvir_start_indeks = indeks;
		return indeks;
	}

	// Idemo jedno polje dolje (ako ima gdje) u meniju
	this.dolje = function() {
		//var indeks = this.polja_aktivnog_objekta.indexOf( this.aktivno_polje );
		var indeks = this.indeksOd( this.polja_aktivnog_objekta, this.aktivno_polje );
		if( indeks < this.polja_aktivnog_objekta.length - 1 )
			this.aktivno_polje = this.polja_aktivnog_objekta[++indeks];

		if( indeks > this.okvir_start_indeks + this.okvir_duzina - 1 )
			this.okvir_start_indeks = indeks - this.okvir_duzina + 1;		
		return indeks;
	}

	// Nazad u nad-meni
	this.lijevo = function() {
		if( this.aktivni_objekat.tata__ == null )
			return -1;
		this.aktivno_polje = this.aktivni_objekat.sin_ime__;
		this.aktivni_objekat = this.aktivni_objekat.tata__;
		this.polja_aktivnog_objekta = this.polja( this.aktivni_objekat );

		//var indeks = this.polja_aktivnog_objekta.indexOf( this.aktivno_polje );
		var indeks = this.indeksOd( this.polja_aktivnog_objekta, this.aktivno_polje );
		var duzina = this.polja_aktivnog_objekta.length;
		this.okvir_start_indeks = duzina <= this.okvir_duzina ? 0 : ( duzina - indeks < this.okvir_duzina ? duzina - this.okvir_duzina : indeks ); 
		return 0;
	}

	// Naprijed u pod-meni
	this.desno = function() {
		if( typeof( this.aktivni_objekat[this.aktivno_polje] ) != "object" )
			return -1;
		this.aktivni_objekat = this.aktivni_objekat[this.aktivno_polje];
		this.polja_aktivnog_objekta = this.polja( this.aktivni_objekat );
		this.aktivno_polje = this.polja_aktivnog_objekta.length > 0 ? this.polja_aktivnog_objekta[0] : null;

		this.okvir_start_indeks = 0;
		return 0;
	}

	// Npr. duzina=10, duzina_okvira=6, aktivno polje ima indeks=6
	// Ako je indeks na pocetku ili kraju okvira onda zajedno
	// sa indeksom "šetamo" i okvir
	// _   _ | _   _   _   _   _   _ | _   _
	// 0   1   2   3   4   5   6   7   8   9
	//         ^               ^
	//  okvir_start_indeks   indeks


	// Vraća indeks od zadatog elementa u nizu
	// Nevjerevatno da IE ne podržava tako glup metod kakav je indexOf
	this.indeksOd = function( niz, element ) {
		if( typeof( niz.indexOf ) == "function" )
			return niz.indexOf( element );
		for( indeks in niz )
			if( niz[indeks] == element )
				return indeks * 1; // for uvijek vraca indeks tipa string pa ga mnozimo sa 1 da konvertujemo u number
		return false;              // string bi kasnije komplikovao stvari a nas indeks sadrzi samo cifre pa je ovo OK
	}


	// Daje sređen izlazni niz od n elemenata za prikaz
	// Izlaz je niz nizova sa imenima cvorova/listova,
	// sadrzajem listova i markiranim (true) aktivnim poljem
	this.izlaz = function() {
		var start_indeks = this.okvir_start_indeks;
		var izlaz__ = new Array();
		for( var i = 0; i < this.okvir_duzina; ++i ) {
			var tmp = new Array(); // svaki put novi objekat jer bi svi elementi izlaza pokazivali na isti (dodjeljuje se pointer)
			if( start_indeks + i < this.polja_aktivnog_objekta.length ) {
				var polje = this.polja_aktivnog_objekta[start_indeks + i];
				tmp[0] = typeof( this.aktivni_objekat[polje] ) == "object" ? "< " + polje + " >" : "< " + polje; // polje
				tmp[1] = polje == this.aktivno_polje ? true : false; // odabran
				tmp[2] = typeof( this.aktivni_objekat[polje] ) == "object" ? "" : this.aktivni_objekat[polje]; // sadrzaj
			}
			else {
				tmp[0] = tmp[2] = "";
				tmp[1] = false;
			}
			izlaz__[i] = tmp;
		}
		return izlaz__;
	}

	// Inicijalizacije
	this.meni = this.dodaj_tate( JSON.parse( json__ ), null, "" ); // meni je glavna promjeniljva koja sadrži meni
	this.aktivni_objekat = this.meni; // aktivni nod - elementima ovog noda je popunjen meni
	this.polja_aktivnog_objekta = this.polja( this.meni ); // elementi menija
	this.aktivno_polje = this.polja_aktivnog_objekta.length > 0 ? this.polja_aktivnog_objekta[0] : null; // aktivno polje u meniju
	this.okvir_start_indeks = 0; // od ovog indeksa počinje okvir (meni)
	this.okvir_duzina = typeof( okvir_duzina ) == "undefined" ? 6 : okvir_duzina; // broj stavki u okviru (meniju)
}


// Da probamo konacno objekat
window.onload = function() {

	/*
		Osvježavanje menija
	
		meni je ul sa id="meni"
		li-jevi sadrzi p elemente
		aktivni p ima class="odabran"
		desni div ima id="desno"
		i sadrži p element u koji ubacujemo 
		sadržaj leaf-ova
	
		izlaz[i][0] - polje
		izlaz[i][1] - odabran (true/false)
		izlaz[i][2] - sadrzaj
	*/
	function osvjezi_meni() {
		var j = 0;
		var ul = document.getElementById( "meni" ).children;
		var div_desno = document.getElementById( "desno" );
		var podaci = meni_moj.izlaz();
		if( div_desno.children.length > 0 )
			div_desno.removeChild( div_desno.children[0] );
		for( var i = 1; i < ul.length - 1; ++i ) { // od drugog do pretposlednjeg li-ja
			j = i - 1;
			ul[i].children[0].innerHTML = podaci[j][0];
			ul[i].children[0].className = podaci[j][1] ? "odabran" : "";
			if( podaci[j][2] != "" && podaci[j][1] ) {
				p = document.createElement( "p" );
				p.innerHTML = podaci[j][2];
				div_desno.appendChild( p );
			}
		}		
	}

	var meni_moj = new meni_objekat( json1 );
	osvjezi_meni();

	/*
		keykodovi strelica:
		gore:	38
		dolje:	40
		lijevo:	37
		desno:	39
	
		ESC keycode: 27

		nadam se da je isto i na Windowsu/MACu
	*/
	document.onkeyup = function() {
		//alert( typeof( event.keyCode ) );
		if( event.keyCode == null )
			return;
		var unicode = event.keyCode ? event.keyCode : event.charCode;
		switch( unicode ) {
			case 38:
				document.getElementById( "glavni_okvir" ).style.display = "block";
				meni_moj.gore();
				break;
			case 40:
				document.getElementById( "glavni_okvir" ).style.display = "block";
				meni_moj.dolje();
				break;
			case 37:
				document.getElementById( "glavni_okvir" ).style.display = "block";
				meni_moj.lijevo();
				break;
			case 39:
				document.getElementById( "glavni_okvir" ).style.display = "block";
				meni_moj.desno();
				break;
			case 27: // ESC
				document.getElementById( "glavni_okvir" ).style.display = "none";
		}
		osvjezi_meni();
		//alert(unicode);
	}

}

