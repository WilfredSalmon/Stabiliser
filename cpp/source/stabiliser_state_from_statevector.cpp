#include "stabiliser_state_from_statevector.h"

#include "f2_helper.h"

#include <algorithm>
#include <optional>
#include <vector>

namespace
{
	template <bool assume_valid, bool return_state>
	auto stabiliser_from_statevector_internal(const std::span<const std::complex<float>> statevector)
		-> std::conditional_t<return_state, std::optional<fst::Stabiliser_State>, bool>
	{
		using namespace fst;

		const std::size_t state_vector_size = statevector.size();

		if (!is_power_of_2(state_vector_size))
		{
			return {};
		}

		std::size_t shift = 0;

		while (shift < state_vector_size && statevector[shift] == float(0))
		{
			++shift;
		}

		if (shift == state_vector_size)
		{
			return {};
		}

		const std::size_t indices_capacity = state_vector_size - shift + 1;
		std::vector<std::size_t> vector_space_indices;
		vector_space_indices.reserve( indices_capacity );
		vector_space_indices.push_back(0);

		for (std::size_t index = shift + 1; index < state_vector_size; index++)
		{
			if (statevector[index] != float(0))
			{
				vector_space_indices.push_back(shift ^ index);
			}
		}

		const std::size_t support_size = vector_space_indices.size();
		assert( support_size <= indices_capacity ); // Our reserve should always be correct capacity to avoid re-allocations

		if (!is_power_of_2(support_size))
		{
			return {};
		}

		const std::size_t dimension = integral_log_2(support_size);
		const float normalisation_factor = static_cast<float>(std::sqrt(support_size));
		const std::complex<float> first_entry = statevector[shift];
		const std::complex<float> global_phase = normalisation_factor * first_entry;

		if (std::abs(std::norm(global_phase) - 1) >= 0.125)
		{
			return {};
		}

		std::ranges::sort(vector_space_indices);

		std::vector<std::size_t> basis_vectors;
		basis_vectors.reserve(dimension);

		std::size_t real_linear_part = 0;
		std::size_t imaginary_part = 0;

		for (std::size_t j = 0; j < dimension; j++)
		{
			const std::size_t weight_one_string = integral_pow_2(j);

			const std::size_t basis_vector = vector_space_indices[weight_one_string];
			basis_vectors.push_back(basis_vector);

			const std::complex<float> phase = statevector[basis_vector ^ shift] / first_entry;

			if (std::norm(phase - float(-1)) < 0.125)
			{
				real_linear_part ^= weight_one_string;
			}
			else if (std::norm(phase - std::complex<float>{0, 1}) < 0.125)
			{
				imaginary_part ^= weight_one_string;
			}
			else if (std::norm(phase - std::complex<float>{0, -1}) < 0.125)
			{
				real_linear_part ^= weight_one_string;
				imaginary_part ^= weight_one_string;
			}
			else if (std::norm(phase - float(1)) >= 0.125)
			{
				return {};
			}
		}

		std::vector<std::size_t> quadratic_form;
		quadratic_form.reserve(dimension * dimension);

		for (std::size_t j = 0; j < dimension; j++)
		{
			for (std::size_t i = j + 1; i < dimension; i++)
			{
				const std::size_t vector_index = integral_pow_2(i) | integral_pow_2(j);

				const float real_linear_eval = sign_f2_dot_product(vector_index, real_linear_part);
				const std::complex<float> imag_linear_eval = imag_f2_dot_product(vector_index, imaginary_part);
				const std::complex<float> linear_eval = real_linear_eval * imag_linear_eval;

				const std::size_t total_index = vector_space_indices[vector_index] ^ shift;

				const std::complex<float> quadratic_form_eval = statevector[total_index] / (first_entry * linear_eval);

				if (std::norm(quadratic_form_eval - float(-1)) < 0.125)
				{
					quadratic_form.push_back(vector_index);
				}
				else if (std::norm(quadratic_form_eval - float(1)) >= 0.125)
				{
					return {};
				}
			}
		}

		if constexpr (!assume_valid)
		{
			const std::span<const std::size_t> quadratic_form_span(quadratic_form);
			std::size_t old_vector_index = 0;
			std::size_t total_index = shift;

			for (std::size_t i = 1; i < support_size; i++)
			{
				// iterate through the gray code
				const std::size_t new_vector_index = i ^ (i >> 1);

				const std::size_t flipped_bit = integral_log_2(new_vector_index ^ old_vector_index);
				total_index ^= basis_vectors[flipped_bit];

				const std::complex<float> actual_phase = statevector[total_index];
				const float real_linear_eval = sign_f2_dot_product(new_vector_index, real_linear_part);
				const std::complex<float> imag_linear_eval = imag_f2_dot_product(new_vector_index, imaginary_part);
				const std::complex<float> quadratic_eval = evaluate_quadratic_form(new_vector_index, quadratic_form_span);
				const std::complex<float> phase_eval = real_linear_eval * imag_linear_eval * quadratic_eval;

				if (std::norm(phase_eval * first_entry - actual_phase) >= 0.001)
				{
					return {};
				}

				old_vector_index = new_vector_index;
			}
		}

		if constexpr (return_state)
		{
			Stabiliser_State state( integral_log_2( state_vector_size ), dimension);
			state.shift = shift;
			state.basis_vectors = std::move(basis_vectors);
			state.real_linear_part = real_linear_part;
			state.imaginary_part = imaginary_part;
			state.quadratic_form = std::move(quadratic_form);
			state.global_phase = global_phase;
			state.row_reduced = true;
			return state;
		}
		else
		{
			return true;
		}
	}
}

fst::Stabiliser_State fst::stabiliser_from_statevector(const std::span<const std::complex<float>> statevector, bool assume_valid)
{
	std::optional<Stabiliser_State> state = assume_valid
												? stabiliser_from_statevector_internal<true, true>(statevector)
												: stabiliser_from_statevector_internal<false, true>(statevector);

	if (!state)
	{
		throw std::invalid_argument("State was not a stabiliser state");
	}

	return *std::move(state);
}

bool fst::is_stabiliser_state(const std::span<const std::complex<float>> statevector)
{
	return stabiliser_from_statevector_internal<false, false>(statevector);
}