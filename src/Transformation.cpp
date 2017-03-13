/*
  Copyright (C) 2009 Jon Macey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "ShaderLib.h"
#include "Transformation.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file Transformation.cpp
/// @brief implementation files for Transformation class
//----------------------------------------------------------------------------------------------------------------------
namespace ngl
{

// Constructor -------------------------------------------------------------------------------------------------------------------
Transformation::Transformation() noexcept
{
  m_position = Vec3(0.0f,0.0f,0.0f);
  m_scale = Vec3(1.0f,1.0f,1.0f);
  m_rotation = Vec3(0.0f,0.0f,0.0f);
  m_isMatrixComputed = false;
  m_matrix=1.0f;
  m_transposeMatrix=1.0f;
  m_inverseMatrix=1.0f;
  computeMatrices();

}

Transformation::Transformation(const Transformation &_t) noexcept
{
  //m_isMatrixComputed=false;

  this->m_position=_t.m_position;
  this->m_scale = _t.m_scale;
  this->m_rotation = _t.m_rotation;
  this->m_isMatrixComputed = true;
  this->m_matrix=_t.m_matrix;
  this->m_transposeMatrix=_t.m_transposeMatrix;
  this->m_inverseMatrix=_t.m_inverseMatrix;
}

Transformation & Transformation::operator =(const Transformation &_t) noexcept
{
  //m_isMatrixComputed=false;

  this->m_position=_t.m_position;
  this->m_scale = _t.m_scale;
  this->m_rotation = _t.m_rotation;
  this->m_isMatrixComputed = true;
  this->m_matrix=_t.m_matrix;
  this->m_transposeMatrix=_t.m_transposeMatrix;
  this->m_inverseMatrix=_t.m_inverseMatrix;
  return *this;
}




void Transformation::setMatrix( const Mat4 &_m   ) noexcept
{
  m_matrix=_m;
  m_transposeMatrix=_m;
  m_transposeMatrix.transpose();
  m_isMatrixComputed = true;
}

// Set scale ---------------------------------------------------------------------------------------------------------------------
void Transformation::setScale( const Vec3& _scale  ) noexcept
{
  m_scale = _scale;
  m_isMatrixComputed = false;
}

void Transformation::setScale( const Vec4& _scale  ) noexcept
{
  m_scale = _scale;
  m_isMatrixComputed = false;
}

void Transformation::setScale(Real _x, Real _y, Real _z  ) noexcept
{
  m_scale.set(_x,_y,_z);
  m_isMatrixComputed = false;
}

// add scale ---------------------------------------------------------------------------------------------------------------------
void Transformation::addScale( const Vec3& _scale ) noexcept
{
  m_scale += _scale;
  m_isMatrixComputed = false;
}


void Transformation::addScale(Real _x,  Real _y,  Real _z ) noexcept
{
  m_scale.m_x+=_x;
  m_scale.m_y+=_y;
  m_scale.m_z+=_z;

  m_isMatrixComputed = false;
}

// Set position --------------------------------------------------------------------------------------------------------------------
void Transformation::setPosition(const Vec4 &_position ) noexcept
{
  m_position = _position;
  m_isMatrixComputed = false;
}
void Transformation::setPosition(const Vec3 &_position) noexcept
{
  m_position = _position;
  m_isMatrixComputed = false;
}
void Transformation::setPosition(Real _x, Real _y, Real _z  ) noexcept
{
  m_position.set(_x,_y,_z);
  m_isMatrixComputed = false;
}

// Set position --------------------------------------------------------------------------------------------------------------------
void Transformation::addPosition( const Vec3& _position) noexcept
{
  m_position+= _position;
  m_isMatrixComputed = false;
}
void Transformation::addPosition( Real _x, Real _y, Real _z ) noexcept
{
  m_position.m_x+=_x;
  m_position.m_y+=_y;
  m_position.m_z+=_z;

  m_isMatrixComputed = false;
}


// set rotation -------------------------------------------------------------------------------------------------------------------
void Transformation::setRotation( const Vec3 &_rotation ) noexcept
{
  m_rotation = _rotation;
  m_isMatrixComputed = false;
}
void Transformation::setRotation( const Vec4 &_rotation ) noexcept
{
  m_rotation = _rotation;
  m_isMatrixComputed = false;
}


void Transformation::setRotation(Real _x, Real _y,  Real _z ) noexcept
{
  m_rotation.set(_x,_y,_z);

  m_isMatrixComputed = false;
}


// set rotation -------------------------------------------------------------------------------------------------------------------
void Transformation::addRotation(const Vec3 &_rotation  ) noexcept
{
  m_rotation+= _rotation;
  m_isMatrixComputed = false;
}
void Transformation::addRotation(Real _x, Real _y, Real _z) noexcept
{
  m_rotation.m_x+=_x;
  m_rotation.m_y+=_y;
  m_rotation.m_z+=_z;
  m_isMatrixComputed = false;
}


// reset matrix ---------------------------------------------------------------------------------------------------------------------
void Transformation::reset() noexcept
{
  m_position = Vec3(0.0f,0.0f,0.0f);
  m_scale = Vec3(1.0f,1.0f,1.0f);
  m_rotation = Vec3(0.0f,0.0f,0.0f);
  m_isMatrixComputed = false;
  computeMatrices();
}

// comptue matrix ---------------------------------------------------------------------------------------------------------------------
void Transformation::computeMatrices() noexcept
{
  if (!m_isMatrixComputed)       // need to recalculate
  {
    Mat4 scale;
    Mat4 rX;
    Mat4 rY;
    Mat4 rZ;
    Mat4 trans;

    // rotation/scale matrix
    Mat4 rotationScale;
    scale.scale(m_scale.m_x, m_scale.m_y, m_scale.m_z);

    rX.rotateX(m_rotation.m_x);
    rY.rotateY(m_rotation.m_y);
    rZ.rotateZ(m_rotation.m_z);
    rotationScale = scale * rX * rY * rZ;

    // transform matrix
    m_matrix = rotationScale;
    m_matrix.m_m[3][0] = m_position.m_x;
    m_matrix.m_m[3][1] = m_position.m_y;
    m_matrix.m_m[3][2] = m_position.m_z;
    m_matrix.m_m[3][3] = 1;



    // tranpose matrix
    m_transposeMatrix = rotationScale;
    m_transposeMatrix.transpose();
    m_transposeMatrix.m_m[0][3] = m_position.m_x;
    m_transposeMatrix.m_m[1][3] = m_position.m_y;
    m_transposeMatrix.m_m[2][3] = m_position.m_z;
    m_transposeMatrix.m_m[3][3] = 1;

    // inverse matrix
    trans.translate(-m_position.m_x, -m_position.m_y, -m_position.m_z);
    scale.scale(1.0f / m_scale.m_x, 1.0f / m_scale.m_y, 1.0f / m_scale.m_z);
    rX.rotateX(-m_rotation.m_x);
    rY.rotateY(-m_rotation.m_y);
    rZ.rotateZ(-m_rotation.m_z);
    m_inverseMatrix = trans * rZ * rY * rX * scale  ;

    m_isMatrixComputed = true;
  }
}

void Transformation::operator*= ( const Transformation &_m) noexcept
{
  m_isMatrixComputed=false;

  computeMatrices();
  m_matrix*=_m.m_matrix;

  /// transpose matrix transformation
  m_transposeMatrix*=_m.m_transposeMatrix;

  /// inverse matrix transformation
  m_inverseMatrix*=_m.m_inverseMatrix;
}

Transformation Transformation::operator*(const Transformation &_m) noexcept
{
  m_isMatrixComputed=false;
  computeMatrices();
  Transformation t;
  t.m_matrix=m_matrix*_m.m_matrix;
  t.m_transposeMatrix=m_transposeMatrix*_m.m_transposeMatrix;
  t.m_inverseMatrix=m_inverseMatrix*_m.m_inverseMatrix;

  return t;
}
void Transformation::loadMatrixToShader(const std::string &_param, const ActiveMatrix &_which) noexcept
{
  computeMatrices();
  ShaderLib *shader=ShaderLib::instance();
  switch (_which)
  {
    case ActiveMatrix::NORMAL :
    {
      shader->setShaderParamFromMat4(_param,m_matrix);
    }
    break;
    case ActiveMatrix::TRANSPOSE :
    {
      shader->setShaderParamFromMat4(_param,m_transposeMatrix);
    }
    break;
    case ActiveMatrix::INVERSE :
    {
      shader->setShaderParamFromMat4(_param,m_inverseMatrix);
    }
    break;

  }


}

void Transformation::loadGlobalAndCurrentMatrixToShader(const std::string &_param, Transformation &_global,  const ActiveMatrix &_which ) noexcept
{
  computeMatrices();
  ShaderLib *shader=ShaderLib::instance();
  switch (_which)
  {
    case ActiveMatrix::NORMAL :
    {
      Mat4 tx=_global.getMatrix()*this->getMatrix();
      shader->setShaderParamFromMat4(_param,tx);
    }
    break;
    case ActiveMatrix::TRANSPOSE :
    {
      Mat4 tx=_global.getTransposeMatrix()*this->getTransposeMatrix();

      shader->setShaderParamFromMat4(_param,tx);
    }
    break;
    case ActiveMatrix::INVERSE :
    {
      Mat4 tx=_global.getInverseMatrix()*this->getInverseMatrix();
      shader->setShaderParamFromMat4(_param,tx);
    }
    break;

  }

}

namespace py = pybind11;

void pyInitTransformation(py::module & m)
{
  py::class_<Transformation>(m, "Transformation")
      .def(py::init<>())
      .def(py::init<const Transformation &>())
      .def("setScale", (void (Transformation::*)(const Vec3 &)) &Transformation::setScale)
      .def("setScale", (void (Transformation::*)(const Vec4 &)) &Transformation::setScale)
      .def("setScale", (void (Transformation::*)(Real,Real,Real)) &Transformation::setScale)
      .def("addScale", (void (Transformation::*)(const Vec3 &)) &Transformation::addScale)
      .def("addScale", (void (Transformation::*)(Real,Real,Real)) &Transformation::addScale)

      .def("setPosition", (void (Transformation::*)(const Vec3 &)) &Transformation::setPosition)
      .def("setPosition", (void (Transformation::*)(const Vec4 &)) &Transformation::setPosition)
      .def("setPosition", (void (Transformation::*)(Real,Real,Real)) &Transformation::setPosition)
      .def("addPosition", (void (Transformation::*)(const Vec3 &)) &Transformation::addPosition)
      .def("addPosition", (void (Transformation::*)(Real,Real,Real)) &Transformation::addPosition)

      .def("setRotation", (void (Transformation::*)(const Vec3 &)) &Transformation::setRotation)
      .def("setRotation", (void (Transformation::*)(const Vec4 &)) &Transformation::setRotation)
      .def("setRotation", (void (Transformation::*)(Real,Real,Real)) &Transformation::setRotation)
      .def("addRotation", (void (Transformation::*)(const Vec3 &)) &Transformation::addRotation)
      .def("addRotation", (void (Transformation::*)(Real,Real,Real)) &Transformation::addRotation)

      .def("setMatrix",&Transformation::setMatrix)
      .def("reset",&Transformation::reset)
      .def("getScale",&Transformation::getScale)
      .def("getPosition",&Transformation::getPosition)
      .def("getRotation",&Transformation::getRotation)
      .def("getMatrix",&Transformation::getMatrix)
      .def("getTransposeMatrix",&Transformation::getTransposeMatrix)
      .def("getInverseMatrix",&Transformation::getInverseMatrix)
//      .def(py::self * py::self)
      .def(py::self *= py::self)
      .def("loadMatrixToShader",&Transformation::loadMatrixToShader)
      .def("loadGlobalAndCurrentMatrixToShader",&Transformation::loadGlobalAndCurrentMatrixToShader)
      ;

}



} // end ngl
