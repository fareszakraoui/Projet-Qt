#ifndef ARDUINO_H
#define ARDUINO_H
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>
class arduino
{
public:
    arduino();
    QString getdata();
    int connect_arduino();
    int close_arduino();
    int write_to_arduino(QByteArray);
    QString read_from_arduino();
    QSerialPort * getserial();
    QString getarduino_port_name();
    int cherchernum_facture(QString num);

private:
    QSerialPort *serial;
    static const quint16 arduino_uno_vendor_id=9025;
    static const quint16 arduino_uno_producy_id=67;
    QString arduino_port_name;
    bool arduino_is_available;
    QString data;
};

#endif // ARDUINO_H
