#ifndef PRODUCTDISPLAY_H
#define PRODUCTDISPLAY_H

#include <QWidget>

namespace Ui {
class ProductDisplay;
}

class ProductDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit ProductDisplay(QWidget *parent = nullptr);
    ~ProductDisplay();

private:
    Ui::ProductDisplay *productUi;
};

#endif // PRODUCTDISPLAY_H
