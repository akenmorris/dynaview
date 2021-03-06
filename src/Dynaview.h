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

  int read_files(int argc, char **argv);

  void add_vtk_file(std::string filename, std::string matrix_filename = "");

  void add_spheres(std::string filename, double r, double g, double b);

  void add_spheres(vtkPoints *points, double r, double g, double b);


  void add_line(double *p1, double *p2);

  //void add_nrrd(std::string nrrd);

  void add_dicom(std::string filename, std::string matrix_filename);

  vtkMatrix4x4 *read_matrix(std::string filename);


  void set_sources(std::string filename);


  static vtkPoints *read_points(std::string filename);

public Q_SLOTS:

  void on_lines_checkbox_toggled();
  void on_atrium_checkbox_toggled();
  void on_origin_checkbox_toggled();
  void on_sources_checkbox_toggled();
  void on_parallel_checkbox_toggled();
  void on_landmarks_checkbox_toggled();
  void on_opacity_slider_valueChanged(int value);

private:

  vtkRenderer *renderer_;


  int argc_;
  char **argv_;

  /// designer form
  Ui_Dynaview* ui_;

};
