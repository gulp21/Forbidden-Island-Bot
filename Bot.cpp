/*
Bot für den sechsten Programmierwettbewerb von freiesMagazin („Die verbotene Insel“)
<http://www.freiesmagazin.de/sechster_programmierwettbewerb>
Copyright (C) 2013 Markus Brenneis

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include "Bot.h"

Bot::Bot(Insel* insel) {
	insel_=insel;
	stdin_.open(stdin, QFile::ReadOnly);
	gezogeneFlutkarten_=0;
	spielIstFortgeschritten_=false;
	
	// der die Summe der Indices zweier gegensätzlicher Aktionen ist 5
	a_.append(Aktion( 0, 0,"CURRENT"));
	a_.append(Aktion( 0,-1,"NORTH"));
	a_.append(Aktion(-1, 0,"WEST"));
	a_.append(Aktion(+1, 0,"EAST"));
	a_.append(Aktion( 0,+1,"SOUTH"));
}

void Bot::start() {
	QString zeile;
	
	do {
		zeile=stdin_.readLine();
		if(zeile.startsWith("ROUND")) {
			QStringList werte=zeile.split(QRegExp("[ ,]"));
			x_=werte[2].toInt();
			y_=werte[3].toInt();
			durchgefuehrteAktionen_=0;
			geheZuLohnenswertemFeld_();
			umgebungTrocknen_();
			// wir haben das gemacht, was sich am meisten lohnt, und wenn wird uns noch bewegen dürfen,
			// nutzen wir das, indem wir uns in Richtung Wasser bewegen
			geheRichtungWasser_();
			while(durchgefuehrteAktionen_<3) {
				std::cout << "GO CURRENT" << std::endl;
				durchgefuehrteAktionen_++;
			}
			if(insel_->gibStatus(x_,y_)==1)
				qDebug() << "(WW) Wieso stehen wir nicht im Trockenen?" << zeile.remove("\n") << x_ << y_;
		} else if(zeile.startsWith("FLOOD")) {
			QStringList werte=zeile.split(QRegExp("[ ,]"));
			insel_->fluten(werte[1].toInt(),werte[2].toInt());
		} else if(zeile.startsWith("INCRFLOOD")) {
			QStringList werte=zeile.split(" ");
			gezogeneFlutkarten_+=werte[1].toInt();
			aktualisiereSpielIstFortgeschritten_();
		}
	} while(!zeile.startsWith("END"));
}

void Bot::aktualisiereSpielIstFortgeschritten_() {
	spielIstFortgeschritten_=((insel_->gibAnzahlLandfelder())<=gezogeneFlutkarten_);
}

void Bot::geheZuLohnenswertemFeld_() {
	if(durchgefuehrteAktionen_==3) return;
	
	std::string aktion="CURRENT";
	int dX=0, dY=0;
	int lohnenswertigkeit=insel_->bewerteLohnenswertigkeit(x_,y_, spielIstFortgeschritten_, durchgefuehrteAktionen_);
	
	// mit einem Schritt zu erreichen
	int lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_,y_-1, spielIstFortgeschritten_, durchgefuehrteAktionen_+1);
	if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
		lohnenswertigkeit=lohnenswertigkeitAlternative;
		dX=0; dY=-1; aktion="NORTH";
	}
	lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_,y_+1, spielIstFortgeschritten_, durchgefuehrteAktionen_+1);
	if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
		lohnenswertigkeit=lohnenswertigkeitAlternative;
		dX=0; dY=+1; aktion="SOUTH";
	}
	lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_-1,y_, spielIstFortgeschritten_, durchgefuehrteAktionen_+1);
	if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
		lohnenswertigkeit=lohnenswertigkeitAlternative;
		dX=-1; dY=0; aktion="WEST";
	}
	lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_+1,y_, spielIstFortgeschritten_, durchgefuehrteAktionen_+1);
	if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
		lohnenswertigkeit=lohnenswertigkeitAlternative;
		dX=+1; dY=0; aktion="EAST";
	}
	
	// mit zwei Schritten zu erreichen
	if(insel_->gibStatus(x_,y_-1)!=2) {
		// gerade gehen
		lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_,y_-2, spielIstFortgeschritten_, durchgefuehrteAktionen_+2);
		if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
			lohnenswertigkeit=lohnenswertigkeitAlternative;
			dX=0; dY=-2; aktion="NORTH\nGO NORTH";
		}
		// um die Ecke gehen
		lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_-1,y_-1, spielIstFortgeschritten_, durchgefuehrteAktionen_+2);
		if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
			lohnenswertigkeit=lohnenswertigkeitAlternative;
			dX=-1; dY=-1; aktion="NORTH\nGO WEST";
		}
		lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_+1,y_-1, spielIstFortgeschritten_, durchgefuehrteAktionen_+2);
		if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
			lohnenswertigkeit=lohnenswertigkeitAlternative;
			dX=+1; dY=-1; aktion="NORTH\nGO EAST";
		}
	}
	if(insel_->gibStatus(x_,y_+1)!=2) {
		// gerade gehen
		lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_,y_+2, spielIstFortgeschritten_, durchgefuehrteAktionen_+2);
		if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
			lohnenswertigkeit=lohnenswertigkeitAlternative;
			dX=0; dY=+2; aktion="SOUTH\nGO SOUTH";
		}
		// um die Ecke gehen
		lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_-1,y_+1, spielIstFortgeschritten_, durchgefuehrteAktionen_+2);
		if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
			lohnenswertigkeit=lohnenswertigkeitAlternative;
			dX=-1; dY=+1; aktion="SOUTH\nGO WEST";
		}
		lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_+1,y_+1, spielIstFortgeschritten_, durchgefuehrteAktionen_+2);
		if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
			lohnenswertigkeit=lohnenswertigkeitAlternative;
			dX=+1; dY=+1; aktion="SOUTH\nGO EAST";
		}
	}
	if(insel_->gibStatus(x_-1,y_)!=2) {
		// gerade gehen
		lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_-2,y_, spielIstFortgeschritten_, durchgefuehrteAktionen_+2);
		if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
			lohnenswertigkeit=lohnenswertigkeitAlternative;
			dX=-2; dY=0; aktion="WEST\nGO WEST";
		}
		// um die Ecke gehen
		lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_-1,y_-1, spielIstFortgeschritten_, durchgefuehrteAktionen_+2);
		if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
			lohnenswertigkeit=lohnenswertigkeitAlternative;
			dX=-1; dY=-1; aktion="WEST\nGO NORTH";
		}
		lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_-1,y_+1, spielIstFortgeschritten_, durchgefuehrteAktionen_+2);
		if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
			lohnenswertigkeit=lohnenswertigkeitAlternative;
			dX=-1; dY=+1; aktion="WEST\nGO SOUTH";
		}
	}
	if(insel_->gibStatus(x_+1,y_)!=2) {
		// gerade gehen
		lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_+2,y_, spielIstFortgeschritten_, durchgefuehrteAktionen_+2);
		if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
			lohnenswertigkeit=lohnenswertigkeitAlternative;
			dX=+2; dY=0; aktion="EAST\nGO EAST";
		}
		// um die Ecke gehen
		lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_+1,y_-1, spielIstFortgeschritten_, durchgefuehrteAktionen_+2);
		if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
			lohnenswertigkeit=lohnenswertigkeitAlternative;
			dX=+1; dY=-1; aktion="EAST\nGO NORTH";
		}
		lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(x_+1,y_+1, spielIstFortgeschritten_, durchgefuehrteAktionen_+2);
		if(lohnenswertigkeitAlternative>lohnenswertigkeit) {
			lohnenswertigkeit=lohnenswertigkeitAlternative;
			dX=+1; dY=+1; aktion="EAST\nGO SOUTH";
		}
	}
	if(aktion!="CURRENT" && lohnenswertigkeit>0) {
		std::cout << "GO " << aktion << std::endl;
		x_+=dX; y_+=dY;
		durchgefuehrteAktionen_++;
		if(aktion.find("GO")!=std::string::npos) durchgefuehrteAktionen_++;
	}
}

void Bot::geheRichtungWasser_() {
	if(durchgefuehrteAktionen_>2) return;
	
	std::string aktionen[3]={"CURRENT","CURRENT","CURRENT"};
	int dX=0, dY=0;
	int lohnenswertigkeit=insel_->bewerteLohnenswertigkeit(x_,y_, spielIstFortgeschritten_, durchgefuehrteAktionen_);
	const int lohnenswertigkeitCurrent=lohnenswertigkeit;
	int lohnenswertigkeitAlternative=lohnenswertigkeit;

	for(int i=1; i<5; i++) {
		int xNeu=x_+a_[i].dX, yNeu=y_+a_[i].dY;
		if(weitergehenSinnvoll_(xNeu,yNeu,3)) {
			for(int j=1, dXi=a_[i].dX, dYi=a_[i].dY; j<5; j++) {
				int xNeu=x_+a_[j].dX+dXi, yNeu=y_+a_[j].dY+dYi;
				if(i+j!=5 && weitergehenSinnvoll_(xNeu,yNeu,2)) {
					for(int k=1, dXj=a_[j].dX+dXi, dYj=a_[j].dY+dYi; k<5; k++) {
						int xNeu=x_+a_[k].dX+dXj, yNeu=y_+a_[k].dY+dYj;
						if(j+k!=5 && i+k!=5 && weitergehenSinnvoll_(xNeu,yNeu,1)) {
							for(int l=1, dXk=a_[k].dX+dXj, dYk=a_[k].dY+dYj; l<5; l++) {
								int xNeu=x_+a_[l].dX+dXk, yNeu=y_+a_[l].dY+dYk;
								if(k+l!=5 && j+l!=5 && insel_->darfBetretenWerden(xNeu,yNeu)) {
									lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(xNeu,yNeu, spielIstFortgeschritten_, 0);
									if(lohnenswertigkeitAlternative>lohnenswertigkeitCurrent && lohnenswertigkeitAlternative>=lohnenswertigkeit && insel_->gibStatus(x_+a_[k].dX+dXj,y_+a_[k].dY+dYj)==0 && durchgefuehrteAktionen_<1) {
										lohnenswertigkeit=lohnenswertigkeitAlternative;
										// das bleibt auf Ebene 3 eingefroren, da wir uns ja nicht weiter bewegen dürfen
										dX=a_[k].dX+dXj; dY=a_[k].dY+dYj; aktionen[0]=a_[i].aktion; aktionen[1]=a_[j].aktion; aktionen[2]=a_[k].aktion;
									}
								}
							} // for l
							lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(xNeu,yNeu, spielIstFortgeschritten_, 0);
							if(lohnenswertigkeitAlternative>lohnenswertigkeitCurrent && lohnenswertigkeitAlternative>=lohnenswertigkeit && insel_->gibStatus(xNeu,yNeu)==0 && durchgefuehrteAktionen_<1) {
								lohnenswertigkeit=lohnenswertigkeitAlternative;
								dX=a_[k].dX+dXj; dY=a_[k].dY+dYj; aktionen[0]=a_[i].aktion; aktionen[1]=a_[j].aktion; aktionen[2]=a_[k].aktion;
							}
						}
					} // for k
					lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(xNeu,yNeu, spielIstFortgeschritten_, 0);
					if(lohnenswertigkeitAlternative>lohnenswertigkeitCurrent && lohnenswertigkeitAlternative>=lohnenswertigkeit && insel_->gibStatus(xNeu,yNeu)==0 && durchgefuehrteAktionen_<2) {
						lohnenswertigkeit=lohnenswertigkeitAlternative;
						dX=a_[j].dX+dXi; dY=a_[j].dY+dYi; aktionen[0]=a_[i].aktion; aktionen[1]=a_[j].aktion; aktionen[2]="CURRENT";
					}
				}
			} // for j
			lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(xNeu,yNeu, spielIstFortgeschritten_, 0);
			if(lohnenswertigkeitAlternative>lohnenswertigkeitCurrent && lohnenswertigkeitAlternative>=lohnenswertigkeit && insel_->gibStatus(xNeu,yNeu)==0) { // >=, damit kürzere Aktionen bevorzugt werden
				lohnenswertigkeit=lohnenswertigkeitAlternative;
				dX=a_[i].dX; dY=a_[i].dY; aktionen[0]=a_[i].aktion; aktionen[1]="CURRENT"; aktionen[2]="CURRENT";
			}
		}
	} // for i in a_
	
	if( (durchgefuehrteAktionen_==2 && aktionen[1]!="CURRENT") || (durchgefuehrteAktionen_==1 && aktionen[2]!="CURRENT")) {
		qDebug() << "(WW) Wir durften nicht alle Aktionen durchführen?" << aktionen[0].c_str() << aktionen[1].c_str() << aktionen[2].c_str();
	}
	
	if(aktionen[0]=="CURRENT" && durchgefuehrteAktionen_!=3) {
		maxRekursionTiefe_=MAX_REKURSION_TIEFE;
		int schritte=0;
		geheRichtungWasserRekursiv_(0,0,0, aktionen,lohnenswertigkeitCurrent,lohnenswertigkeit, dX,dY,0,0,0,0,schritte,0);
	}
	
	if(durchgefuehrteAktionen_!=3 && lohnenswertigkeit<=0) {
		maxRekursionTiefe_=MAX_REKURSION_TIEFE;
		int schritte=0;
		geheRichtungWasserRekursiv_(0,0,0, aktionen,lohnenswertigkeitCurrent,lohnenswertigkeit, dX,dY,0,0,0,0,schritte,0);
	}
	
	for(int i=0; i<3 && durchgefuehrteAktionen_<3 && aktionen[i]!="CURRENT"; i++) {
		std::cout << "GO " << aktionen[i] << std::endl;
		durchgefuehrteAktionen_++;
		if(durchgefuehrteAktionen_==3 && i<2 && aktionen[i+1]!="CURRENT") {
			qDebug() << "(WW) Aktionen";
			return; // TODO WARNING
		}
	}
	
	x_+=dX; y_+=dY; // TODO WARNING das ist falsch, wenn nicht alle Aktionen durchgeführt werden (wg. durchgefuehrteAktionen_==3) -> es muss abhängig von durchgefuehrteAktionen_ eingefroren werden (das kann und so lange egal sein, wie wir den Wert nachher nicht mehr brauchen)
	umgebungTrocknen_();
}

void Bot::geheRichtungWasserRekursiv_(const int i, const int j, const int k, std::string aktionen[3], const int lohnenswertigkeitCurrent, int &lohnenswertigkeit, int &dX, int &dY, int dXn2, int dYn2, const int n2, const int n1, int &schritte, const int tiefe) {
	if(tiefe==maxRekursionTiefe_) return;
	for(int n=1, dXn1=a_[n1].dX+dXn2, dYn1=a_[n1].dY+dYn2; n<5; n++) {
		int xNeu=x_+a_[n].dX+dXn1, yNeu=y_+a_[n].dY+dYn1;
		if(n1+n!=5 && n2+n!=5 && weitergehenSinnvoll_(xNeu,yNeu,1)) {
			if(tiefe>2) {
				geheRichtungWasserRekursiv_(i,j,k,aktionen,lohnenswertigkeitCurrent,lohnenswertigkeit, dX,dY,dXn1,dYn1,n1,n,schritte,tiefe+1);
			} else if(tiefe==2) {
				geheRichtungWasserRekursiv_(i,j,n,aktionen,lohnenswertigkeitCurrent,lohnenswertigkeit, dX,dY,dXn1,dYn1,n1,n,schritte,tiefe+1);
			} else if(tiefe==1) {
				geheRichtungWasserRekursiv_(i,n,0,aktionen,lohnenswertigkeitCurrent,lohnenswertigkeit, dX,dY,dXn1,dYn1,n1,n,schritte,tiefe+1);
			} else if(tiefe==0) {
				geheRichtungWasserRekursiv_(n,0,0,aktionen,lohnenswertigkeitCurrent,lohnenswertigkeit, dX,dY,dXn1,dYn1,n1,n,schritte,tiefe+1);
			}
			int lohnenswertigkeitAlternative=insel_->bewerteLohnenswertigkeit(xNeu,yNeu, spielIstFortgeschritten_, 0);
			if(lohnenswertigkeitAlternative>lohnenswertigkeitCurrent && (lohnenswertigkeitAlternative>lohnenswertigkeit || (tiefe+1<schritte && lohnenswertigkeitAlternative==lohnenswertigkeit))) {
				if(durchgefuehrteAktionen_==0 && insel_->gibStatus(x_+a_[i].dX+a_[j].dX+a_[k].dX,y_+a_[i].dY+a_[j].dY+a_[k].dY)==0) {
					// das bleibt auf Ebene 3 eingefroren, da wir uns ja nicht weiterbewegen dürfen
					dX=a_[i].dX+a_[j].dX+a_[k].dX; dY=a_[i].dY+a_[j].dY+a_[k].dY;
					aktionen[0]=a_[i].aktion; aktionen[1]=a_[j].aktion; aktionen[2]=a_[k].aktion;
					schritte=tiefe+1;
					lohnenswertigkeit=lohnenswertigkeitAlternative;
				} else if(durchgefuehrteAktionen_==1 && insel_->gibStatus(x_+a_[i].dX+a_[j].dX,y_+a_[i].dY+a_[j].dY)==0) {
					dX=a_[i].dX+a_[j].dX; dY=a_[i].dY+a_[j].dY;
					aktionen[0]=a_[i].aktion; aktionen[1]=a_[j].aktion; aktionen[2]="CURRENT";
					schritte=tiefe+1;
					lohnenswertigkeit=lohnenswertigkeitAlternative;
				} else if(durchgefuehrteAktionen_==2 && insel_->gibStatus(x_+a_[i].dX,y_+a_[i].dY)==0) {
					dX=a_[i].dX; dY=a_[i].dY;
					aktionen[0]=a_[i].aktion; aktionen[1]="CURRENT"; aktionen[2]="CURRENT";
					schritte=tiefe+1;
					lohnenswertigkeit=lohnenswertigkeitAlternative;
				}
			}
		}
	} // for n
}

bool Bot::weitergehenSinnvoll_(int xNeu, int yNeu, int maxDurchgefuehrteAktionen) const {
	return (insel_->darfBetretenWerden(xNeu,yNeu) && (durchgefuehrteAktionen_<maxDurchgefuehrteAktionen || insel_->istSicher(x_,y_,xNeu,yNeu,spielIstFortgeschritten_)));
}

void Bot::umgebungTrocknen_() {
	if(durchgefuehrteAktionen_>2) return;
	if(insel_->gibStatus(x_,y_)==1) {
		std::cout << "DRY CURRENT" << std::endl;
		insel_->trockenlegen(x_,y_);
		durchgefuehrteAktionen_++;
	}
	if(durchgefuehrteAktionen_>2) return;
	if(insel_->gibStatus(x_,y_-1)==1 && !insel_->warSchonmalGeflutet(x_,y_-1)) {
		std::cout << "DRY NORTH" << std::endl;
		insel_->trockenlegen(x_,y_-1);
		durchgefuehrteAktionen_++;
	}
	if(durchgefuehrteAktionen_>2) return;
	if(insel_->gibStatus(x_,y_+1)==1 && !insel_->warSchonmalGeflutet(x_,y_+1)) {
		std::cout << "DRY SOUTH" << std::endl;
		insel_->trockenlegen(x_,y_+1);
		durchgefuehrteAktionen_++;
	}
	if(durchgefuehrteAktionen_>2) return;
	if(insel_->gibStatus(x_-1,y_)==1 && !insel_->warSchonmalGeflutet(x_-1,y_)) {
		std::cout << "DRY WEST" << std::endl;
		insel_->trockenlegen(x_-1,y_);
		durchgefuehrteAktionen_++;
	}
	if(durchgefuehrteAktionen_>2) return;
	if(insel_->gibStatus(x_+1,y_)==1 && !insel_->warSchonmalGeflutet(x_+1,y_)) {
		std::cout << "DRY EAST" << std::endl;
		insel_->trockenlegen(x_+1,y_);
		durchgefuehrteAktionen_++;
	}
	if(durchgefuehrteAktionen_>2) return;
	if(insel_->gibStatus(x_,y_-1)==1) {
		std::cout << "DRY NORTH" << std::endl;
		insel_->trockenlegen(x_,y_-1);
		durchgefuehrteAktionen_++;
	}
	if(durchgefuehrteAktionen_>2) return;
	if(insel_->gibStatus(x_,y_+1)==1) {
		std::cout << "DRY SOUTH" << std::endl;
		insel_->trockenlegen(x_,y_+1);
		durchgefuehrteAktionen_++;
	}
	if(durchgefuehrteAktionen_>2) return;
	if(insel_->gibStatus(x_-1,y_)==1) {
		std::cout << "DRY WEST" << std::endl;
		insel_->trockenlegen(x_-1,y_);
		durchgefuehrteAktionen_++;
	}
	if(durchgefuehrteAktionen_>2) return;
	if(insel_->gibStatus(x_+1,y_)==1) {
		std::cout << "DRY EAST" << std::endl;
		insel_->trockenlegen(x_+1,y_);
		durchgefuehrteAktionen_++;
	}
}
