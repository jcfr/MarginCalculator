/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __qSlicerMotionSimulatorDoubleArrayModule_h
#define __qSlicerMotionSimulatorDoubleArrayModule_h

// SlicerQt includes
#include "qSlicerLoadableModule.h"

#include "qSlicerMotionSimulatorDoubleArrayModuleExport.h"

class qSlicerMotionSimulatorDoubleArrayModulePrivate;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_MOTIONSIMULATORDOUBLEARRAY_EXPORT qSlicerMotionSimulatorDoubleArrayModule :
  public qSlicerLoadableModule
{
  Q_OBJECT
  Q_INTERFACES(qSlicerLoadableModule);

public:

  typedef qSlicerLoadableModule Superclass;
  explicit qSlicerMotionSimulatorDoubleArrayModule(QObject *parent=0);
  virtual ~qSlicerMotionSimulatorDoubleArrayModule();

  qSlicerGetTitleMacro(QTMODULE_TITLE);

  virtual QString helpText()const;
  virtual QString acknowledgementText()const;
  virtual QStringList contributors()const;

  virtual QStringList categories()const;
  virtual QStringList dependencies() const;
  virtual bool isHidden()const;

protected:

  /// Initialize the module. Register the volumes reader/writer
  virtual void setup();

  /// Create and return the widget representation associated to this module
  virtual qSlicerAbstractModuleRepresentation * createWidgetRepresentation();

  /// Create and return the logic associated to this module
  virtual vtkMRMLAbstractLogic* createLogic();

protected:
  QScopedPointer<qSlicerMotionSimulatorDoubleArrayModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerMotionSimulatorDoubleArrayModule);
  Q_DISABLE_COPY(qSlicerMotionSimulatorDoubleArrayModule);

};

#endif
