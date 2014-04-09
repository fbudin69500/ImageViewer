#include "ImageViewerConfigure.h"
#ifdef USE_CLI
#include "ImageViewerCLP.h"
#endif
//Qt includes
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QPlastiqueStyle>

//QtImageViewer includes
#include "QtGlSliceView.h"
#include "QtSlicer.h"

// itk includes
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkMetaImageIOFactory.h"

int main( int argc, char* argv[] ) 
{
#ifdef USE_CLI
  PARSE_ARGS;
#endif


  QApplication myApp( argc, argv );

  QtSlicer qtSlicerWindow( 0, 0, TRUE );
  //myApp.setMainWidget(&m_GUI);

  qtSlicerWindow.setWindowTitle("Insight Qt Slicer" );
  myApp.setStyle(new QPlastiqueStyle );
  QPalette p( QColor( 239, 239, 239 ) );
  myApp.setPalette( p );

  typedef double                            PixelType;
  typedef itk::Image<PixelType, 3>          ImageType;
  typedef itk::ImageFileReader<ImageType>   ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  QString filePathToLoad;

#ifdef USE_CLI
  filePathToLoad = QString::fromStdString(inputImage);
#endif

  if(filePathToLoad.isEmpty())
    {
    filePathToLoad = QFileDialog::getOpenFileName(&qtSlicerWindow,"", QDir::currentPath());
    }

  reader->SetFileName( filePathToLoad.toLatin1().data() );

  qDebug() << "loading image " << filePathToLoad << " ... ";
  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << "Exception in file reader " << std::endl;
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Done!" << std::endl;
  qtSlicerWindow.setInputImage( reader->GetOutput() );
#ifdef USE_CLI
  qtSlicerWindow.OpenGlWindow->setOrientation(orientation);
  qtSlicerWindow.OpenGlWindow->setSliceNum(sliceOffset);
  qtSlicerWindow.OpenGlWindow->setMaxIntensity(maxIntensity);
  qtSlicerWindow.OpenGlWindow->setMinIntensity(minIntensity);
  qtSlicerWindow.OpenGlWindow->setZoom(zoom);
  qtSlicerWindow.OpenGlWindow->transpose(transpose);
  qtSlicerWindow.OpenGlWindow->flipZ(zFlipped);
  qtSlicerWindow.OpenGlWindow->flipY(yFlipped);
  qtSlicerWindow.OpenGlWindow->flipX(xFlipped);
  qtSlicerWindow.OpenGlWindow->setOverlayOpacity(overlayOpacity);
  qtSlicerWindow.OpenGlWindow->setViewCrosshairs(crosshairs);
  qtSlicerWindow.OpenGlWindow->setViewDetails(details);
  qtSlicerWindow.OpenGlWindow->setViewValuePhysicalUnits(physicalUnits);
  qtSlicerWindow.OpenGlWindow->setViewValue(value);
  qtSlicerWindow.OpenGlWindow->setViewAxisLabel(axisLabel);
  qtSlicerWindow.OpenGlWindow->setViewClickedPoints(clickedPoints);
  qtSlicerWindow.OpenGlWindow->setImageMode(imageMode.c_str());
  qtSlicerWindow.OpenGlWindow->setIWModeMax(iwModeMax.c_str());
  qtSlicerWindow.OpenGlWindow->setIWModeMin(iwModeMin.c_str());

  qtSlicerWindow.OpenGlWindow->update();
#endif

  qtSlicerWindow.show();
  int execReturn;
  try
    {
    execReturn = myApp.exec();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << "Exception during GUI execution" << std::endl;
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
    }
 
  return execReturn;

}
