#include <cstring>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <vector>

#include "concavehull.hpp"
#include "galaxy.hpp"

namespace py = pybind11;

py::array_t<double> pyconcavehull(const py::array_t<double>& pts, double chi_factor) {
	py::buffer_info buf = pts.request();
	
	if (buf.ndim != 2) {
		throw std::runtime_error("Number of dimensions must be two");
	}
	
	if (buf.shape[1] != 2) {
		throw std::runtime_error("Second dimension's size must be two");
	}

	double *ptr = static_cast<double *>(buf.ptr);

	std::vector<double> ch = concavehull({ptr, ptr+buf.size}, chi_factor);

	py::buffer_info resbuf = py::buffer_info(
		ch.data(),
		buf.itemsize,
		buf.format,
		buf.ndim,
		{(size_t)(ch.size() >> 1), (size_t)2},
		buf.strides
		);
	return py::array_t<double>(resbuf);
}

py::array_t<double> pygalaxy(const py::array_t<double>& pts, const py::array_t<double>& center, double chi_factor) {
	py::buffer_info buf = pts.request();
	
	if (buf.ndim != 2) {
		throw std::runtime_error("Number of dimensions must be two");
	}
	
	if (buf.shape[1] != 2) {
		throw std::runtime_error("Second dimension's size must be two");
	}

	py::buffer_info buf_center = center.request();

	double *ptr = static_cast<double *>(buf.ptr);
	double *ptr_center = static_cast<double *>(buf_center.ptr);

	std::vector<double> ch = galaxy({ptr, ptr+buf.size}, {ptr_center, ptr_center+buf_center.size}, chi_factor);

	py::buffer_info resbuf = py::buffer_info(
		ch.data(),
		buf.itemsize,
		buf.format,
		buf.ndim,
		{(size_t)(ch.size() >> 1), (size_t)2},
		buf.strides
		);
	return py::array_t<double>(resbuf);
}

PYBIND11_MODULE(concavehull, m) {
	m.def("concavehull", &pyconcavehull, "Find concave hull from array of 2D points",
	      py::arg("pts"), py::arg("chi_factor") = 0.1);
	m.def("galaxy", &pygalaxy, "Find galaxy from array of 2D points",
	      py::arg("pts"), py::arg("center"), py::arg("chi_factor") = 0.1);
}
