#include <QMainWindow>

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>

// Forward Qt class declarations
class Ui_Dynaview;

class Dynaview : public QMainWindow

{
  Q_OBJECT

public:

  Dynaview();
  ~Dynaview();

  void initialize_vtk();

  void add_vtk_file(std::string filename, std::string matrix_filename = "");

  void add_spheres(std::string filename, double r, double g, double b);

  void add_spheres(vtkPoints *points, double r, double g, double b);


  void add_line(double *p1, double *p2);

  //void add_nrrd(std::string nrrd);

  void add_dicom(std::string filename, std::string matrix_filename);

  vtkMatrix4x4 *read_matrix(std::string filename);


  void set_sources(std::string filename);


  static vtkPoints *read_points(std::string filename);
private:

  vtkRenderer *renderer_;


  /// designer form
  Ui_Dynaview* ui_;

};
