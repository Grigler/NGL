#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/cast.h>

#include "MultiBufferVAO.h"
namespace py = pybind11;
namespace ngl
{

  void pyInitMultiBufferVAO(py::module & m)
  {
    py::class_<MultiBufferVAO,AbstractVAO>(m, "MultiBufferVAO")
        .def_static("create",&MultiBufferVAO::create)
        .def("draw", &MultiBufferVAO::draw)
        .def("removeVAO", &MultiBufferVAO::removeVAO)
        .def("setData", &MultiBufferVAO::setData)
        .def("getBufferID", &MultiBufferVAO::getBufferID);
  }
}
