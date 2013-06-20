/*
Bot für den sechsten Programmierwettbewerb von freiesMagazin („Die verbotene Insel“)
<http://www.freiesmagazin.de/sechster_programmierwettbewerb>
Copyright (C) 2013 Markus Brenneis

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _Feld_
#define _Feld_

#include <QList>

class Feld {
	private:
		//! 0: trocken, 1: geflutet, 2: untergegangen;
		short status_;
		bool schonmalGeflutet_;
		
	public:
		//! erstellt eine neue Insel
		Feld(char status);
		void fluten();
		void trockenlegen();
		short gibStatus() const;
		bool warSchonmalGeflutet() const;
};

#endif // _Feld_
