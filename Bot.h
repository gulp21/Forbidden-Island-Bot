/*
Bot für den sechsten Programmierwettbewerb von freiesMagazin („Die verbotene Insel“)
<http://www.freiesmagazin.de/sechster_programmierwettbewerb>
Copyright (C) 2013 Markus Brenneis

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _Bot_
#define _Bot_

#include <QDebug>
#include <QFile>
#include <QStringList>
#include <iostream>

#include "Insel.h"
#include "Aktion.h"

const int MAX_REKURSION_TIEFE=12;

class Bot {
	private:
		//! aktuelle Position des Bots
		int x_, y_, durchgefuehrteAktionen_, gezogeneFlutkarten_, maxRekursionTiefe_;
		bool spielIstFortgeschritten_;
		Insel *insel_;
		QFile stdin_;
		QList<Aktion> a_;
		
		void geheZuLohnenswertemFeld_();
		//! trocknet C; N,S,W,E, bevorzugt Felder, die nie geflutet wurden
		void umgebungTrocknen_();
		void geheRichtungWasser_();
		void aktualisiereSpielIstFortgeschritten_();
		/*!
		 * @param i,j,k: Index der ersten drei durchzuführenden Aktionen
		 * @param aktionen[3]: Speichert diese Aktionen
		 * @param lohnenswertigkeitCurrent: Lohnenswertigkeit des Feldes, auf dem der Bot gerade ist
		 * @param lohnenswertigkeit: Lohnenswertigkeit des bisher als am besten besten bewerteten Feldes
		 * @param dX,dY: Differenz der Koordinaten dieses Felder zu der aktuellen Botposition
		 * @param dXn2,dYn2: dx,dY zwei Rekursionsstufen höher
		 * @param n2,n1: Indices der Aktionen zwei bzw. eine Rekursionsstufen höher
		 * @param schritte: Anzahl der benötigten Schritte, um das Feld mit @param lohnenswertigkeit zu erreichen
		 * @param tiefe: Rekursionstiefe
		**/
		void geheRichtungWasserRekursiv_(const int i, const int j, const int k, std::string aktionen[3], const int lohnenswertigkeitCurrent, int &lohnenswertigkeit, int &dX, int &dY, int dXn2, int dYn2, const int n2, const int n1, int &schritte, const int tiefe);
		bool weitergehenSinnvoll_(int xNeu, int yNeu, int maxDurchgefuehrteAktionen) const;
		
	public:
		//! erstellt einen neuen Bot auf der Insel @param insel
		Bot(Insel* insel);
		void start();
};

#endif // _Bot_
