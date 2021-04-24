#include "widget.h"

#include <QApplication>
#include <QTranslator>
#include <QtDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    qDebug() << translator.load(":/zh_cn.qm");
    a.installTranslator(&translator);
    Widget w(&translator);
    w.show();
    return a.exec();
}
