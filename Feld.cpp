/*
Bot für den sechsten Programmierwettbewerb von freiesMagazin („Die verbotene Insel“)
<http://www.freiesmagazin.de/sechster_programmierwettbewerb>
Copyright (C) 2013 Markus Brenneis

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include "Feld.h"

#include <QDebug>

Feld::Feld(char status) {
	if(status=='#') {
		status_=0;
	} else if(status=='o') {
		status_=1;
	} else if(status=='.') {
		status_=2;
	} else {
		qDebug() << "(DD) WARNUNG: Feldstatus" << status << "unbekannt.";
	}
// 	qDebug() << status_;
	schonmalGeflutet_=false;
}

void Feld::fluten() {
	status_++;
	schonmalGeflutet_=true;
	if(status_>2) qDebug() << "(DD) WARNUNG: Feldstatus" << status_ << "größer 2.";
}

void Feld::trockenlegen() {
	if(status_!=1) qDebug() << "(DD) WARNUNG: Feldstatus" << status_ << "ist nicht 1.";
	status_=0;
}

short Feld::gibStatus() const {
	return status_;
}

bool Feld::warSchonmalGeflutet() const {
	return schonmalGeflutet_;
}
