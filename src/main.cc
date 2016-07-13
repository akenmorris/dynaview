#include <Dynaview.h>

#include <QApplication>
#include <iostream>

int main( int argc, char** argv )
{

  if ( argc < 11 )
  {
    std::cerr << "Usage: dynaview <endo.vtk> <S.csv> lmx.csv zmr.csv fluoro1.dcm fluoro2.dcm MX1.csv MX2.csv lm_mr.csv MPro.csv\n";
    return 1;
  }

  std::cerr << "Dynaview initializing...\n";

  QApplication app( argc, argv );

  Dynaview dynaview;
  dynaview.show();

  dynaview.initialize_vtk();

  dynaview.add_vtk_file( argv[1] );

  dynaview.add_spheres( argv[2], 1, 0, 0 );

  dynaview.add_spheres( argv[3], 0, 1, 0 );

  dynaview.add_spheres( argv[4], 0, 0, 1 );


  dynaview.add_dicom( argv[5], argv[7] );
  dynaview.add_dicom( argv[6], argv[8] );

  // original CS landmarks
  dynaview.add_spheres( argv[9], 1, 0, 1 );


  dynaview.add_vtk_file( argv[1], argv[10] );

  return app.exec();
}
