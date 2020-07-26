#ifndef VIEWIM_H
#define VIEWIM_H

#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QPainter>


namespace Ui {
class ViewIm;
}

class ViewIm : public QDialog
{
    Q_OBJECT

public:
    explicit ViewIm(QString dir, QWidget *parent = nullptr);
    ~ViewIm();

private:
    Ui::ViewIm *ui;
    QString directory;
};

#endif // VIEWIM_H
