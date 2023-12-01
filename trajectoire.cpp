 #include "trajectoire.h"
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QtDebug>
#include <QObject>
#include<QDate>
#include<QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
using namespace QtCharts;

Trajectoire::Trajectoire()
{
}
Trajectoire::Trajectoire(int id_trajectoire,QDate date_heure_depart,QDate date_heure_fin,int x_gps,int y_gps,int duree,int vitesse)    //constructeure parametré

{
this->id_trajectoire=id_trajectoire;
this->date_heure_depart=date_heure_depart;
    this->date_heure_fin=date_heure_fin;
    this->x_gps=x_gps;
    this->y_gps=y_gps;
        this->duree=duree;
    this->vitesse=vitesse;
};
int Trajectoire::getid_trajectoire() {return id_trajectoire;}
QDate Trajectoire::getdate_heure_depart(){return date_heure_depart ;}
QDate Trajectoire::getdate_heure_fin(){return date_heure_fin;}
int Trajectoire::getx_gps (){return x_gps;}
int Trajectoire::gety_gps (){return y_gps;}
int Trajectoire::getduree (){return duree;}
int Trajectoire::getvitesse (){return vitesse;}


void Trajectoire::setid_trajectoire(int id_trajectoire){this->id_trajectoire=id_trajectoire;}
void Trajectoire::setdate_heure_depart(QDate  date_heure_depart){this->date_heure_depart=date_heure_depart;}
void Trajectoire::setdate_heure_fin(QDate date_heure_fin){this->date_heure_fin=date_heure_fin;}
void Trajectoire::setx_gps(int x_gps){this->x_gps=x_gps;}
void Trajectoire::sety_gps(int y_gps){this->y_gps=y_gps;}
void Trajectoire::setduree(int duree){this->duree=duree;}
void Trajectoire::setvitesse(int vitesse){this->vitesse=vitesse;}



bool Trajectoire::ajouter()
{//bool test=false;
    QSqlQuery query;


          query.prepare("INSERT INTO Trajectoire (id_trajectoire,date_heure_depart,date_heure_fin,x_gps,y_gps,duree,vitesse) "
                        "VALUES (:id_trajectoire,:date_heure_depart,:date_heure_fin,:x_gps,:y_gps,:duree,:vitesse)");
          query.bindValue(":id_trajectoire",id_trajectoire);
          query.bindValue(":date_heure_depart",date_heure_depart);
          query.bindValue(":date_heure_fin", date_heure_fin);
          query.bindValue(":x_gps", x_gps);
          query.bindValue(":y_gps", y_gps);
          query.bindValue(":duree", duree);
          query.bindValue(":vitesse", vitesse);


          return query.exec();   //execution de requette
};
QSqlQueryModel * Trajectoire::afficher()
{
    QSqlQueryModel * model= new QSqlQueryModel();

    model->setQuery("SELECT * FROM Trajectoire");  //afficher tous les colones
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id_trajectoire"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("date_heure_depart"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("date_heure_fin"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("x_gps"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("y_gps"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("duree"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("vitesse"));



    return model;
    }

bool Trajectoire::supprimer(int id_trajectoire)
{
    QSqlQuery query ;
    query.prepare("DELETE from Trajectoire where id_trajectoire =:id_trajectoire") ;
    query.bindValue(":id_trajectoire",id_trajectoire);
    return query.exec();
}
bool Trajectoire::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE Trajectoire SET id_trajectoire=:id_trajectoire, date_heure_depart=:date_heure_depart, date_heure_fin=:date_heure_fin, x_gps=:x_gps, y_gps=:y_gps, duree=:duree, vitesse=:vitesse WHERE id_trajectoire=:id_trajectoire");
    query.bindValue(":id_trajectoire", id_trajectoire);
    query.bindValue(":date_heure_depart", date_heure_depart);
    query.bindValue(":date_heure_fin", date_heure_fin);
    query.bindValue(":x_gps", x_gps);
    query.bindValue(":y_gps", y_gps);
    query.bindValue(":duree", duree);
    query.bindValue(":vitesse", vitesse);

    return query.exec(); // Exécution de la requête
}
QSqlQueryModel* Trajectoire::rechercher(QString a)
{
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("select * from Trajectoire where id_trajectoire like '"+a+"%'");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id_trajectoire"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("date_heure_depart"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("date_heure_fin"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("x_gps"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("y_gps"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("duree"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("vitesse"));
    return model;
}
QSqlQueryModel *Trajectoire::trier(QString x)
{
    QSqlQueryModel * model= new QSqlQueryModel();
    qDebug()<<x<<endl;

    if(x=="Id Trajectoire (Asc)")
        model->setQuery("select*  from Trajectoire order by id_trajectoire");
    else if(x=="Date debut (Asc)")
        model->setQuery("select*  from Trajectoire order by date_heure_depart");
    else if (x=="Date Fin (Des)")
        model->setQuery("select*  from Trajectoire order by date_heure_fin desc");
    else if (x=="Default")
            model->setQuery("select * from Trajectoire");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id_trajectoire"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("date_heure_depart"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("date_heure_fin"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("x_gps"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("y_gps"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("duree"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("vitesse"));
        return model;

}
bool Trajectoire::isValidEmail(QString email) {
    QRegularExpression regex("[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}");

       // We use QRegularExpressionMatch to test if the email matches the regex pattern
       QRegularExpressionMatch match = regex.match(email);

       // If the match is valid, return true. Otherwise, return false.
       return match.hasMatch();
}
