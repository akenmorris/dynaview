#include <Dynaview.h>

#include <QApplication>
#include <iostream>

int main( int argc, char** argv )
{

  if ( argc < 3 )
  {
    std::cerr << "Usage: dynaview <endo.vtk> <S.csv> \n";
    return 1;
  }

  std::cerr << "Dynaview initializing...\n";

  QApplication app( argc, argv );

  Dynaview dynaview;
  dynaview.show();

  dynaview.initialize_vtk();

  dynaview.add_vtk_file( argv[1] );

  dynaview.add_spheres( argv[2] );

  return app.exec();
}
