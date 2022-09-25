#include "fontdlg.h"
#include "ui_fontdlg.h"

fontDlg::fontDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fontDlg)
{
    ui->setupUi(this);

}

fontDlg::~fontDlg()
{
    delete ui;
}


void fontDlg::initDlg(QFont font1, QColor color1, QColor color2)
{
    font = font1;
    fontColor = color1;
    backColor = color2;
    ui->label->setText(font.family() + "; " + QString::number(font.pointSize()));
    ui->label_2->setText(QString::number(fontColor.red()) + ", "
                         +QString::number(fontColor.green()) + ", "
                         +QString::number(fontColor.blue()));
    ui->label_3->setText(QString::number(backColor.red()) + ", "
                         +QString::number(backColor.green()) + ", "
                         +QString::number(backColor.blue()));
}


void fontDlg::on_pushButton_clicked()
{
    bool isOk = false;
    font = QFontDialog::getFont(&isOk, font ,this);
    if(isOk){
        ui->label->setText(font.family() + "; " + QString::number(font.pointSize()));
    }
}


void fontDlg::on_pushButton_2_clicked()
{
    fontColor = QColorDialog::getColor(fontColor,this);
    ui->label_2->setText(QString::number(fontColor.red()) + ", "
                         +QString::number(fontColor.green()) + ", "
                         +QString::number(fontColor.blue()));
}


void fontDlg::on_pushButton_3_clicked()
{
    backColor = QColorDialog::getColor(backColor,this);
    ui->label_3->setText(QString::number(backColor.red()) + ", "
                         +QString::number(backColor.green()) + ", "
                         +QString::number(backColor.blue()));
}


void fontDlg::on_pushButton_4_clicked()
{
    emit this->giveFont(font);
    emit this->giveFontColor(fontColor);
    emit this->giveBackColor(backColor);
    this->close();
}

