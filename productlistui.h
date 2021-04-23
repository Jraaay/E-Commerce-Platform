#ifndef PRODUCTLISTUI_H
#define PRODUCTLISTUI_H

#include <QWidget>

namespace Ui
{
    class productListUi;
}

class productListUi : public QWidget
{
    Q_OBJECT

public:
    explicit productListUi(QWidget *parent = nullptr);
    ~productListUi();
    Ui::productListUi *ui;

private:
};

#endif // PRODUCTLISTUI_H
