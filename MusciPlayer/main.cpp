#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("Icon/Sys.png"));
    Dialog w;
    w.ShowWindow();

    return a.exec();
}
