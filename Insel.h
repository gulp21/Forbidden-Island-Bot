/*
Bot für den sechsten Programmierwettbewerb von freiesMagazin („Die verbotene Insel“)
<http://www.freiesmagazin.de/sechster_programmierwettbewerb>
Copyright (C) 2013 Markus Brenneis

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _Insel_
#define _Insel_

const int SCHWELLE_SCHONMAL_GEFLUTET=7;
const int SCHWELLE_ECKE=1;
const int LOHNENSWERTIGKEIT_GEFLUTET=6;
const int LOHNENSWERTIGKEIT_NIEGEFLUTET_SUMMAND=4;
const int LOHNENSWERTIGKEIT_NIE_GEFLUTET=LOHNENSWERTIGKEIT_GEFLUTET+LOHNENSWERTIGKEIT_NIEGEFLUTET_SUMMAND;

#include "Feld.h"
#include <QList>

class Insel {
	private:
		//! repräsentiert die Insel
		//! intern werden die Koordinaten von 0 bis n-1 gezaehlt, die öffentlichen Fkt. erwarteten Werte >0
		//! [zeile][spalte]
		QList< QList<Feld> > insel_;
		int anzahlLandfelder_;
		
	public:
		//! erstellt eine neue Insel
		Insel();
		void zeileAnhaengen(QList<Feld> zeile);
		void fluten(int x, int y);
		void trockenlegen(int x, int y);
		//! gibt -1 zurück, wenn das Feld nicht existiert
		short int gibStatus(int x, int y) const;
		bool warSchonmalGeflutet(int x, int y) const;
		int bewerteLohnenswertigkeit(int x, int y, bool spielIstFortgeschritten, int schritte) const;
		bool istSicher(int xAktuell, int yAktuell, int xNeu, int yNeu, bool spielIstFortgeschritten) const;
		bool darfBetretenWerden(int x, int y) const;
		int gibAnzahlLandfelder() const;
		int gibHoehe() const;
		int gibBreite() const;
};

#endif // _Insel_
