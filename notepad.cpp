#include "notepad.h"
#include "ui_notepad.h"

NotePad::NotePad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NotePad)
{
    ui->setupUi(this);

    //初始化
    //设置标题
    setWindowTitle("myNotepad.txt[*] - NotePad");
    //字符集
    codec = QTextCodec::codecForName("UTF-8");
    //读取设置
    readSettings();
    //添加状态栏
    setStatusBar(ui->statusbar);
    //初始化状态栏欢迎语
    ui->statusbar->showMessage(codec->toUnicode("欢迎使用NotePad"));
    //添加行列数显示
    label_RowCol = new QLabel(codec->toUnicode("1行 0列"));
    ui->statusbar->addPermanentWidget(label_RowCol);
    //添加字数显示
    label_Count = new QLabel(codec->toUnicode("0字符"));
    ui->statusbar->addPermanentWidget(label_Count);
    //初始化处于日间模式
    modeFlag = true;
    //初始化老板键状态处于否
    bossFlag = false;
    //初始化自动保存为否
    autoSaveFlag = false;
    //初始化不存在文件路径
    isHavePath = false;
    //初始化窗口的颜色样式
    QString strSheet = codec->toUnicode("QTextEdit{color:rgb(%1,%2,%3);\
                                        background-color:rgb(%4,%5,%6);}")
                        .arg(fontColor.red()).arg(fontColor.green()).arg(fontColor.blue())
                        .arg(backColor.red()).arg(backColor.green()).arg(backColor.blue());
    ui->textEdit->setStyleSheet(strSheet);
    ui->statusbar->setStyleSheet("");
    //初始化浮动提示
    ui->action_new->setStatusTip(codec->toUnicode("创建一个文件"));
    ui->action_open->setStatusTip(codec->toUnicode("打开一个已经存在的文件"));
    ui->action_save->setStatusTip(codec->toUnicode("保存文档到硬盘"));
    ui->action_saveAs->setStatusTip(codec->toUnicode("以新的名称保存文档"));
    ui->action_exit->setStatusTip(codec->toUnicode("退出应用程序"));
    ui->action_undo->setStatusTip(codec->toUnicode("撤销先前的操作"));
    ui->action_redo->setStatusTip(codec->toUnicode("恢复先前的操作"));
    ui->action_cut->setStatusTip(codec->toUnicode("剪切选中的内容到剪贴板"));
    ui->action_copy->setStatusTip(codec->toUnicode("复制选中的内容到剪贴板"));
    ui->action_font->setStatusTip(codec->toUnicode("打开字体对话框"));
    ui->action_find->setStatusTip(codec->toUnicode("查找文档中内容"));
    ui->action_selectAll->setStatusTip(codec->toUnicode("将整个文档全部选中"));
    ui->action_clear->setStatusTip(codec->toUnicode("清空文档中所有内容"));
    ui->action_fullScreen->setStatusTip(codec->toUnicode("设置窗口全屏"));
    ui->action_maxiScreen->setStatusTip(codec->toUnicode("设置窗口最大化"));
    ui->action_miniScreen->setStatusTip(codec->toUnicode("设置窗口最小化"));
    ui->action_normal->setStatusTip(codec->toUnicode("设置窗口正常化"));
    ui->action_settingBossKey->setStatusTip(codec->toUnicode("设置老板键"));
    ui->action_bossKey->setStatusTip(codec->toUnicode("使用老板键"));

    //连接信号和槽
    //放大
    connect(ui->action_bigger,&QAction::triggered,this,[=]{
        ui->textEdit->zoomIn();
    });
    //缩小
    connect(ui->action_smaller,&QAction::triggered,this,[=]{
        font = ui->textEdit->font();
        if(font.pointSize() > 12) ui->textEdit->zoomOut();
    });
    //设置自动保存
    connect(ui->action_autoSave,&QAction::triggered,this,[&]{
        if(autoSaveFlag)
        {
            autoSaveFlag = false;
            ui->action_autoSave->setText("自动保存------(否)");
        }else
        {
            autoSaveFlag = true;
            ui->action_autoSave->setText("自动保存------(是)");
        }
    });
    //设置夜间模式
    connect(ui->action_mode,&QAction::triggered,this,[&]{
        if(modeFlag){
            modeFlag = false;
            ui->action_mode->setText("日间模式(&M)");
            this->setPalette(QPalette(QPalette::Base,Qt::black));
            //设置字体的颜色
            fontColor = Qt::white;
            backColor = Qt::black;
            QString fontSheet;
            fontSheet = codec->toUnicode("QTextEdit{color:rgb(%1,%2,%3);background-color:black;}")
                    .arg(fontColor.red()).arg(fontColor.green()).arg(fontColor.blue());
            ui->textEdit->setStyleSheet(fontSheet);
            //            qDebug()<<fontSheet;
            //tnnd直接通过样式表来改样式
            ui->menubar->setStyleSheet("\
                                       QMenuBar {\
                                           background-color:black;\
                                           color:white\
                                       }\
                                       QMenuBar::item::selected{\
                                           background-color:rgb(100,100,100);\
                                       }\
                                       QMenu {\
                                           background-color:black;\
                                           color:white\
                                       }\
                                       QMenu::item::selected{\
                                           background-color:rgb(100,100,100);\
                                       }");
        }else{
            modeFlag = true;
            ui->action_mode->setText("夜间模式(&M)");
            ui->textEdit->setStyleSheet("");
            this->setPalette(QPalette(QPalette::Base,Qt::white));
            ui->menubar->setStyleSheet("");
            fontColor = Qt::black;
            backColor = Qt::white;
        }
    });
    //打开老板键设置对话框
    connect(ui->action_settingBossKey,&QAction::triggered,this,[=]{
        emit this->keySettings();
    });
    //使用老板键
    connect(ui->action_bossKey,&QAction::triggered,this,&NotePad::bossKey);
    //当光标位置改变时
    connect(ui->textEdit,&QTextEdit::cursorPositionChanged,this,&NotePad::showTextRowAndCol);
    //当文档内容改变时
    connect(ui->textEdit->document(),&QTextDocument::contentsChanged,this,&NotePad::documentChanged);
    //字体和颜色
    connect(ui->action_font,&QAction::triggered,this,&NotePad::clicked_font);
    //打开文件
    connect(ui->action_open,&QAction::triggered,this,&NotePad::openFile);
    //保存文件
    connect(ui->action_save,&QAction::triggered,this,&NotePad::save);
    //另存为文件
    connect(ui->action_saveAs,&QAction::triggered,this,&NotePad::saveAs);
    //新建文件
    connect(ui->action_new,&QAction::triggered,this,&NotePad::newFile);
    //文件中查找
    connect(ui->action_find,&QAction::triggered,this,&NotePad::find);
    //书签对话框
    connect(ui->action_bookMarks,&QAction::triggered,this,&NotePad::clicked_bookmarks);
}

NotePad::~NotePad()
{
    delete ui;
}

//事件
void NotePad::wheelEvent(QWheelEvent *event)//鼠标滚轮事件
{
    if(QApplication::keyboardModifiers() == Qt::ControlModifier){
        font = ui->textEdit->font();
        if(event->angleDelta().y()>0) ui->textEdit->zoomIn();
        else if(event->angleDelta().y() < 0 && font.pointSize() > 12)
            ui->textEdit->zoomOut();
    }
}
void NotePad::closeEvent(QCloseEvent *event) //关闭事件
{
    if(maybeSave())
    {
        //保存设置
        if(!modeFlag)
        {
            backColor = Qt::white;
            fontColor = Qt::black;
        }
        writeSet();
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
void NotePad::contextMenuEvent(QContextMenuEvent *e) // 右键菜单事件
{
    e->accept();

    QMenu *menu = new QMenu(ui->textEdit); // 创建菜单，并向其中添加动作

    QAction *undo = menu->addAction(codec->toUnicode("撤销(&Z)"),ui->textEdit,
                                    &QTextEdit::undo,QKeySequence::Undo);
    undo->setEnabled(ui->textEdit->document()->isUndoAvailable());

    QAction *redo = menu->addAction(codec->toUnicode("恢复(&Y)"),ui->textEdit,
                                    &QTextEdit::redo,QKeySequence::Redo);
    redo->setEnabled(ui->textEdit->document()->isRedoAvailable());

    menu->addSeparator();

    QAction *cut = menu->addAction(codec->toUnicode("剪切(&T)"),ui->textEdit,
                                   &QTextEdit::cut,QKeySequence::Cut);
    cut->setEnabled(ui->textEdit->textCursor().hasSelection());

    QAction *copy = menu->addAction(codec->toUnicode("复制(&C)"),ui->textEdit,
                                    &QTextEdit::copy,QKeySequence::Copy);
    copy->setEnabled(ui->textEdit->textCursor().hasSelection());

    menu->addAction(codec->toUnicode("粘贴(&P)"),ui->textEdit,&QTextEdit::paste,QKeySequence::Paste);

    QAction *clear = menu->addAction(codec->toUnicode("清空"),ui->textEdit,&QTextEdit::clear);
    clear->setEnabled(!ui->textEdit->document()->isEmpty());

    menu->addSeparator();

    QAction *select = menu->addAction(codec->toUnicode("全选(&A)"),ui->textEdit,
                                      &QTextEdit::selectAll,QKeySequence::SelectAll);
    select->setEnabled(!ui->textEdit->document()->isEmpty());

    menu->exec(e->globalPos()); // 获取鼠标的位置，然后在这个位置显示菜单

    delete menu; // 最后销毁这个菜单
}
//函数
void NotePad::setCurrentFile(const QString thisFilePath)//设置当前文件
{
    lastFilePath = QFileInfo(thisFilePath).path();
    //有了保存路径
    isHavePath = true;
    //设置textEdit没有被更改过
    ui->textEdit->document()->setModified(false);
    //顺便把标题上的“*”去掉，就是设置没被更改过
    setWindowModified(false);
    //从新设置标题
    this->setWindowTitle(QFileInfo(thisFilePath).fileName()
                         + codec->toUnicode("[*] - NotePad"));
}
void NotePad::writeSettings() //写入设置
{
    font = ui->textEdit->font();
    QSettings set("Tropinone","NotePad");
    set.setValue("open",lastFilePath);
    set.setValue("save",lastSavePath);
    set.setValue("fontSize",font.pointSize());
    set.setValue("fontFamily",font.family());
    set.setValue("fontColorR",fontColor.red());
    set.setValue("fontColorG",fontColor.green());
    set.setValue("fontColorB",fontColor.blue());
    set.setValue("backColorR",backColor.red());
    set.setValue("backColorG",backColor.green());
    set.setValue("backColorB",backColor.blue());
}
void NotePad::readSettings() //读取设置
{
    font = ui->textEdit->font();
    QSettings set("Tropinone","NotePad");
    lastFilePath = set.value("open","").toString();
    lastSavePath = set.value("save","").toString();
    font.setPointSize(set.value("fontSize",12).toInt());
    font.setFamily(set.value("fontFamily",font.family()).toString());
    fontColor.setRed(set.value("fontColorR",0).toInt());
    fontColor.setGreen(set.value("fontColorG",0).toInt());
    fontColor.setBlue(set.value("fontColorB",0).toInt());
    backColor.setRed(set.value("backColorR",255).toInt());
    backColor.setGreen(set.value("backColorG",255).toInt());
    backColor.setBlue(set.value("backColorB",255).toInt());
    ui->textEdit->setFont(font);
}
bool NotePad::save() //保存
{
    if(!isHavePath){
        return saveAs();
    }else{
        return saveFile(filePath);
    }
}
bool NotePad::saveAs()//另存为
{
    // 获取文件路径，如果为空，则返回false
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    codec->toUnicode("另存为"),
                                                    lastSavePath + codec->toUnicode("/myNotepad.txt"),
                                                    "txt(*.txt)");
    if (fileName.isEmpty())
        return false;

    ui->statusbar->showMessage(codec->toUnicode("保存成功"),1000);
    return saveFile(fileName); // 否则保存文件
}
bool NotePad::maybeSave()//判断是否需要保存
{
    if(ui->textEdit->document()->isModified()){
        QMessageBox box;
        box.setWindowTitle(tr("NotePad"));
        box.setText(codec->toUnicode("是否保存对“%1”的更改？")
                    .arg(""));
        box.setIcon(QMessageBox::Warning);

        // 添加按钮，QMessageBox::YesRole可以表明这个按钮的行为
        QPushButton *yesBtn = box.addButton(codec->toUnicode("是(&Y)"),QMessageBox::YesRole);

        box.addButton(codec->toUnicode("否(&N)"),QMessageBox::NoRole);
        QPushButton *cancelBtn = box.addButton(codec->toUnicode("取消"),
                                               QMessageBox::RejectRole);
        box.exec(); // 弹出对话框，让用户选择是否保存修改，或者取消关闭操作
        if (box.clickedButton() == yesBtn)// 如果用户选择是，则返回保存操作的结果
            return save();
        else if (box.clickedButton() == cancelBtn) // 如果选择取消，则返回false
            return false;
    }
    return true;
}
bool NotePad::saveFile(const QString thisFilePath)//保存操作
{
    //    QString theFilePath = thisFilePath + codec->toUnicode(QFileInfo());
    QFile file(thisFilePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("NotePad"),
                             codec->toUnicode("无法写入文件 %1:\n%2.")
                             .arg(thisFilePath,file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->textEdit->toPlainText(); // 以纯文本文件写入
    QApplication::restoreOverrideCursor();

    setCurrentFile(thisFilePath);
    ui->statusbar->showMessage(codec->toUnicode("保存成功"),1000);
    savePath = QFileInfo(thisFilePath).path();
    lastSavePath = savePath;
    return true;
}
//槽函数
void NotePad::clicked_font()//字体对话框的槽函数
{
    fdlg = new fontDlg;
    //    qDebug()<<font.family();
    //    qDebug()<<font.pointSize();
    fdlg->initDlg(font, fontColor, backColor);
    connect(fdlg, SIGNAL(giveFont(QFont)), this, SLOT(getFont(QFont)));
    connect(fdlg, SIGNAL(giveFontColor(QColor)), this, SLOT(getFontColor(QColor)));
    connect(fdlg, SIGNAL(giveBackColor(QColor)), this, SLOT(getBackColor(QColor)));
    fdlg->exec();
}
void NotePad::documentChanged()//当文档发生变动时
{
    setWindowModified(ui->textEdit->document()->isModified());
    //自动保存
    if(autoSaveFlag) save();
    //显示当前字符数
    QString text = ui->textEdit->toPlainText();
    label_Count->setText(QString::number(text.length() - text.count("\n")) + "字符");
}
void NotePad::openFile()//打开文件
{
    if(!maybeSave()) return;
    writeSettings();
    writeSet();
    QString path = QFileDialog::getOpenFileName(this,
                                                codec->toUnicode("打开文件"),
                                                lastFilePath,
                                                codec->toUnicode("txt file(*.txt);;all files(*.*)"));
    if(path.isEmpty()) return;
    filePath = path;
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("NotePad"),
                             codec->toUnicode("无法读取文件 %1:\n%2.")
                             .arg(filePath,file.errorString()));
        return ;
    }

    QTextStream in(&file);

    QApplication::setOverrideCursor(Qt::WaitCursor); // 设置鼠标状态为等待状态
    ui->textEdit->setPlainText(in.readAll());  // 读取文件的全部文本内容，并添加到编辑器中
    QApplication::restoreOverrideCursor(); // 恢复鼠标状态


    ui->statusbar->showMessage(codec->toUnicode("打开文件：%1").arg(QFileInfo(filePath).fileName()),1000);

    //显示当前字符数
    QString text = ui->textEdit->toPlainText();
    label_Count->setText(QString::number(text.length() - text.count("\n")) + "字符");

    setCurrentFile(filePath);
    bookmarks = new QDialog;
    initBookmarks(bookmarks);
}
void NotePad::showTextRowAndCol()// 每当编辑器中的光标位置改变，就重新显示行号和列号
{
    // 因为获取的行号和列号都是从0开始的，所以我们这里进行了加1
    row = ui->textEdit->textCursor().blockNumber() + 1;
    col = ui->textEdit->textCursor().columnNumber();

    //    ui->statusBar->showMessage(codec->toUnicode("%1行 %2列").arg(rowNum).arg(colNum));
    label_RowCol->setText(codec->toUnicode("%1行 %2列").arg(row).arg(col));

    //判断是否选中了内容
    if(!ui->textEdit->textCursor().selectedText().isEmpty())//如果不为空，那么相当于是选中了东西
    {
        //显示当前选中的字符数
        label_Count->setText(QString::number(ui->textEdit->textCursor().selectedText().length())
                             + " 字符");
    }else{
        //显示当前字符数
        label_Count->setText(QString::number(ui->textEdit->toPlainText().length())
                             + " 字符");
    }
}
void NotePad::newFile()//新建文件
{
    QString exePath = QCoreApplication::applicationDirPath();
    exePath.append("/NotePad.exe");
    QString cmd = QString("start ") + exePath;
    system(cmd.toStdString().c_str());
}
void NotePad::find()//文件中查找
{
    //建立查找对话框
    findDlg = new QDialog(this);
    //创建查找输入框
    QLineEdit * lineEdit = new QLineEdit("",findDlg);
    //查找按钮
    QPushButton * findBtn = new QPushButton(findDlg);
    findBtn->setText("查找");

    //这是用来储存查找的字符串
    QString findStr;
    findStr = "";

    //新建布局
    QVBoxLayout * layout = new QVBoxLayout(findDlg);
    //将lineEdit和pushButton放入布局
    layout->addWidget(lineEdit);
    layout->addWidget(findBtn);

    //查找按钮的槽函数
    connect(findBtn,&QPushButton::clicked,findDlg,[=]{
        //赋值，将输入框中的内容赋值给findStr
        QString findStr = lineEdit->text();
        if(findStr.isEmpty()){
            //如果是空的
            QMessageBox::warning(findDlg,"查找","请输入您要查找的内容");
        }else{
            if(!ui->textEdit->find(findStr)){
                //查找（向下查找）找不到
                if(ui->textEdit->find(findStr,QTextDocument::FindBackward)){
                    //如果向上找能找到，说明这是到结尾了
                    //循环查找->需要回到文档开头再继续查找
                    ui->textEdit->moveCursor(QTextCursor::Start);
                    //再从头查找一次
                    ui->textEdit->find(findStr);
                    //上色，将找到的上色
                    QPalette palette = ui->textEdit->palette();
                    palette.setColor(QPalette::Highlight,palette.color(QPalette::Active,
                                                                       QPalette::Highlight));
                    ui->textEdit->setPalette(palette);
                    ui->statusbar->showMessage(codec->toUnicode("循环"),1000);
                }else{
                    //如果向上找也找不到，就是两边都找不到
                    //说明没有这个内容
                    QMessageBox::warning(findDlg,"查找",tr("找不到 %1").arg(findStr));
                }
            }else{
                //找到的话也标记出来
                QPalette palette = ui->textEdit->palette();
                palette.setColor(QPalette::Highlight,palette.color(QPalette::Active,
                                                                   QPalette::Highlight));
                ui->textEdit->setPalette(palette);
                ui->statusbar->showMessage(codec->toUnicode("找到：%1").arg(findStr),500);
            }
        }
    });
    //把对话框显示出来（非模态）
    findDlg->show();
    ui->statusbar->showMessage(codec->toUnicode("查找文件"),1000);
}
void NotePad::bossKey()//设置老板键的快捷键
{
    if(bossFlag)
    {
        bossFlag = false;
        this->show();
    }
    else
    {
        bossFlag = true;
        this->hide();
    }
}
void NotePad::setKey(QKeySequence key)//设置老板键的键
{
    ui->action_bossKey->setShortcut(key);
}
void NotePad::getFont(QFont f)//获取字体
{
    font = f;
    ui->textEdit->setFont(font);
}
void NotePad::getFontColor(QColor c)//获取字体颜色
{
    fontColor = c;
    QString fontSheet = codec->toUnicode("QTextEdit{color:rgb(%1,%2,%3);\
                                         background-color:rgb(%4,%5,%6);}")
.arg(fontColor.red()).arg(fontColor.green()).arg(fontColor.blue())
.arg(backColor.red()).arg(backColor.green()).arg(backColor.blue());
ui->textEdit->setStyleSheet(fontSheet);
}
void NotePad::getBackColor(QColor c)//获取背景颜色
{
    backColor = c;
    QString fontSheet = codec->toUnicode("QTextEdit{color:rgb(%1,%2,%3);\
                                         background-color:rgb(%4,%5,%6);}")
.arg(fontColor.red()).arg(fontColor.green()).arg(fontColor.blue())
.arg(backColor.red()).arg(backColor.green()).arg(backColor.blue());
ui->textEdit->setStyleSheet(fontSheet);
}
void NotePad::clicked_bookmarks()//点击了书签，初始化
{
    bookmarks = new QDialog(this);
    initBookmarks(bookmarks);

    bookmarks->show();
}
void NotePad::initBookmarks(QDialog * d)//初始化书签
{
    d->setMaximumSize(300,300);
    d->setMinimumSize(300,300);
    listWidget = new QListWidget(d);
    listWidget->move(0,0);
    listWidget->resize(200,300);
    newBBtn = new QPushButton("新建书签",d);
    delBBtn = new QPushButton("删除书签",d);
    toBBtn = new QPushButton("转到书签",d);
    BBtn = new QPushButton("保存",d);
    newBBtn->move(200,0);
    delBBtn->move(200,30);
    toBBtn->move(200,60);
    BBtn->move(200,90);
    newBBtn->resize(92,30);
    delBBtn->resize(92,30);
    toBBtn->resize(92,30);
    BBtn->resize(92,30);

    QSettings set("Tropinone","NotePad");

    QList<QString> list;
    list.clear();
    QList<QString> keylist = set.allKeys();
    foreach(QString item , keylist)
    {
        if(item == filePath)
        {
            read(set.value(filePath).toStringList());
            break;
        }
        else read(list);
    }

    if(!rowAndCol.isEmpty())
    {
        for(int i = 0 ; i < rowAndCol.count() ; i++ )
        {
            QListWidgetItem *item;
            item = new QListWidgetItem(rowAndCol[i][0]);
            listWidget->addItem(item);
        }
    }
    else
    {
        delBBtn->setEnabled(false);
        toBBtn->setEnabled(false);
    }

    connect(newBBtn,&QPushButton::clicked,this,&NotePad::newBookmarks);
    connect(delBBtn,&QPushButton::clicked,this,&NotePad::delBookmarks);
    connect(toBBtn,&QPushButton::clicked,this,&NotePad::toBookmarks);
    connect(BBtn,&QPushButton::clicked,this,[=]{
        if(!rowAndCol.isEmpty())
            writeSet();
        else if(rowAndCol.isEmpty())
        {
            QSettings set("Tropinone","NotePad");
            QList<QString> keylist = set.allKeys();
            foreach(QString item , keylist)
            {
                if(item == filePath)
                {
                    set.remove(filePath);
                    break;
                }
                else read(list);
            }
        }
    });

}
void NotePad::read(QList<QString> list)//读取书签
{
    if(list.isEmpty())
    {
        rowAndCol.clear();
        return;
    }
    rowAndCol.clear();
    for(int i = 0 ; i < list.count() ; i++)
    {
        QList<QString> list1;
        list1.append(list[i++]);
        list1.append(list[i]);
        rowAndCol.append(list1);
        list1.clear();
    }
}
void NotePad::toBookmarks()//转到书签
{
    int index = listWidget->currentRow();
    int len = rowAndCol[index][1].toInt();
    QTextCursor cursor;
    cursor = ui->textEdit->textCursor();
    cursor.setPosition(len);
    ui->textEdit->setTextCursor(cursor);
    QDialog * dlg = new QDialog(this);
    dlg->show();
    dlg->close();
}
void NotePad::newBookmarks()//新建书签
{
    QDialog * newBDlg = new QDialog(this);
    newBDlg->setMaximumSize(200,150);
    newBDlg->setMinimumSize(200,150);
    QPushButton * sureBBtn = new QPushButton("确定",newBDlg);
    QLabel * label_1 = new QLabel("书签名:",newBDlg);
    QLabel * label_2 = new QLabel("行数:",newBDlg);
    QLabel * label_3 = new QLabel("列数:",newBDlg);
    QLineEdit * line1 = new QLineEdit(newBDlg);
    QLineEdit * line2 = new QLineEdit(newBDlg);
    QLineEdit * line3 = new QLineEdit(newBDlg);
    label_1->move(10,10);
    label_1->resize(54,16);
    label_2->move(10,40);
    label_2->resize(54,16);
    label_3->move(10,70);
    label_3->resize(54,16);
    sureBBtn->move(110,100);
    sureBBtn->resize(75,24);
    line1->move(80,10);
    line1->resize(113,20);
    line2->move(80,40);
    line2->resize(113,20);
    line3->move(80,70);
    line3->resize(113,20);
    line2->setFocusPolicy(Qt::NoFocus);
    line3->setFocusPolicy(Qt::NoFocus);
    if(!ui->textEdit->textCursor().selectedText().isEmpty())
        line1->setText(ui->textEdit->textCursor().selectedText());
    line2->setText(QString::number(ui->textEdit->textCursor().blockNumber()));
    line3->setText(QString::number(ui->textEdit->textCursor().columnNumber()));
    connect(sureBBtn,&QPushButton::clicked,this,[=]{
        QString name = line1->text();
        QString thisrow = line2->text();
        QString thiscol = line3->text();
        if(name.isEmpty() || thisrow.isEmpty() || thiscol.isEmpty())
        {
            QMessageBox::critical(bookmarks,
                                  codec->toUnicode("错误"),
                                  codec->toUnicode("请完整填写三项内容"));
        }
        else
        {
            QTextCursor cursor, orignal;
            cursor = ui->textEdit->textCursor();
            orignal = cursor;
            cursor.movePosition(QTextCursor::Start,QTextCursor::KeepAnchor);
            ui->textEdit->setTextCursor(cursor);
            QString len = QString::number(ui->textEdit->textCursor().selectedText().length());
            ui->textEdit->setTextCursor(orignal);
            listWidget->clear();
            QList<QString> list;
            list.clear();
            list.append(name);
            list.append(len);
            rowAndCol.append(list);
            listWidget->clear();
//            qDebug()<<len;
            newBDlg->close();
        }
    });
    newBDlg->exec();
    if(!rowAndCol.isEmpty())
    {
        for(int i = 0 ; i < rowAndCol.count() ; i++ )
        {
            QListWidgetItem *item;
            item = new QListWidgetItem(rowAndCol[i][0]);
            listWidget->addItem(item);
        }
        toBBtn->setEnabled(true);
        delBBtn->setEnabled(true);
    }
}
void NotePad::delBookmarks()//删除书签
{
    int index = listWidget->currentRow();
    rowAndCol.removeAt(index);
    listWidget->clear();
    for(int i = 0 ; i < rowAndCol.count() ; i++ )
    {
        QListWidgetItem *item;
        item = new QListWidgetItem(rowAndCol[i][0]);
        listWidget->addItem(item);
    }
    if(rowAndCol.isEmpty())
    {
        delBBtn->setEnabled(false);
        toBBtn->setEnabled(false);
    }
}
QList<QString> NotePad::changeList()
{
    QList<QString> list;
    list.clear();
    for(int i = 0 ; i < rowAndCol.count() ; i++ )
    {
        list.append(rowAndCol[i][0]);
        list.append(rowAndCol[i][1]);
    }
    return list;
}
void NotePad::writeSet()
{
    if(rowAndCol.isEmpty()) return;
    else
    {
        QSettings set("Tropinone","NotePad");
        set.setValue(filePath,changeList());
//        qDebug()<<filePath;
    }
}


