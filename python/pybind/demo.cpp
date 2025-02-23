#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <vector>

namespace py = pybind11;

std::vector<double> create_vector(int n)
{
    std::vector<double> vec(n);
    return vec;
}

py::array_t<double> create_matrix_array(int n) {
    std::vector<std::array<std::array<double, 3>, 3>> data(n);

    // Fill with some example values
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                data[i][j][k] = static_cast<double>(i * 9 + j * 3 + k);
            }
        }
    }

    // Copy to a new owned numpy array
    py::array_t<double> result({n, 3, 3});
    std::memcpy(result.mutable_data(), data.data(), n * 3 * 3 * sizeof(double));

    return result;
}

PYBIND11_MODULE(demo, m) {
    m.def("create_matrix_array", &create_matrix_array);
    m.def("create_vector", &create_vector, py::return_value_policy::move);
}
