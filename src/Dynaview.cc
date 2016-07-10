#include <Dynaview.h>

// ui
#include <ui_Dynaview.h>

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>

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

  this->ui_->qvtk_widget->GetRenderWindow()->AddRenderer(this->renderer_);

  // Create a sphere
  vtkSmartPointer<vtkSphereSource> sphereSource =
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->SetCenter( 0.0, 0.0, 0.0 );
  sphereSource->SetRadius( 5.0 );

  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection( sphereSource->GetOutputPort() );

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper( mapper );

  //  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
//    vtkSmartPointer<vtkRenderWindowInteractor>::New();
//  renderWindowInteractor->SetRenderWindow( renderWindow );

  this->renderer_->AddActor( actor );
  this->renderer_->SetBackground( .3, .6, .3 ); // Background color green
}
