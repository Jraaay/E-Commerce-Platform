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
    void loginRegFun();
    void guestLogin();
    void showProduct();
    void changeLang();
    void invalidUsername() const;
    void validUsername() const;
    int curType;
    consumerClass curConsumer;  // 当前消费者
    sellerClass curSeller;      // 当前商家
    QTranslator *translatorPtr; // 翻译文件

private:
    void init();
    void loginRegSwitchFun() const;
    void saveUserList(list<string>);
    void setTypeSeller();
    void setTypeConsumer();
    list<string> getUserList();
    Ui::Widget *ui;      // ui文件
    QMessageBox *msgBox; // 提示框
};
#endif // WIDGET_H
