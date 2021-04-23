#ifndef PROMPTBOX_H
#define PROMPTBOX_H

#include <QWidget>
#include <QtWinExtras>

namespace Ui
{
    class promptBox;
}
using namespace std;

class promptBox : public QWidget
{
    Q_OBJECT

public:
    explicit promptBox(QWidget *parent = nullptr, string showText = "", int showTime = 1000);
    void closePromptBox();
    ~promptBox();

private:
    Ui::promptBox *ui;
};

#endif // PROMPTBOX_H
