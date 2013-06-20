/*
Bot für den sechsten Programmierwettbewerb von freiesMagazin („Die verbotene Insel“)
<http://www.freiesmagazin.de/sechster_programmierwettbewerb>
Copyright (C) 2013 Markus Brenneis

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include "Insel.h"
#include "Bot.h"

#include <QDebug>
#include <QFile>
#include <QList>
#include <QStringList>

Insel* insel=0;
Bot* bot=0;

int main() {
	QFile qfStdin;
	qfStdin.open(stdin, QFile::ReadOnly);
	
	QString zeile;
	
	do { // while zeile!="GAMEBOARDEND"
		zeile=qfStdin.readLine();
		if(zeile.startsWith("GAMEBOARDSTART")) {
			QStringList werte=zeile.split(QRegExp("[ ,]"));
			insel=new Insel();
			for(int i=0; i<werte[2].toInt(); i++) {
				zeile=qfStdin.readLine();
				QList<Feld> inselzeile;
				for(int j=0; j<werte[1].toInt(); j++) {
					inselzeile.append(Feld(zeile[j].toLatin1()));
				}
				insel->zeileAnhaengen(inselzeile);
			} // for each Zeile
		} // if GAMEBOARDSTART
	} while(!zeile.startsWith("GAMEBOARDEND"));
	
	qfStdin.close();
	
	bot=new Bot(insel);
	bot->start();
	
	return 0;
}
