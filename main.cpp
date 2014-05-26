#include <QApplication>
#include "DaplugPassGui.h"

int main(int argc, char* argv[]){

    QApplication app(argc,argv);

    DaplugPassGui dpg;
    dpg.show();

    return app.exec();

}
