#ifndef _FAST_STABILISER_PAULI_PYBIND_H
#define _FAST_STABILISER_PAULI_PYBIND_H

#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/stl.h>

#include "pauli.h"

namespace py = pybind11;
using namespace fst;
using namespace pybind11::literals;

// TODO: Export the operator ==
// TODO: Return numpy arrays rather than arrays. Eliminate copying by making types opaque?
namespace fst_pybind {
    void init_pauli(py::module_ &m)
    {
        py::class_<Pauli>(m, "Pauli")
            .def_readwrite("number_qubits", &Pauli::number_qubits, "int")
            .def_readwrite("x_vector", &Pauli::x_vector, "int")
            .def_readwrite("z_vector", &Pauli::z_vector, "int")
            .def_readwrite("sign_bit", &Pauli::sign_bit, "int/bool, either 0 or 1")
            .def_readwrite("imag_bit", &Pauli::imag_bit, "int/bool, either 0 or 1")
            .def(py::init<const std::size_t, const std::size_t, const std::size_t, const bool, const bool>(), "number_qubits"_a, "x_vector"_a, "z_vector"_a, "sign_bit"_a, "imag_bit"_a)
            .def("is_hermitian", &Pauli::is_hermitian, "Returns whether the pauli operator is Hermitian")
            .def("commutes_with", &Pauli::commutes_with, "Given another Pauli, used to check whether it commutes with this Pauli", "other_pauli"_a)
            .def("anticommutes_with", &Pauli::anticommutes_with, "Given another Pauli, used to check whether it anticommutes with this Pauli", "other_pauli"_a)
            .def("has_eigenstate", &Pauli::has_eigenstate, "Given a statevector x on the same number of qubits as the Pauli P, check whether or not Px = (-1)^(eig_sign) x, i.e. whether x is an eigenstate of P with eigenvalue (-1)^(eig_sign).", "vector"_a, "eig_sign"_a)
            .def("get_matrix", &Pauli::get_matrix, "Returns the matrix of the Pauli (with respect to the computational basis)")
            .def("multiply_vector", &Pauli::multiply_vector, "Given a vector x on the same number of qubits as the Pauli P, return Px", "vector"_a)
            .def("multiply_by_pauli_on_right", &Pauli::multiply_by_pauli_on_right, "Given another pauli Q, multiply this Pauli on the right by Q. Note, the current instance is set to the result.", "other_pauli"_a)
            .doc() = "The class used to represent a Pauli operator. Pauli is (-1)^(sign_bit) * (-i)^(imag_bit) * X^(x_vector) * Z^(z_vector). The phase of the Pauli is (-1)^(sign_bit) * (-i)^(imag_bit)";
    }
}

#endif