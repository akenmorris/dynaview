#include <QMainWindow>

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

// Forward Qt class declarations
class Ui_Dynaview;

class Dynaview : public QMainWindow

{
  Q_OBJECT

public:

  Dynaview();
  ~Dynaview();

  void initialize_vtk();

  void add_vtk_file(std::string filename);

private:

  vtkRenderer *renderer_;


  /// designer form
  Ui_Dynaview* ui_;

};
