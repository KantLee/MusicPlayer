#include "kmusicplayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KMusicPlayer w;
    w.show();
    return a.exec();
}
