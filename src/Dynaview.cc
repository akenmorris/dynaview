#include <Dynaview.h>

// ui
#include <ui_Dynaview.h>

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>

#include <vtkPolyDataReader.h>
#include <vtkProperty.h>
#include <vtkNrrdReader.h>
#include <vtkImageViewer2.h>
//#include <vtkImagevi
#include <vtkDICOMImageReader.h>

#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkImageSliceMapper.h>

#include <vtkImagePlaneWidget.h>
#include <vtkImageMapper.h>
#include <vtkImageData.h>
#include <vtkActor2D.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkLookupTable.h>
#include <vtkImageSlice.h>
#include <vtkImageResliceMapper.h>
#include <vtkPlaneSource.h>
#include <vtkImageShiftScale.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>

#include <QFile>
#include <QTextStream>

//---------------------------------------------------------------------------
Dynaview::Dynaview()
{
  this->ui_ = new Ui_Dynaview;
  this->ui_->setupUi( this );
}

//---------------------------------------------------------------------------
Dynaview::~Dynaview()
{}

//---------------------------------------------------------------------------
void Dynaview::initialize_vtk()
{
  this->renderer_ = vtkRenderer::New();

  //this->renderer_->SetRenderWindow( this->ui_->qvtk_widget->GetRenderWindow() );

  this->ui_->qvtk_widget->GetRenderWindow()->AddRenderer( this->renderer_ );

//  this->renderer_->SetBackground( .3, .6, .3 ); // Background color green

  this->renderer_->SetAmbient( 1, 1, 1 );




  vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
  //sphereSource->SetCenter( parts[0].toDouble(), parts[1].toDouble(), parts[2].toDouble() );
  sphereSource->SetRadius( 5.0 );


  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection( sphereSource->GetOutputPort() );

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper( mapper );

  //Set the color of the sphere
  //actor->GetProperty()->SetColor( r, g, b ); //(R,G,B)

  this->renderer_->AddActor( actor );



}

//---------------------------------------------------------------------------
void Dynaview::add_vtk_file( std::string filename )
{

  vtkSmartPointer<vtkPolyData> input1;

  vtkSmartPointer<vtkPolyDataReader> reader1 = vtkSmartPointer<vtkPolyDataReader>::New();
  reader1->SetFileName( filename.c_str() );
  reader1->Update();
  input1 = reader1->GetOutput();

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection( reader1->GetOutputPort() );

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper( mapper );

  this->renderer_->AddActor( actor );
}

//---------------------------------------------------------------------------
void Dynaview::add_spheres( std::string filename, double r, double g, double b )
{

  double x, y, z;

  QFile* file = new QFile( QString::fromStdString( filename ) );

  if ( !file->open( QIODevice::ReadOnly ) )
  {
    std::cerr << "Error opening file for reading: " << filename << "\n";
  }

  QTextStream ts( file );

  while ( !ts.atEnd() )
  {
    QString str;
    ts >> str;
    if ( str != "" )
    {
      QStringList parts = str.split( "," );

      // Create a sphere
      vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
      sphereSource->SetCenter( parts[0].toDouble(), parts[1].toDouble(), parts[2].toDouble() );
      sphereSource->SetRadius( 5.0 );

      vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
      mapper->SetInputConnection( sphereSource->GetOutputPort() );

      vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
      actor->SetMapper( mapper );

      //Set the color of the sphere
      actor->GetProperty()->SetColor( r, g, b ); //(R,G,B)

      this->renderer_->AddActor( actor );
    }
  }
  file->close();
}

//---------------------------------------------------------------------------
void Dynaview::add_dicom( std::string filename, std::string matrix_filename )
{

  QFile* file = new QFile( QString::fromStdString( matrix_filename ) );

  if ( !file->open( QIODevice::ReadOnly ) )
  {
    std::cerr << "Error opening file for reading: " << matrix_filename << "\n";
    return;
  }

  QTextStream ts( file );

  vtkMatrix4x4* matrix = vtkMatrix4x4::New();

  matrix->Identity();

  int row = 0;
  while ( !ts.atEnd() )
  {
    QString str;
    ts >> str;
    if ( str != "" )
    {
      QStringList parts = str.split( "," );

      matrix->SetElement( row, 0, parts[0].toDouble() );
      matrix->SetElement( row, 1, parts[1].toDouble() );
      matrix->SetElement( row, 2, parts[2].toDouble() );
      matrix->SetElement( row, 3, parts[3].toDouble() );

      row++;
    }
  }
  file->close();

  vtkMatrix4x4* m2 = vtkMatrix4x4::New();
  m2->SetElement( 0, 0, matrix->GetElement( 0, 0 ) );
  m2->SetElement( 0, 1, -matrix->GetElement( 0, 2 ) );
  m2->SetElement( 0, 2, matrix->GetElement( 0, 1 ) );
  m2->SetElement( 0, 3, matrix->GetElement( 0, 3 ) );
  m2->SetElement( 1, 0, matrix->GetElement( 1, 0 ) );
  m2->SetElement( 1, 1, -matrix->GetElement( 1, 2 ) );
  m2->SetElement( 1, 2, matrix->GetElement( 1, 1 ) );
  m2->SetElement( 1, 3, matrix->GetElement( 1, 3 ) );
  m2->SetElement( 2, 0, matrix->GetElement( 2, 0 ) );
  m2->SetElement( 2, 1, -matrix->GetElement( 2, 2 ) );
  m2->SetElement( 2, 2, matrix->GetElement( 2, 1 ) );
  m2->SetElement( 2, 3, matrix->GetElement( 2, 3 ) );
  m2->SetElement( 3, 0, matrix->GetElement( 3, 0 ) );
  m2->SetElement( 3, 1, -matrix->GetElement( 3, 2 ) );
  m2->SetElement( 3, 2, matrix->GetElement( 3, 1 ) );
  m2->SetElement( 3, 3, matrix->GetElement( 3, 3 ) );

  // Read all the DICOM files in the specified directory.
  vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
  //reader->SetDirectoryName(folder.c_str());
  reader->SetFileName( filename.c_str() );
  reader->Update();

  // Create an image
  vtkSmartPointer<vtkImageCanvasSource2D> imageSource =
    vtkSmartPointer<vtkImageCanvasSource2D>::New();
  imageSource->SetScalarTypeToUnsignedChar();
  imageSource->SetExtent( 0, 20, 0, 20, 0, 0 );
  imageSource->SetNumberOfScalarComponents( 3 );
  imageSource->SetDrawColor( 127, 255, 100 );
  imageSource->FillBox( 0, 20, 0, 20 );
  imageSource->SetDrawColor( 20, 20, 20 );
  imageSource->DrawSegment( 0, 0, 19, 19 );
  imageSource->DrawSegment( 19, 0, 0, 19 );
  imageSource->Update();

  // Pass the original image and the lookup table to a filter to create
  // a color image:
  vtkSmartPointer<vtkImageMapToWindowLevelColors> scalarValuesToColors =
    vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();
  //scalarValuesToColors->SetLookupTable(lookupTable);
  scalarValuesToColors->PassAlphaToOutputOn();
  scalarValuesToColors->SetInputConnection( reader->GetOutputPort() );
  double* range = reader->GetOutput()->GetScalarRange();
  std::cerr << "range = " << range[0] << " - " << range[1] << "\n";
  scalarValuesToColors->SetWindow( range[1] - range[0] );
  scalarValuesToColors->SetLevel( 0.5 * ( range[1] + range[0] ) );
  scalarValuesToColors->SetOutputFormatToLuminance();
  scalarValuesToColors->Update();

  range = scalarValuesToColors->GetOutput()->GetScalarRange();

  vtkSmartPointer<vtkImageShiftScale> xShiftScale =
    vtkSmartPointer<vtkImageShiftScale>::New();
  xShiftScale->SetOutputScalarTypeToUnsignedChar();
  xShiftScale->SetScale( 255 / range[1] );
  xShiftScale->SetInputConnection( scalarValuesToColors->GetOutputPort() );
  xShiftScale->Update();

  std::cerr << "range = " << range[0] << " - " << range[1] << "\n";

  vtkTexture* texture = vtkTexture::New();
  texture->SetInputConnection( xShiftScale->GetOutputPort() );
//texture->SetInputConnection(imageSource->GetOutputPort());

  vtkPlaneSource* plane = vtkPlaneSource::New();
  plane->SetCenter( 0, 0, 0 );
  ///plane->SetCenter( 0.0, 0.0, 0.0 );
  plane->SetNormal( 0.0, 0.0, 1.0 );

  //plane->SetPoint1( 0, 5120, 0 );
  //plane->SetPoint2( 5120, 0, 0 );

  plane->SetPoint1( 0, 100, 0 );
  plane->SetPoint2( 100, 0, 0 );

  //plane->SetCenter(-263,-159,-147);

  vtkPolyDataMapper* polymapper = vtkPolyDataMapper::New();
  polymapper->SetInputConnection( plane->GetOutputPort() );

  vtkActor* plane_actor = vtkActor::New();
  plane_actor->SetMapper( polymapper );
  plane_actor->SetTexture( texture );

  plane_actor->SetUserMatrix( m2 );
  //plane_actor->SetUserMatrix( matrix );

  //matrix->Print( std::cerr );

  plane_actor->GetProperty()->SetSpecular( 1 );
  plane_actor->GetProperty()->SetAmbient( 1 );
  plane_actor->GetProperty()->SetBackfaceCulling( 0 );

  this->renderer_->AddActor( plane_actor );

  return;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
