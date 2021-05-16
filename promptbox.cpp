#include "promptbox.h"
#include "ui_promptbox.h"
#include <QDesktopWidget>

promptBox::promptBox(QWidget *parent, string showText, int showTime) : QWidget(parent),
                                                                       ui(new Ui::promptBox)
{
    move((QApplication::desktop()->width()) / 2 - 200, (QApplication::desktop()->height()) / 3 * 2);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    QTimer::singleShot(showTime, this, &promptBox::closePromptBox); // 设置自动关闭和析构
    ui->setupUi(this);
    ui->label->setText(showText.c_str());
}

void promptBox::closePromptBox()
{
    close();
    delete this;
}

promptBox::~promptBox()
{
    delete ui;
}
