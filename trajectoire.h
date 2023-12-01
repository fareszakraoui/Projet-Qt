#ifndef TRAJECTOIRE_H
#define TRAJECTOIRE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

#include <QString>
#include <QSqlQuery>
#include <QtDebug>
#include <qmessagebox.h>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <algorithm>
#include <QVariant>
#include <QtSql>
#include <QPdfWriter>
#include <QPainter>
#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QValidator>
#include <QRegExp>
#include <QRegExpValidator>
#include <QStandardItemModel>
#include <QLabel>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtPrintSupport/QPrinter>
#include <QLineSeries>
//mail
#include <QByteArray>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslSocket>

#include <QtCore/QTextStream>

#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>
class Trajectoire
{
public:
    Trajectoire(); //constructeur par defaut

    Trajectoire(int,QDate,QDate,int,int,int,int);    //constructeure parametré
                                   //constructeur par défaut
int getid_trajectoire();
QDate getdate_heure_depart();
QDate getdate_heure_fin();
int getx_gps();
int gety_gps();
int getvitesse();

int getduree();


void setid_trajectoire(int);
void setdate_heure_depart(QDate);
void setdate_heure_fin(QDate);
void setx_gps(int);
void sety_gps(int);
void setvitesse(int);
void setduree(int);


bool ajouter();
QSqlQueryModel * afficher();
bool supprimer(int);
bool modifier();
QSqlQueryModel *rechercher(QString);
    QSqlQueryModel *trier(QString);
    bool isValidEmail(QString email);

private :
int id_trajectoire;
QDate date_heure_depart,date_heure_fin;
int x_gps,y_gps,duree,vitesse;
};

#endif // TRAJECTOIRE_H
