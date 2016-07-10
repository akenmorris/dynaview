#include <Dynaview.h>

#include <QApplication>
#include <iostream>


int main(int argc, char **argv)
{

  std::cerr << "Dynaview initializing...\n";

  QApplication app( argc, argv );

  Dynaview dynaview;
  dynaview.show();

  dynaview.initialize_vtk();

  return app.exec();


}
