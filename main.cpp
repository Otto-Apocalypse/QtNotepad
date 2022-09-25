#include "notepad.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <MyGlobalShortCut/MyGlobalShortCut.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "NotePad_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    NotePad w;

    MyGlobalShortCut *shortcut = new MyGlobalShortCut("Ctrl+Alt+Z",&w);
    QObject::connect(shortcut,SIGNAL(activated()),&w,SLOT(bossKey()));

    QObject::connect(&w,&NotePad::keySettings,&w,[&]{
        QDialog *keyDlg = new QDialog(&w);
        //界面初始化
        //添加布局
        QVBoxLayout *vlayout = new QVBoxLayout(keyDlg);
        //添加内容
        QPushButton *keyBtn = new QPushButton("确定",keyDlg);
        QKeySequenceEdit *keyEdit = new QKeySequenceEdit(keyDlg);
        keyEdit->setKeySequence(shortcut->getMyKey());
        vlayout->addWidget(keyEdit);
        vlayout->addWidget(keyBtn);

        QObject::connect(keyBtn,&QPushButton::clicked,&w,[&]{
            shortcut = new MyGlobalShortCut(keyEdit->keySequence().toString(),&w);
            QObject::connect(shortcut,SIGNAL(activated()),&w,SLOT(bossKey()));
            w.setKey(keyEdit->keySequence());
            keyDlg->close();
        });

        keyDlg->exec();
    });


    w.show();
    return a.exec();
}
