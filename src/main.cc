#include <Dynaview.h>

#include <QApplication>
#include <iostream>

int main( int argc, char** argv )
{

  if ( argc < 13 )
  {
    std::cerr << "Usage: dynaview <endo.vtk> <S.csv> lmx1.csv zmr.csv fluoro1.dcm fluoro2.dcm MX1.csv MX2.csv lm_mr.csv MPro.csv lmx2.csv lmx.csv\n";
    return 1;
  }

  std::cerr << "Dynaview initializing...\n";

  QApplication app( argc, argv );

  Dynaview dynaview;
  dynaview.show();

  dynaview.initialize_vtk();

  //dynaview.add_vtk_file( argv[1] );

  // S.csv
  //dynaview.add_spheres( argv[2], 1, 0, 0 );

  vtkPoints *sources = Dynaview::read_points(argv[2]);
  dynaview.add_spheres( sources, 1, 0, 0 );

  // lmx1.csv
  vtkPoints *f1_points = Dynaview::read_points(argv[3]);
  dynaview.add_spheres( f1_points, 0, 1, 0 );

  // zmr.csv
  dynaview.add_spheres( argv[4], 0, 1, 0 );

 // lmx2.csv
  vtkPoints *f2_points = Dynaview::read_points(argv[11]);
  dynaview.add_spheres( f2_points, 0, 1, 0 );

  dynaview.add_line(sources->GetPoint(1),f1_points->GetPoint(0));
  dynaview.add_line(sources->GetPoint(0),f2_points->GetPoint(0));


  // lmx.csv
  //dynaview.add_spheres( argv[12], 0, 0, 1 );

  // flouro1.dcm, MX1.csv
  dynaview.add_dicom( argv[5], argv[7] );

  // flouro2.dcm, MX1.csv
  dynaview.add_dicom( argv[6], argv[8] );

  // original CS landmarks (lm_mr)
  //dynaview.add_spheres( argv[9], 1, 0, 1 );


  dynaview.add_vtk_file( argv[1], argv[10] );

  return app.exec();
}
