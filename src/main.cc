#include <Dynaview.h>

#include <QApplication>
#include <iostream>

int main( int argc, char** argv )
{

  if ( argc < 2 )
  {
    std::cerr << "Usage: dynaview <endo.vtk>\n";
    return 1;
  }

  std::cerr << "Dynaview initializing...\n";

  QApplication app( argc, argv );

  Dynaview dynaview;
  dynaview.show();

  dynaview.initialize_vtk();

  dynaview.add_vtk_file( argv[1] );

  return app.exec();
}
