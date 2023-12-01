#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <qDebug>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QPdfWriter>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include "smtp.h"
#include <QtCharts/QChart>
#include <QPixmap>
#include <QMessageBox>
#include <QComboBox>
#include <QFileDialog>
#include <QPainter>
#include <QTextStream>
#include <QTextDocument>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts>
#include <QPieSlice>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWidget>
#include <QUrl>
#include <QQmlContext>

#include <QtWidgets/QWidget>
#include <QtCharts/QChart>
#include <QQuickItem>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tab_trajectoire->setModel(T.afficher());
    ui->comboBox_modif->setModel(T.afficher());
    ui->comboBox_supp->setModel(T.afficher());


    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->quickWidget->show();

    auto obj = ui->quickWidget->rootObject();
    connect(this, SIGNAL(setCenter(QVariant, QVariant)), obj, SLOT(setCenter(QVariant, QVariant)));
    connect(this, SIGNAL(addMarker(QVariant, QVariant)), obj, SLOT(addMarker(QVariant, QVariant)));


    emit setCenter(36.83,10.21);
    emit addMarker(36.83,10.20);
    emit addMarker(36.82,10.21);
    emit addMarker(36.81,10.20);
    emit addMarker(36.898,10.190);
    //initialiser arduino

        int ret = A.connect_arduino();
           switch(ret)
           {
           case (0): qDebug() << "arduino is available and connected to"<<A.getarduino_port_name();

              break;

             case (1):  qDebug() << "arduino is available but not  connected to"<<A.getarduino_port_name();
               break;
           case (-1):
                qDebug() << "arduino is not  available";
           }
}

MainWindow::~MainWindow()
{
    delete ui;
}


//Trajectoire::Trajectoire(int id_trajectoire,QDate date_heure_depart,QDate date_heure_fin,int x_gps,int y_gps,int duree,int vitesse)    //constructeure parametré

void MainWindow::on_pb_ajouter_clicked()
{
    // Réinitialisez les étiquettes d'erreur à vide
    ui->lblErreurId->setText("");
    ui->lblErreurduree->setText("");
    ui->lblErreurx_gps->setText("");
    ui->lblErreury_gps->setText("");
    ui->lblErreurvitesse->setText("");

    // Récupérez les valeurs d'entrée
    QString str_id_trajectoire = ui->le_id_trajectoire->text();
    int id_trajectoire=str_id_trajectoire.toInt();

    QDate date_heure_depart = ui->le_date_heure_depart->date();
    QDate date_heure_fin = ui->le_date_heure_fin->date();

    QString str_x_gps = ui->le_x_gps->text();
    int x_gps=str_x_gps.toInt();

    QString str_y_gps = ui->le_y_gps->text();
    int y_gps=str_y_gps.toInt();

    QString str_duree=ui->le_duree->text();
    int duree=str_duree.toInt();

    QString str_vitesse=ui->le_vitesse->text();
    int vitesse=str_vitesse.toInt();

    // Validez les champs d'entrée
    bool isValid = true;

    if (str_id_trajectoire.isEmpty()) {
        ui->lblErreurId->setText("<font color='red'>ID Trajectoire ne peut pas être vide!!!!</font>");
        isValid = false;
    }

    // Validation pour le titre (exemplaire, assurez-vous qu'il n'est pas vide)
    if (str_x_gps.isEmpty()) {
        ui->lblErreurx_gps->setText("<font color='red'>x gps ne peut pas être vide!!!</font>");
        isValid = false;
    }

    if (str_y_gps.isEmpty()) {
        ui->lblErreury_gps->setText("<font color='red'>y gps ne peut pas être vide!!!</font>");
        isValid = false;
    }
    if (str_vitesse.isEmpty()) {
        ui->lblErreurvitesse->setText("<font color='red'>Vitesse ne peut pas être vide!!!</font>");
        isValid = false;
    }
if (str_duree.isEmpty())
{
       ui->lblErreurduree->setText("<font color='red'>Duree ne peut pas être vide!!!</font>");
       isValid = false;
   }


    // Ajouter une vérification de l'existence de l'ID dans la base de données
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id_trajectoire FROM Trajectoire WHERE id_trajectoire = :id_trajectoire");
    checkQuery.bindValue(":id_trajectoire", id_trajectoire);
    if (checkQuery.exec() && checkQuery.next()) {
        ui->lblErreurId->setText("<font color='red'>ID Trajectoire existe déjà!!!!</font>");
        isValid = false;
    }


    if (isValid) {
        // Si l'entrée est valide et que l'ID n'existe pas, procédez à l'ajout de l'activité
        Trajectoire T(id_trajectoire, date_heure_depart, date_heure_fin, x_gps, y_gps,duree,vitesse);

        QSqlQuery query;
        bool test = T.ajouter();
        if (test) {
            ui->tab_trajectoire->setModel(T.afficher());
            ui->comboBox_modif->setModel(T.afficher());
            ui->comboBox_supp->setModel(T.afficher());
            QMessageBox::information(nullptr, QObject::tr("Ajout Trajectoire"),
                QObject::tr("Trajectoire ajoutée avec succès.\n"
                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);




        } else {
            QMessageBox::critical(nullptr, QObject::tr("Ajout Trajectoire"),
                QObject::tr("Erreur lors de l'ajout de Trajectoire.\n"
                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        }
    } else {
        // Gérez l'erreur d'entrée invalide ou d'ID existant
    }
}


void MainWindow::on_pb_modifier_clicked()
{
    QString str_id_trajectoire = ui->comboBox_modif->currentText();
    int id_trajectoire = str_id_trajectoire.toInt();
    if (id_trajectoire == 0) {
        QMessageBox::critical(nullptr, QObject::tr("Modifier Trajectoire"),
            QObject::tr("ID trajectoire invalide.\n"
                        "Veuillez saisir un ID valide."), QMessageBox::Ok);
        return;
    }

    QDate date_heure_depart = ui->le_date_heure_depart2->date();
    QDate date_heure_fin = ui->le_date_heure_fin->date();

    QString str_x_gps = ui->le_x_gps2->text();
    int x_gps = str_x_gps.toInt();
    if (x_gps == 0) {
        QMessageBox::critical(nullptr, QObject::tr("Modifier Trajectoire"),
            QObject::tr("Coordonnée X GPS invalide.\n"
                        "Veuillez saisir une valeur numérique valide."), QMessageBox::Ok);
        return;
    }

    QString str_y_gps = ui->le_y_gps2->text();
    int y_gps = str_y_gps.toInt();
    if (y_gps == 0) {
        QMessageBox::critical(nullptr, QObject::tr("Modifier Trajectoire"),
            QObject::tr("Coordonnée Y GPS invalide.\n"
                        "Veuillez saisir une valeur numérique valide."), QMessageBox::Ok);
        return;
    }

    QString str_duree = ui->le_duree2->text();
    int duree = str_duree.toInt();
    if (duree == 0) {
        QMessageBox::critical(nullptr, QObject::tr("Modifier Trajectoire"),
            QObject::tr("Durée invalide.\n"
                        "Veuillez saisir une valeur numérique valide."), QMessageBox::Ok);
        return;
    }

    QString str_vitesse = ui->le_vitesse2->text();
    int vitesse = str_vitesse.toInt();
    if (vitesse == 0) {
        QMessageBox::critical(nullptr, QObject::tr("Modifier Trajectoire"),
            QObject::tr("Vitesse invalide.\n"
                        "Veuillez saisir une valeur numérique valide."), QMessageBox::Ok);
        return;
    }

    // Vérification si l'ID existe
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM Trajectoire WHERE id_trajectoire = :id_trajectoire");
    checkQuery.bindValue(":id_trajectoire", id_trajectoire);

    if (!checkQuery.exec())
    {
        // Erreur lors de l'exécution de la requête
        qDebug() << "Erreur lors de la vérification de l'ID :" << checkQuery.lastError().text();
        return;
    }

    if (!checkQuery.next())
    {
        // L'ID n'existe pas, affichez un message d'erreur
        QMessageBox::critical(nullptr, QObject::tr("Modifier Trajectoire"),
            QObject::tr("L'ID n'existe pas dans la base de données.\n"
                        "Click Cancel to exit."), QMessageBox::Cancel);
        return;
    }

    // L'ID existe, vous pouvez maintenant effectuer la modification
    Trajectoire T(id_trajectoire, date_heure_depart, date_heure_fin, x_gps, y_gps, duree, vitesse);

    QSqlQuery query;

    bool test = T.modifier();
    if (test)
    {
        ui->tab_trajectoire->setModel(T.afficher());
        ui->comboBox_modif->setModel(T.afficher());
        ui->comboBox_supp->setModel(T.afficher());
        QMessageBox::information(nullptr, QObject::tr("Modifier Trajectoire"),
            QObject::tr("Trajectoire modifiée avec succès."), QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Modifier Trajectoire"),
            QObject::tr("Erreur lors de la modification de la trajectoire.\n"
                        "Click Cancel to exit."), QMessageBox::Cancel);
    }
}

void MainWindow::on_pb_supprimer_clicked()
{
    int id_trajectoire=ui->comboBox_supp->currentText().toInt();

    bool test=T.supprimer(id_trajectoire);
    if(test)
    {
        ui->tab_trajectoire->setModel(T.afficher());
        ui->comboBox_modif->setModel(T.afficher());
        ui->comboBox_supp->setModel(T.afficher());

        QMessageBox::information(nullptr, QObject::tr("Supprimer Trajectoire"),
                    QObject::tr("Trajectoire supprimé.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);


    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Supprimer service"),
                    QObject::tr("Erreur !.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}

void MainWindow::on_valider_clicked()
{
    {
        QString id_trajectoire=ui->comboBox_modif->currentText();
                  QSqlQuery query;
                  query.prepare("select * from Trajectoire where id_trajectoire='"+id_trajectoire+"'");
                  if (query.exec())
                  {
                   while (query.next())
                   {


                       ui->le_date_heure_depart2->setDate(query.value(1).toDate());
                       ui->le_date_heure_fin2->setDate(query.value(2).toDate());
                       ui->le_x_gps2->setText(query.value(3).toString());
                       ui->le_y_gps2->setText(query.value(4).toString());

                       ui->le_vitesse2->setText(query.value(5).toString());
                       ui->le_duree2->setText(query.value(6).toString());






}
                  }
              }

}



void MainWindow::on_reset_clicked()
{
    ui->le_x_gps2->clear();
    ui->le_y_gps2->clear();

    ui->le_duree2->clear();
    ui->le_vitesse2->clear();


}

void MainWindow::on_PDF_clicked()
{
    QPdfWriter pdf("C:/Users/admin/Desktop/Gestion_trajectoire/Gestion_trajectoire/list_des_trajectoire.pdf");

          QPainter painter(&pdf);

          int i = 4000;
          painter.setPen(Qt::black);
          QFont titleFont("Book Script", 20);
          titleFont.setWeight(QFont::Bold);  // Ajouter cette ligne pour mettre le texte en gras
          painter.setFont(titleFont);
          painter.drawText(2500, 1400, "LISTE DES TRAJECTOIRE");

                 painter.setPen(Qt::Key_Green);
                 painter.setFont(QFont("Time New Roman", 10));
                 painter.drawRect(100, 100, 9200, 2700); // dimension ta3 rectangle
                 painter.drawRect(100, 3000, 9200, 500);


                 painter.drawText(300,3300,"ID Trajectoire");
                 painter.drawText(1500,3300,"Date Depart");
                 painter.drawText(3500,3300,"Date Fin");
                 painter.drawText(5500,3300,"X GPS");
                 painter.drawText(6500,3300,"Y GPS");
                 painter.drawText(7500,3300,"Duree");
                 painter.drawText(8500,3300,"Vitesse");


                 QImage image("C:/Users/admin/Desktop/Gestion_trajectoire/Gestion_trajectoire/logoesprit.png");
                 painter.drawImage(QRectF(200, 200, 2000, 2000), image);

                 QImage image1("C:/Users/admin/Desktop/Gestion_trajectoire/Gestion_trajectoire/Eco-Friendly.jpg");
                 painter.drawImage(QRectF(7000, 200, 2000, 2000), image1);

                 painter.drawRect(100, 3700, 9200, 9000);

                 QSqlQuery query;
                 query.prepare("select * from Trajectoire");
                 query.exec();
                 while (query.next())
                 {
                     painter.drawText(300, i, query.value(0).toString());
                     painter.drawText(1500, i, query.value(1).toString());
                     painter.drawText(3500, i, query.value(2).toString());
                     painter.drawText(5500, i, query.value(3).toString());
                     painter.drawText(6500, i, query.value(4).toString());
                     painter.drawText(7500, i, query.value(5).toString());
                     painter.drawText(8500, i, query.value(6).toString());

                     i = i + 350;
                 }

                 QMessageBox::information(this, QObject::tr("PDF Enregistré!"),
                     QObject::tr("PDF Enregistré!.\n" "Click Cancel to exit."), QMessageBox::Cancel);
}

void MainWindow::on_lineEdit_recherche_textChanged(const QString &arg1) //recherche dynamique
{
        QString NOM=ui->lineEdit_recherche->text();
       ui->tab_trajectoire->setModel(T.rechercher(NOM));
      ui->tab_trajectoire->clearSelection();
}
void MainWindow::on_comboBox_tri_activated(const QString &arg1)
{


    QString NOM=ui->comboBox_tri->currentText();
   ui->tab_trajectoire->setModel(T.trier(NOM));
  ui->tab_trajectoire->clearSelection();
}

void MainWindow::on_pushButton_arduino_clicked()
{


    QString data=ui->lineEdit_arduino->text();

    if(data!="0")
               {

               if(A.cherchernum_facture(data)==1)
               {
                 A.write_to_arduino("1");
                      QMessageBox:: information(nullptr, QObject::tr("OK"),
                                                QObject::tr(" Bienvenue\n"
                                                            "click cancel to exit."),QMessageBox::Cancel);
                }

                   else
                       {
                       QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                             QObject::tr("Numero nexiste pas !!!.\n"
                                                     "click Cancel to exit."),QMessageBox::Cancel);
                        A.write_to_arduino("0");
                        }



               }

}
