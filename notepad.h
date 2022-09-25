#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QKeySequenceEdit>
#include <QVBoxLayout>
#include <QSettings>
#include <QWheelEvent>
#include <QCloseEvent>
#include <QContextMenuEvent>
#include <QTextCodec>

#include "fontdlg.h"

QT_BEGIN_NAMESPACE
namespace Ui { class NotePad; }
QT_END_NAMESPACE

class NotePad : public QMainWindow
{
    Q_OBJECT

public:
    NotePad(QWidget *parent = nullptr);
    ~NotePad();

    //打开操作
    void openFile();
    //保存文件
    bool save();
    //另存为
    bool saveAs();
    //是否需要保存
    bool maybeSave();
    //保存操作
    bool saveFile(QString thisFilePath);
    //新建文件
    void newFile();

    //读取设置中的列表（书签）
    void read(QList<QString> list);
    //将二维列表转换成一维列表（二维只有三个）
    QList<QString> changeList();
    //保存书签设定
    void writeSet();
    //初始化书签对话框
    void initBookmarks(QDialog *d);
    //创建书签
    void newBookmarks();
    //删除书签
    void delBookmarks();
    //转到书签
    void toBookmarks();

    //文件中查找对话框
    void find();
    //字体对话框
    void clicked_font();
    //书签对话框
    void clicked_bookmarks();

    //当文档变动时
    void documentChanged();
    //当光标位置改变时
    void showTextRowAndCol();
    //设置文件
    void setCurrentFile(const QString);

    //读取文件设置
    void readSettings();
    //写入文件设置
    void writeSettings();

    //设置老板键的键
    void setKey(QKeySequence key);

public slots:
    //触发老板键
    void bossKey();

    //获取字体
    void getFont(QFont f);
    //获取字体颜色
    void getFontColor(QColor c);
    //获取背景色
    void getBackColor(QColor c);

signals:
    void keySettings();

public:
    //滚轮事件
    void wheelEvent(QWheelEvent *event);
    //关闭事件
    void closeEvent(QCloseEvent *event);
    //鼠标右键事件
    void contextMenuEvent(QContextMenuEvent *e);

private:
    Ui::NotePad *ui;

    //字符编码集
    QTextCodec *codec;

    //用于显示行列数
    QLabel * label_RowCol;
    //用于显示字数
    QLabel * label_Count;

    //行列数储存
    int col, row;

    //用于保存打开文件路径
    QString filePath;
    //用于保存打开文件的根路径
    QString lastFilePath;
    //用于保存保存文件的路径
    QString savePath;
    //用于保存保存文件的根路径
    QString lastSavePath;

    //标记是否是日间模式。日间模式是true,夜间模式是false
    bool modeFlag;
    //标记是否处于老板键状态。处于是true,不处于是false
    bool bossFlag;
    //标记是否自动保存
    bool autoSaveFlag;
    //标记是否存在文件路径
    bool isHavePath;

    //用于保存textEdit的字体
    QFont font;

    //储存字体颜色
    QColor fontColor;
    //储存背景色
    QColor backColor;

    //查找对话框
    QDialog * findDlg;
    //老板键设置对话框
    QDialog * keyDlg;
    //字体和颜色对话框
    fontDlg * fdlg;
    //书签对话框
    QDialog * bookmarks;
    //用于储存书签的行[1]列[2]还有名字[0]
    QList<QList<QString>> rowAndCol;
    //储显示书签
    QListWidget * listWidget;
    //显示书签按钮
    QPushButton * newBBtn;
    QPushButton * delBBtn;
    QPushButton * toBBtn;
    QPushButton * BBtn;

};
#endif // NOTEPAD_H
