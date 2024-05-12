#pragma once

#include <complex>
#include <span>

#include "stabiliser_state.h"

namespace fst
{
    /// Convert a state vector of complex amplitudes into a stabiliser state object.
    ///
    /// Assuming valid is faster, but will result in undefined behaviour if the state vector is not in fact a
    /// valid stabaliser state
    Stabiliser_State stabiliser_from_statevector(const std::span<const std::complex<float>> statevector,
                                                 bool assume_valid = false);

    bool is_stabiliser_state(const std::span<const std::complex<float>> statevector);
}
