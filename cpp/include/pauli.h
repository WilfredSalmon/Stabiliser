#pragma once

#include <complex>
#include <span>
#include <vector>

// TODO: add verification for pauli/pauli and pauli/vector multiplication where
// dimensions don't match

namespace fst
{
    /// The class used to represent a Pauli operator.
    /// Puali is (-1)^(sign_bit) * (-i)^(imag_bit) * X^(x_vector) * Z^(z_vector)
    /// The phase of the Pauli is (-1)^(sign_bit) * (-i)^(imag_bit)
    class Pauli
    {
      private:
        std::size_t number_qubits = 0;
        std::size_t x_vector = 0;
        std::size_t z_vector = 0;

        std::complex<float> phase = 1;

        bool sign_bit = 0;
        bool imag_bit = 0;

      public:
        Pauli(const std::size_t number_qubits, const std::size_t x_vector, const std::size_t z_vector,
              const bool sign_bit, const bool imag_bit);

        /// Updates the phase to be consistent with the current sign & imag bits
        void update_phase();

        /// returns whether the pauli operator is Hermitian
        bool is_hermitian() const;

        /// given another Pauli, used to check whether it commutes/anticommutes
        /// with this Pauli
        bool commutes_with(const Pauli &other_pauli) const;
        bool anticommutes_with(const Pauli &other_pauli) const;

        /// Given a statevector x on the same number of qubits as the Pauli P, check
        /// whether or not Px = (-1)^(eig_sign) x, i.e. whether x is an eigenstate of P
        /// with eigenvalue (-1)^(eig_sign).
        bool has_eigenstate(const std::span<const std::complex<float>> vector, const unsigned int eign_sign) const;

        /// Returns the matrix of the Pauli (with respect to the computational basis)
        std::vector<std::vector<std::complex<float>>> get_matrix() const;

        /// Given a vector x on the same number of qubits as the Pauli P, return Px
        std::vector<std::complex<float>> multiply_vector(const std::span<const std::complex<float>> vector) const;

        /// Given another pauli Q, multiply this Pauli on the right by Q
        /// Note, the current instance is set to the result.
        void multiply_by_pauli_on_right(const Pauli &other_pauli);

        [[nodiscard]] std::size_t get_x_vector() const noexcept
        {
            return x_vector;
        }
        [[nodiscard]] std::size_t get_z_vector() const noexcept
        {
            return z_vector;
        }
        [[nodiscard]] bool get_sign_bit() const noexcept
        {
            return sign_bit;
        }
        [[nodiscard]] bool get_imag_bit() const noexcept
        {
            return imag_bit;
        }

        bool operator==(const Pauli &other) const = default;
    };
}
