#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCryptographicHash>
#include <QMessageBox>
#include <fstream>
#include "nlohmann/json.hpp"
#include "user.h"
#include <QTranslator>

using namespace std;
using json = nlohmann::json;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QTranslator *translator, QWidget *parent = nullptr);
    ~Widget();
    int curType;
    consumerClass curConsumer;
    sellerClass curSeller;
    void loginRegFun();
    void guestLogin();
    void showProduct();
    void changeLang();
    QTranslator *translatorPtr;

private:
    Ui::Widget *ui;
    QMessageBox *msgBox;
    void init();
    void loginRegSwitchFun();
    void saveUserList(list<string>);
    list<string> getUserList();
    void setTypeSeller();
    void setTypeConsumer();
};
#endif // WIDGET_H
