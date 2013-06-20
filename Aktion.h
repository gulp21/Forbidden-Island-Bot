/*
Bot für den sechsten Programmierwettbewerb von freiesMagazin („Die verbotene Insel“)
<http://www.freiesmagazin.de/sechster_programmierwettbewerb>
Copyright (C) 2013 Markus Brenneis

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _Aktion_
#define _Aktion_

class Aktion {
	public:
		const short dX, dY;
		const std::string aktion;
		
		Aktion(short dX, short dY, std::string aktion) : dX(dX), dY(dY), aktion(aktion) {}
};

#endif // _Aktion_
