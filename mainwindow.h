#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "trajectoire.h"
#include "arduino.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_ajouter_clicked();

    void on_pb_modifier_clicked();

    void on_pb_supprimer_clicked();

    void on_valider_clicked();

    void on_reset_clicked();

    void on_PDF_clicked();
    void on_comboBox_tri_activated(const QString &arg1);
    void on_lineEdit_recherche_textChanged(const QString &arg1);


    void on_pushButton_arduino_clicked();
signals:
    void setCenter(QVariant, QVariant);
    void addMarker(QVariant, QVariant);
private:
    Ui::MainWindow *ui;
    Trajectoire T;
    arduino A;
};
#endif // MAINWINDOW_H
