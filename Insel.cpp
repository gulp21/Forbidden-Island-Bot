/*
Bot für den sechsten Programmierwettbewerb von freiesMagazin („Die verbotene Insel“)
<http://www.freiesmagazin.de/sechster_programmierwettbewerb>
Copyright (C) 2013 Markus Brenneis

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include "Insel.h"

#include <QDebug>

Insel::Insel() {
	anzahlLandfelder_=0;
}

void Insel::zeileAnhaengen(QList<Feld> zeile) {
	insel_.append(zeile);
	for(int i=0; i<zeile.length(); i++) {
		if(zeile[i].gibStatus()==0) anzahlLandfelder_++;
	}
}

void Insel::fluten(int x, int y) {
	if(insel_[y-1][x-1].gibStatus()==0) anzahlLandfelder_--;
// 	qDebug() << insel_.length() << insel_[0].length() << x << y;
// 	qDebug() << "(DD)" << x << y << insel_[y-1][x-1].gibStatus();
	insel_[y-1][x-1].fluten();
}

void Insel::trockenlegen(int x, int y) {
	insel_[y-1][x-1].trockenlegen();
	anzahlLandfelder_++;
}

short int Insel::gibStatus(int x, int y) const {
	x--;
	y--;
	if(x<0 || y<0 || y>=insel_.length() || x>=insel_[0].length())
		return -1;
	return insel_[y][x].gibStatus();
}

bool Insel::warSchonmalGeflutet(int x, int y) const {
	x--;
	y--;
	if(x<0 || y<0 || y>=insel_.length() || x>=insel_[0].length())
		return false;
	return insel_[y][x].warSchonmalGeflutet();
}

int Insel::bewerteLohnenswertigkeit(int x, int y, bool spielIstFortgeschritten, int schritte) const {
	int lohnenswertigkeit=0;
	int status=gibStatus(x,y);
	bool schonmalGeflutet=warSchonmalGeflutet(x,y);
	
	// Zeilfeld betrachten
	if(status==2 || status==-1 || schritte>2) return -100;
	if(schonmalGeflutet && spielIstFortgeschritten) lohnenswertigkeit-=SCHWELLE_SCHONMAL_GEFLUTET;
	
	if(status==1) {
		lohnenswertigkeit+=LOHNENSWERTIGKEIT_GEFLUTET;
		if(!schonmalGeflutet)
			lohnenswertigkeit+=LOHNENSWERTIGKEIT_NIEGEFLUTET_SUMMAND;
	}
	
	// ist eine Ecke
	if(!spielIstFortgeschritten) { // wir sind etwas vorsichtiger, wenn das Spiel noch nicht weit fortgeschritten ist
		if(x==1 || y==1 || y==insel_.length() || x==insel_[0].length() ||
		   (gibStatus(x,y-1)==2 && gibStatus(x,y+1)==2 && (gibStatus(x-1,y)==2 || gibStatus(x+1,y)==2)) ||
		   (gibStatus(x+1,y)==2 && gibStatus(x-1,y)==2 && (gibStatus(x,y-1)==2 || (gibStatus(x,y+1)==2))) )
			lohnenswertigkeit-=SCHWELLE_ECKE;
	} else { // jetzt kann eh jeder Zug unser Ende bedeuten, also sind wir risikoreicher
		if( (x==1 && (y==1 || y==insel_.length())) || (x==insel_[0].length() && (y==1 || y==insel_.length())) ||
		    (gibStatus(x,y-1)==2 && gibStatus(x,y+1)==2 && (gibStatus(x-1,y)==2 || gibStatus(x+1,y)==2)) ||
		    (gibStatus(x+1,y)==2 && gibStatus(x-1,y)==2 && (gibStatus(x,y-1)==2 || (gibStatus(x,y+1)==2))) )
			lohnenswertigkeit-=SCHWELLE_ECKE;
	}
	
	// Umgebung betrachten
	status=gibStatus(x,y-1);
	schonmalGeflutet=warSchonmalGeflutet(x,y-1);
	if(status==1 && !schonmalGeflutet) {
		lohnenswertigkeit+=LOHNENSWERTIGKEIT_NIE_GEFLUTET;
		schritte++;
	}
	if(schritte==3) return lohnenswertigkeit;
	
	status=gibStatus(x,y+1);
	schonmalGeflutet=warSchonmalGeflutet(x,y+1);
	if(status==1 && !schonmalGeflutet) {
		lohnenswertigkeit+=LOHNENSWERTIGKEIT_NIE_GEFLUTET;
		schritte++;
	}
	if(schritte==3) return lohnenswertigkeit;
	
	status=gibStatus(x-1,y);
	schonmalGeflutet=warSchonmalGeflutet(x-1,y);
	if(status==1 && !schonmalGeflutet) {
		lohnenswertigkeit+=LOHNENSWERTIGKEIT_NIE_GEFLUTET;
		schritte++;
	}
	if(schritte==3) return lohnenswertigkeit;
	
	status=gibStatus(x+1,y);
	schonmalGeflutet=warSchonmalGeflutet(x+1,y);
	if(status==1 && !schonmalGeflutet) {
		lohnenswertigkeit+=LOHNENSWERTIGKEIT_NIE_GEFLUTET;
		schritte++;
	}
	if(schritte==3) return lohnenswertigkeit;
	
	// jetzt auch schonmal geflutete Felder nehmen
	status=gibStatus(x,y-1);
	if(status==1) {
		lohnenswertigkeit+=LOHNENSWERTIGKEIT_GEFLUTET;
		schritte++;
	}
	if(schritte==3) return lohnenswertigkeit;
	
	status=gibStatus(x,y+1);
	if(status==1) {
		lohnenswertigkeit+=LOHNENSWERTIGKEIT_GEFLUTET;
		schritte++;
	}
	if(schritte==3) return lohnenswertigkeit;
	
	status=gibStatus(x-1,y);
	if(status==1) {
		lohnenswertigkeit+=LOHNENSWERTIGKEIT_GEFLUTET;
		schritte++;
	}
	if(schritte==3) return lohnenswertigkeit;
	
	status=gibStatus(x+1,y);
	if(status==1) {
		lohnenswertigkeit+=LOHNENSWERTIGKEIT_GEFLUTET;
		schritte++;
	}
	if(schritte==3) return lohnenswertigkeit;
	
	return lohnenswertigkeit;
}

bool Insel::istSicher(int xAktuell, int yAktuell, int xNeu, int yNeu, bool spielIstFortgeschritten) const {
	if(gibStatus(xNeu,yNeu)==0)
		return (warSchonmalGeflutet(xAktuell,yAktuell) || !spielIstFortgeschritten || !warSchonmalGeflutet(xNeu,yNeu));
	return false;
}

bool Insel::darfBetretenWerden(int x, int y) const {
	return (gibStatus(x,y)==1 || gibStatus(x,y)==0);
}

int Insel::gibAnzahlLandfelder() const {
	return anzahlLandfelder_;
}

int Insel::gibHoehe() const {
	return insel_.length();
}

int Insel::gibBreite() const {
	return insel_[0].length();
}
