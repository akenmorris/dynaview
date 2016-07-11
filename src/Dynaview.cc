#include <Dynaview.h>

// ui
#include <ui_Dynaview.h>

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>

#include <vtkPolyDataReader.h>

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
void Dynaview::add_spheres( std::string filename )
{

  double x, y, z;

  QFile* file = new QFile( QString::fromStdString( filename ) );

  if ( !file->open( QIODevice::ReadOnly ) )
  {
    std::cerr << "Error opening file for reading\n";
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

      //  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      //    vtkSmartPointer<vtkRenderWindowInteractor>::New();
      //  renderWindowInteractor->SetRenderWindow( renderWindow );

      this->renderer_->AddActor( actor );
    }
  }
  file->close();
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
