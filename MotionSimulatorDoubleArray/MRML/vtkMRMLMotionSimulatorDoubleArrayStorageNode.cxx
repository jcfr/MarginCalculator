/*=auto=========================================================================

Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

See COPYRIGHT.txt
or http://www.slicer.org/copyright/copyright.txt for details.

Program:   3D Slicer
Module:    $RCSfile: vtkMRMLFiducialListStorageNode.cxx,v $
Date:      $Date: 2006/03/17 15:10:10 $
Version:   $Revision: 1.6 $

=========================================================================auto=*/

// MRML includes
#include "vtkMRMLMotionSimulatorDoubleArrayStorageNode.h"
#include "vtkMRMLMotionSimulatorDoubleArrayNode.h"
#include "vtkMRMLScene.h"

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkStringArray.h>

//------------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLMotionSimulatorDoubleArrayStorageNode);

//----------------------------------------------------------------------------
vtkMRMLMotionSimulatorDoubleArrayStorageNode::vtkMRMLMotionSimulatorDoubleArrayStorageNode()
{
}

//----------------------------------------------------------------------------
vtkMRMLMotionSimulatorDoubleArrayStorageNode::~vtkMRMLMotionSimulatorDoubleArrayStorageNode()
{
}

//----------------------------------------------------------------------------
void vtkMRMLMotionSimulatorDoubleArrayStorageNode::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkMRMLStorageNode::PrintSelf(os,indent);
}

//----------------------------------------------------------------------------
bool vtkMRMLMotionSimulatorDoubleArrayStorageNode::CanReadInReferenceNode(vtkMRMLNode *refNode)
{
  return refNode->IsA("vtkMRMLMotionSimulatorDoubleArrayNode");
}

//----------------------------------------------------------------------------
int vtkMRMLMotionSimulatorDoubleArrayStorageNode::ReadDataInternal(vtkMRMLNode *refNode)
{
  std::string fullName = this->GetFullNameFromFileName();

  if (fullName == std::string(""))
    {
    vtkErrorMacro("vtkMRMLMotionSimulatorDoubleArrayStorageNode: File name not specified");
    return 0;
    }

  // cast the input node
  vtkMRMLMotionSimulatorDoubleArrayNode *doubleArrayNode =
    vtkMRMLMotionSimulatorDoubleArrayNode::SafeDownCast(refNode);

  if (doubleArrayNode == NULL)
    {
    vtkErrorMacro("ReadData: unable to cast input node " << refNode->GetID()
                  << " to a double array (measurement) node");
    return 0;
    }

  // open the file for reading input
  fstream fstr;
  fstr.open(fullName.c_str(), fstream::in);

  if (!fstr.is_open() || !fstr.good() || fstr.eof())
    {
    vtkErrorMacro("ERROR opening measurement file \"" << this->FileName << "\"");
    return 0;
    }

  char line[1024];
  // default column ordering for measurement info
  // independent_variable dependent_veariable err
  int xColumn = 0;
  int yColumn = 1;
  int zColumn = 2;
  int valueColumn = 3;
  int value2Column = 4;

  int numColumns = 5;
  std::vector<std::string> labels;
  // save the valid lines in a vector, parse them once know the max id
  std::vector<std::string>lines;
  bool firstLine = true;

  while (fstr.good())
    {
    fstr.getline(line, 1024);


    // does it start with a #?
    if (line[0] == '#')
      {
      vtkDebugMacro("Comment line, checking:\n\"" << line << "\"");
      }

    else
      {
      // is it empty?
      if (line[0] == '\0')
        {
        vtkDebugMacro("Empty line, skipping:\n\"" << line << "\"");
        }
      else
        {
        vtkDebugMacro("got a line: \n\"" << line << "\"");
        char *ptr;
        bool reTokenise = false;
        if (strncmp(line, ",", 1) == 0)
          {
          ptr = NULL;
          reTokenise = true;
          }
        else
          {
          ptr = strtok(line, ",");
          }
        double x = 0.0, y = 0.0, z = 0.0, value = 0.0, value2 = 0.0;
        int columnNumber = 0;

        while (columnNumber < numColumns)
          {
          if (ptr != NULL)
            {
            if (columnNumber == xColumn)
              {
              if (firstLine)
                {
                labels.push_back(ptr);
                }
              else
                {
                x = atof(ptr);
                }
              }
            else if (columnNumber == yColumn)
              {
              if (firstLine)
                {
                labels.push_back(ptr);
                }
              else
                {
                y = atof(ptr);
                }
              }
            else if (columnNumber == zColumn)
              {
              if (firstLine)
                {
                labels.push_back(ptr);
                }
              else
                {
                z = atof(ptr);
                }
              }
            else if (columnNumber == valueColumn)
              {
              if (firstLine)
                {
                labels.push_back(ptr);
                }
              else
                {
                value = atof(ptr);
                }
              }
            else if (columnNumber == value2Column)
              {
              if (firstLine)
                {
                labels.push_back(ptr);
                }
              else
                {
                value2 = atof(ptr);
                }
              }
            }
          if (reTokenise == false)
            {
            ptr = strtok(NULL, ",");
            }
          else
            {
            ptr = strtok(line, ",");
            // turn it off
            reTokenise = false;
            }
          columnNumber++;
          } // end while over columns
        int   fidIndex;
        if (firstLine)
          {
          doubleArrayNode->vtkMRMLMotionSimulatorDoubleArrayNode::SetLabels(labels);
          }
        else
          {
          fidIndex = doubleArrayNode->vtkMRMLMotionSimulatorDoubleArrayNode::AddXYZValue(x, y, z, value, value2);
          if (fidIndex == 0)
            {
            vtkErrorMacro("Error adding a measurement to the list");
            }
          }
        firstLine = false;

        } // point line
      }
    }
  fstr.close();

  // If it's the first line, that means there was no point in the file.
  return firstLine ? 0 : 1;
}

//----------------------------------------------------------------------------
int vtkMRMLMotionSimulatorDoubleArrayStorageNode::WriteDataInternal(vtkMRMLNode *refNode)
{
    if (this->GetFileName() == NULL) 
    {
        vtkErrorMacro("WriteData: file name is not set");
        return 0;
    }

    std::string fullName = this->GetFullNameFromFileName();
    if (fullName == std::string("")) 
    {
        vtkErrorMacro("vtkMRMLMotionSimulatorDoubleArrayStorageNode: File name not specified");
        return 0;
    }

    // cast the input node
    vtkMRMLMotionSimulatorDoubleArrayNode *doubleArrayNode =
      vtkMRMLMotionSimulatorDoubleArrayNode::SafeDownCast(refNode);

    if (doubleArrayNode == NULL)
      {
      vtkErrorMacro("WriteData: unable to cast input node " << refNode->GetID() << " to a known double array node");
      return 0;
      }

    // open the file for writing
    fstream of;

    of.open(fullName.c_str(), fstream::out);

    if (!of.is_open())
    {
        vtkErrorMacro("WriteData: unable to open file " << fullName.c_str() << " for writing");
        return 0;
    }

    // put down a header
    of << "# measurement file " << (this->GetFileName() != NULL ? this->GetFileName() : "null") << endl;

    // if change the ones being included, make sure to update the parsing in ReadData
    of << "# columns = x,y,z,value,value2" << endl;
    std::vector< std::string > labels = doubleArrayNode->GetLabels();
    if (labels.size())
    {
        for(unsigned int l = 0; l<labels.size()-1; l++)
        {
            of << labels.at(l) << ",";
        }
        of << labels.at(labels.size()-1) << endl;
    }



    for (unsigned int i = 0; i < doubleArrayNode->GetSize(); i++)
    {
        double x,y,z,value,value2;
        if (doubleArrayNode->GetXYZValue(i, &x, &y, &z, &value, &value2))
        {

            of << x << "," << y << "," << z << "," << value << "," << value2;
            of << endl;   
        }
        else if (doubleArrayNode->GetXYZValue(i, &x, &y, &z))
        {

            of << x << "," << y << "," << z;
            of << endl;
        }
    }
  of.close();


  return 1;
}

//----------------------------------------------------------------------------
void vtkMRMLMotionSimulatorDoubleArrayStorageNode::InitializeSupportedReadFileTypes()
{
  this->SupportedReadFileTypes->InsertNextValue("CSV (.csv)");
  this->SupportedReadFileTypes->InsertNextValue("Text (.txt)");
}

//----------------------------------------------------------------------------
void vtkMRMLMotionSimulatorDoubleArrayStorageNode::InitializeSupportedWriteFileTypes()
{
  this->SupportedWriteFileTypes->InsertNextValue("CSV (.csv)");
  this->SupportedWriteFileTypes->InsertNextValue("Text (.txt)");
}

//----------------------------------------------------------------------------
const char* vtkMRMLMotionSimulatorDoubleArrayStorageNode::GetDefaultWriteFileExtension()
{
  return "csv";
}
