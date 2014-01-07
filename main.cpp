#include <QtGui/QApplication>
#include "usb2can.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    usb2can foo;
    foo.show();
    return app.exec();
}
