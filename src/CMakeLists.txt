PROJECT( dynaview )

cmake_minimum_required(VERSION 2.8)  


#Match everything when analyzing dependencies
INCLUDE_REGULAR_EXPRESSION("^.*$")
OPTION (BUILD_SHARED_LIBS "Build shared libraries" OFF)
OPTION (BUILD_TESTS "Build dynaview test applications." ON)
OPTION (INSTALL_TEST_FILES "Include dynaview test images and help files in the install." OFF)
OPTION (INSTALL_SOURCE  "Install dynaview source code." OFF)
OPTION (USE_PRECOMPILED_HEADERS "Use precompiled headers to speed up compilation" ON)

OPTION (LOAD_DEFAULT_PROJECT "Load default project" OFF)


MESSAGE(STATUS "** PRECOMPILED_HEADERS: ${USE_PRECOMPILED_HEADERS}")

#set(CMAKE_BUILD_TYPE Release)

IF(LOAD_DEFAULT_PROJECT)
  ADD_DEFINITIONS(-DLOAD_DEFAULT_PROJECT)
ENDIF(LOAD_DEFAULT_PROJECT)

IF (WIN32 AND MSVC)
  ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
  SET(QT_WINDOWS_STUFF Qt5::WinMain)
ENDIF (WIN32 AND MSVC)

FIND_PACKAGE(ITK REQUIRED)
IF (ITK_FOUND)
  INCLUDE (${ITK_USE_FILE})
ENDIF(ITK_FOUND)
MESSAGE(STATUS "** ITK_DIR: ${ITK_DIR}")
MESSAGE(STATUS "** ITK_LIBRARIES: ${ITK_LIBRARIES}")


FIND_PACKAGE(VTK COMPONENTS
  vtkCommonCore
  vtkInfovisCore
#  vtkInteractionStyle
#  vtkRenderingFreeTypeOpenGL
  vtkViewsQt
  vtkFiltersCore
  vtkGUISupportQt
  vtkIOLegacy
  vtkIOGeometry
  vtkFiltersCore
  vtkImagingStencil
  vtkRenderingQt
  )

 MESSAGE(STATUS "** USE_VTK_FILE: ${VTK_USE_FILE}")
 INCLUDE(${VTK_USE_FILE})

#INCLUDE( ${USE_VTK_FILE} )
#MESSAGE(STATUS "** VTK_DIR: ${VTK_DIR}")
#MESSAGE(STATUS "** VTK_LIBRARIES: ${VTK_LIBRARIES}")


#find_package(OpenCV REQUIRED )



SET(CMAKE_INCLUDE_CURRENT_DIR ON)



IF (CMAKE_SIZEOF_VOID_P MATCHES 8)
  SET(DYNAVIEW_BITS "64bit")
  SET(QT_LIB_DEP_DIR "C:/Qt/Qt5.6.0-vs2015-x64/5.6/msvc2015_64/bin")
ELSE (CMAKE_SIZEOF_VOID_P MATCHES 8)
  SET(DYNAVIEW_BITS "32bit")
  SET(QT_LIB_DEP_DIR "C:/Qt/Qt5.6.0-vs2015-x86/5.6/msvc2015/bin")
ENDIF (CMAKE_SIZEOF_VOID_P MATCHES 8)

#ADD_DEFINITIONS( -Wall )

# by default only QtCore and QtGui modules are enabled
# other modules must be enabled like this:
SET( QT_USE_QT3SUPPORT TRUE )   
SET( QT_USE_QTXML TRUE )
SET( QT_USE_QTSVG TRUE )
SET( QT_USE_QTNETWORK TRUE )
SET( QT_USE_QTSCRIPT TRUE )

#--------------------------------------------------------------------------------
# This sets the windows build that will need the special winmain@16 call. Qt provides
#  this for us in the qtmain.lib file. Using this cmake code will ensure we have it
#  linked into our build. Not needed on Unix/OS X/Linux which is why we have the 
#  IF(WIN32) conditional.
IF (WIN32)
    SET (QT_USE_QTMAIN TRUE)
ENDIF (WIN32)

# this command finds Qt4 libraries and sets all required variables
# note that it's Qt4, not QT4 or qt4
SET(QT_MIN_VERSION "4.7.4")
#FIND_PACKAGE( Qt4 REQUIRED )

find_package(Qt5Widgets REQUIRED)
#find_package(Qt5OpenGL REQUIRED)
 
include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITION_DIRS})


# add some useful macros and variables
# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
#INCLUDE( ${QT_USE_FILE} )

MESSAGE(STATUS "** QT_QMAKE_EXECUTABLE: ${QT_QMAKE_EXECUTABLE}")



### Application
SET(DYNAVIEW_APPLICATION_HDRS
  Dynaview.h
  )

SET(DYNAVIEW_APPLICATION_SRCS
  Dynaview.cc
)


SET(DYNAVIEW_APPLICATION_UIS
  Dynaview.ui
)

### Main Stuff
SET( DYNAVIEW_SRCS
     main.cc
     ${DYNAVIEW_APPLICATION_SRCS}
)

SET( DYNAVIEW_HDRS
     ${DYNAVIEW_APPLICATION_HDRS}
)

#SET( DYNAVIEW_RCS
#     Resources/dynaview.qrc
#)

SOURCE_GROUP("Application" FILES ${DYNAVIEW_APPLICATION_SRCS} ${DYNAVIEW_APPLICATION_HDRS} ${DYNAVIEW_APPLICATION_UIS})

# this command will generate rules that will run rcc on all files from DYNAVIEW_RCS
# in result DYNAVIEW_RC_SRCS variable will contain paths to files produced by rcc
QT5_ADD_RESOURCES( DYNAVIEW_RC_SRCS ${DYNAVIEW_RCS} )

# this will run uic on .ui files:
QT5_WRAP_UI( DYNAVIEW_UI_HDRS ${DYNAVIEW_APPLICATION_UIS} 
  ${DYNAVIEW_GROOM_UIS}
  ${DYNAVIEW_OPTIMIZE_UIS} 
  ${DYNAVIEW_ANALYSIS_UIS} 
)

# and finally this will run moc:
QT5_WRAP_CPP( DYNAVIEW_MOC_SRCS ${DYNAVIEW_APPLICATION_HDRS} ${DYNAVIEW_VISUALIZATION_HDRS} ${DYNAVIEW_MOC_HDRS} ${DYNAVIEW_WORKFLOW_HDRS})



# we need this to be able to include headers produced by uic in our code
# (CMAKE_BINARY_DIR holds a path to the build directory)
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )
#INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

# For Apple set the icns file containing icons
IF(APPLE)
  # set how it shows up in the Info.plist file
#  SET(MACOSX_BUNDLE_ICON_FILE dynaview.icns) 
  # set where in the bundle to put the icns file
#  SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/Resources/dynaview.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  # include the icns file in the target
#  SET(DYNAVIEW_SRCS ${DYNAVIEW_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/Resources/dynaview.icns)
ENDIF(APPLE)

#IF(WIN32)
#  SET(DYNAVIEW_SRCS ${DYNAVIEW_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/Resources/dynaview.rc)
#ENDIF(WIN32)

if(MSVC)
  SOURCE_GROUP(moc_sources FILES ${DYNAVIEW_MOC_SRCS})
endif(MSVC)


# here we instruct CMake to build DYNAVIEW executable from all of the source files
ADD_EXECUTABLE( dynaview MACOSX_BUNDLE WIN32 
  ${DYNAVIEW_SRCS}
#  ${DYNAVIEW_HDRS}
  ${DYNAVIEW_MOC_SRCS} 
  ${DYNAVIEW_RC_SRCS} 
  ${DYNAVIEW_UI_HDRS} 
  ${DYNAVIEW_MOC_HDRS}
#  Resources/dynaview.icns
  )

TARGET_LINK_LIBRARIES( dynaview
  ${QT_LIBRARIES} 
  ${ITK_LIBRARIES}
  ${VTK_LIBRARIES}
  ${OpenCV_LIBS}
  ${QT_WINDOWS_STUFF}
  Qt5::Widgets
  )

# custom plist
if (APPLE)
    set_target_properties(dynaview PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Info.plist.in)
endif ()

#-- Add an Option to toggle the generation of the API documentation
option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
if(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR 
      "Doxygen is needed to build the documentation. Please install it correctly")
  endif()
  #-- Configure the Template Doxyfile for our specific project
  configure_file(Doxyfile.in 
                 ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)
  #-- Add a custom target to run Doxygen when ever the project is built
  add_custom_target (docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
    SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
  # IF you do NOT want the documentation to be generated EVERY time you build the project
  # then leave out the 'ALL' keyword from the above command.
endif()




###########################################
# Flags for Microsoft Compiler
###########################################

IF (WIN32 AND MSVC)
#  ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501 -DNTDDI_VERSION=0x05010000)
#  ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501)
#  ADD_DEFINITIONS(-DPSAPI_VERSION=1)
  # Disable Visual C++ Secure Warnings
  ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-D_BIND_TO_CURRENT_VCLIBS_VERSION=1)
  ADD_DEFINITIONS(-D_BIND_TO_CURRENT_CRT_VERSION=1)
  SET(CMAKE_CXX_FLAGS "/Oi ${CMAKE_CXX_FLAGS} /MP4 /Zm250")
  MESSAGE(STATUS "** WINDOWS OPTIONS")
ENDIF (WIN32 AND MSVC)



# INSTALLATION AND PACKAGING
include(InstallRequiredSystemLibraries)

set(plugin_dest_dir bin)
set(qtconf_dest_dir bin)
set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/dynaview")
if(APPLE)
  set(plugin_dest_dir dynaview.app/Contents/MacOS)
  set(qtconf_dest_dir dynaview.app/Contents/Resources)
  set(contents_dest_dir dynaview.app/Contents)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/dynaview.app")
endif(APPLE)
if(WIN32)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/dynaview.exe")
endif(WIN32)



#--------------------------------------------------------------------------------
# Install the OGSVIEW application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.
install(TARGETS dynaview
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )

install(FILES example.jpg DESTINATION ".")

IF (DYNAVIEW_BITS MATCHES "64bit")
  install(FILES "C:/Qt/Qt5.6.0-vs2015-x64/5.6/msvc2015_64/plugins/platforms/qwindows.dll" DESTINATION "bin/platforms")
ELSE (DYNAVIEW_BITS MATCHES "64bit")
  install(FILES "C:/Qt/Qt5.6.0-vs2015-x86/5.6/msvc2015/plugins/platforms/qwindows.dll" DESTINATION "bin/platforms")
ENDIF (DYNAVIEW_BITS MATCHES "64bit")


#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE'
#install(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${plugin_dest_dir}/plugins COMPONENT Runtime)

#--------------------------------------------------------------------------------
# Install needed Qt .nib files and others
#if(APPLE)
#  install(DIRECTORY "${QT_QTGUI_LIBRARY_RELEASE}/Versions/Current/Resources" DESTINATION ${contents_dest_dir} COMPONENT Runtime)
#endif(APPLE)

#--------------------------------------------------------------------------------
# install a qt.conf file
# this inserts some cmake code into the install script to write the file
install(CODE "
   file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")
    " COMPONENT Runtime)




#--------------------------------------------------------------------------------
# Use BundleUtilities to get all other dependencies for the application to work.
# It takes a bundle or executable along with possible plugins and inspects it
# for dependencies.  If they are not system dependencies, they are copied.

# directories to look for dependencies


set(DIRS ${QT_LIBRARY_DIR} ${QT_BINARY_DIR} ${CGAL_DIR}/../auxiliary/gmp/lib ${CGAL_DIR}/bin ${QT_PLUGINS_DIR} ${QT_LIB_DEP_DIR})

# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
# Note that the image plugins depend on QtSvg and QtXml, and it got those copied
# over.  

# fixup_bundle will run the appropriate install_name_tool
# commands on each of the dependencies of the main executable and the
# plugin libraries.  
#
# install_name_tool is used to point any non-system
# dynamically-loaded libraries to the appropriate libraries in the
# distribution
#
# fixup_bundle also grabs the appropriate parts of the qt framework
# (by following dependencies) and installs them in the distribution
# 

install(CODE "
     file(GLOB_RECURSE QTPLUGINS
       \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
     include(BundleUtilities)
     fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
     " COMPONENT Runtime)

# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well
set(CPACK_BINARY_DRAGNDROP ON)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "dynaview")
set(CPACK_PACKAGE_VENDOR "OrthoGrid")
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/PackageDescription.txt")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "39")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_EXECUTABLES "dynaview" "dynaview")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "dynaview ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")


set(CPACK_BUILD_CONFIG "Release")

include(CPack)
