#ifndef FONTDLG_H
#define FONTDLG_H

#include <QDialog>
#include <QFontDialog>
#include <QColorDialog>

namespace Ui {
class fontDlg;
}

class fontDlg : public QDialog
{
    Q_OBJECT

public:
    explicit fontDlg(QWidget *parent = nullptr);
    ~fontDlg();
    void initDlg(QFont font1, QColor color1, QColor color2);

signals:
    void giveFont(QFont font);
    void giveFontColor(QColor color);
    void giveBackColor(QColor color);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::fontDlg *ui;

    QFont font;
    QColor fontColor;
    QColor backColor;

};

#endif // FONTDLG_H
