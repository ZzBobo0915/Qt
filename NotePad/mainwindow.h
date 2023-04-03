#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include <QStatusBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public:
    QTextEdit *textEdit = new QTextEdit();
    QLabel *labelText1;
    QLabel *labelText2;
    QLabel *labelText3;
    QLabel *labelText4;

    QString currentFile;
    QStatusBar *sBar;

private slots:
    void createFile();
    void openFile();
    bool saveFile(QString);
    bool saveAsFile();
    bool slotFile();
    void quitApp();
    void slotDelete();
    void writeDate();
    void slotAbout();
    void showLabel();
    bool canBeSaveOrNot();
    void setFileName(QString);
    void loadFile(QString fileName);

protected:
    void wheelEvent(QWheelEvent *event) override;
};
#endif // MAINWINDOW_H
