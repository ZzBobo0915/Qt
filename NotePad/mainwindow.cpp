#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QWheelEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("无标题-记事本");
    this->setWindowIcon(QIcon(":/image/notebook"));

/* 菜单栏 */
    QMenuBar *menuBar = this->menuBar();
    QMenu *menuFile = menuBar->addMenu("文件");
    QMenu *menuEdit = menuBar->addMenu("编辑");
    QMenu *menuHelp = menuBar->addMenu("帮助");

/* 文件菜单 */
    QAction *actionCreate = menuFile->addAction("新建");  // 菜单动作
    actionCreate->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));  // 设置快捷键
    connect(actionCreate, &QAction::triggered, this, &MainWindow::createFile);  // connect

    QAction *actionOpen = menuFile->addAction("打开");
    actionOpen->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
    connect(actionOpen, &QAction::triggered, this, &MainWindow::openFile);

    QAction *actionSave = menuFile->addAction("保存");
    actionSave->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_S));
    connect(actionSave, &QAction::triggered, this, &MainWindow::slotFile);

    QAction *actionSaveAs = menuFile->addAction("另存为...");
    actionSaveAs->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_S));
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::saveAsFile);

    menuFile->addSeparator();

    QAction *actionPrint = menuFile->addAction("打印");
    actionPrint->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));

    menuFile->addSeparator();

    QAction *actionQuit = menuFile->addAction("退出");
    actionQuit->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
    connect(actionQuit, &QAction::triggered, this, &MainWindow::quitApp);

/* 编辑菜单 */
    QAction *actionZre = menuEdit->addAction("撤销");
    actionZre->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Z));
    connect(actionZre, &QAction::triggered, this, [this](){
        this->textEdit->undo();
    });

    QAction *actionCut = menuEdit->addAction("剪切");
    actionCut->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_X));
    connect(actionCut, &QAction::triggered, this, [this](){
        this->textEdit->cut();
    });

    QAction *actionCopy = menuEdit->addAction("复制");
    actionCopy->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));
    connect(actionCopy, &QAction::triggered, this, [this](){
        this->textEdit->copy();
    });

    QAction *actionPaste = menuEdit->addAction("粘贴");
    actionPaste->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_V));
    connect(actionPaste, &QAction::triggered, this, [this](){
        this->textEdit->paste();
    });

    QAction *actionDel = menuEdit->addAction("删除");
    actionDel->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_L));
    connect(actionDel, &QAction::triggered, this, &MainWindow::slotDelete);

    menuEdit->addSeparator();

    QAction *actionAllselect = menuEdit->addAction("全选");
    actionAllselect->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_A));
    connect(actionAllselect, &QAction::triggered, this, [this](){
        this->textEdit->selectAll();
    });

    QAction *actionDate = menuEdit->addAction("时间/日期");
    actionDate->setShortcut(QKeySequence(Qt::Key_F5));
    connect(actionDate, &QAction::triggered, this, &MainWindow::writeDate);

/* 关于菜单 */
    QAction *actionAbout = menuHelp->addAction("关于");
    actionAbout->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_H));
    connect(actionAbout, &QAction::triggered, this, &MainWindow::slotAbout);

    QAction *actionAboutqt = menuHelp->addAction("关于Qt");
    connect(actionAboutqt, &QAction::triggered, this, [this]() {
       QMessageBox::aboutQt(this, "关于Qt");
    });


/* 文本编辑框 */
    textEdit->setFont(QFont("微软雅黑",12));
    setCentralWidget(textEdit);

/* 状态栏 */
    sBar = this->statusBar();
    labelText1 = new QLabel();
    labelText1->setText("第1行,第1列");
    labelText2 = new QLabel();
    labelText2->setText("100%");
    labelText3 = new QLabel();
    labelText3->setText("Windows(CRLF)  ");
    labelText4 = new QLabel();
    labelText4->setText("UTF-8  ");
    sBar->addWidget(labelText1);           // 设置在左边，一般为实时消息
    sBar->addPermanentWidget(labelText2);  // 设置在右边，一般为永久消息
    sBar->addPermanentWidget(labelText3);
    sBar->addPermanentWidget(labelText4);
    connect(textEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::showLabel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/* 判断文件是否被修改 */
bool MainWindow::canBeSaveOrNot()
{
    // 判断文件是否被更改
    if (textEdit->document()->isModified()) {
        QMessageBox::StandardButtons result = QMessageBox::warning(this, "Warning", "你想要保存么?",
                                                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (QMessageBox::Cancel == result) return false;
        if (QMessageBox::Save == result) return slotFile();
        if (QMessageBox::Discard == result) return true;
    }
    return true;
}

/* 创建新文件 */
void MainWindow::createFile()
{
    if (canBeSaveOrNot()){
        textEdit->clear();
        setFileName("");
    }
}

/* 设置文件名同时设置MainWindow标题 */
void MainWindow::setFileName(QString fileName)
{
    currentFile = fileName;
    textEdit->document()->setModified(false);
    this->setWindowModified(false);
    if (fileName.isEmpty()){
        this->setWindowFilePath("未命名.txt");
        this->setWindowTitle("未命名.txt");
    }else {
        QFileInfo info(fileName);
        this->setWindowFilePath(fileName);
        this->setWindowTitle(info.completeBaseName());
    }
    //qDebug() << currentFile;
}

/* 打开已有的文件 */
void MainWindow::openFile()
{
    if (canBeSaveOrNot()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty()) loadFile(fileName);
    }
}

/* 加载文件 */
void MainWindow::loadFile(QString fileName)
{
    QFile *file = new QFile(fileName);
    if (!file->open(QFile::ReadOnly|QFile::Text)) {
        QMessageBox::critical(this, "critical", "can not read file");
    }else {
        QTextStream in(file);
        textEdit->setText(in.readAll());
        setFileName(fileName);
    }
}

/* 文件另存为 */
bool MainWindow::saveAsFile()
{
    QString path = QFileDialog::getSaveFileName(this, "Save File", ".", "Text File(*.txt)");
    if (path.isEmpty()) {
        return false;
    }else return saveFile(path);

}

/* 保存文件 */
bool MainWindow::saveFile(QString fileName)
{
    QFile file(fileName);
    file.setFileName(fileName);
    if (!file.open(QFile::WriteOnly|QFile::Text)) {
        QMessageBox::critical(this, "critical", "cannot write file");
        return false;
    }
    QTextStream out(&file);
    out << textEdit->toPlainText();
    this->setFileName(fileName);
    return true;
}

/* 保存文件槽函数 */
bool MainWindow::slotFile()
{
    if (currentFile.isEmpty()) return saveAsFile();
    else return saveFile(currentFile);
}

/* 退出 */
void MainWindow::quitApp()
{
    if (canBeSaveOrNot()) {
        qApp->closeAllWindows();  // 关闭所有窗口
    }
}

/* 删除 */
void MainWindow::slotDelete()
{
    QTextCursor cursor = textEdit->textCursor();
    // 如果有选中则取消，以免受影响
    if (cursor.hasSelection()) cursor.clearSelection();
    // 删除前一个字符
    cursor.deletePreviousChar();
    textEdit->setTextCursor(cursor);
}

/* 写入时间 */
void MainWindow::writeDate()
{
    //QDateTime current_date = QDateTime::currentDateTime();
    //QString currentDate = current_date.toString("hh:mm yyyy/MM/dd");
    textEdit->append(QDateTime::currentDateTime().toString("hh:mm yyyy/MM/dd"));
}

/* 关于 */
void MainWindow::slotAbout()
{
    QMessageBox::about(this, "About NotePad", "Producted By ZzBoAYU.\n"
                                              "Used Qt Creator4.11.1\n"
                                              "On Win11");
}

/* 动态显示行列 */
void MainWindow::showLabel()
{
    QTextCursor textCursor = textEdit->textCursor();  // 获取鼠标光标
    int rowNum = textCursor.blockNumber();
    int colNum = textCursor.columnNumber();
    labelText1->setText(tr("第%1行,第%2列").arg(rowNum+1).arg(colNum+1));
}

/* 重写滑轮事件处理函数wheelEvent */
void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        // angleDelta()返回滚轮度数的增量
        int delta = event->angleDelta().y();
        if (delta > 0) {
            textEdit->zoomIn();
        }else {
            textEdit->zoomOut();
        }
    }
}






