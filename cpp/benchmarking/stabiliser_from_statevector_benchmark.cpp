#include <benchmark/benchmark.h>

#include <vector>
#include <complex>

#include "stim/circuit_vs_amplitudes.h"
#include "stabiliser_state_from_statevector.h"
#include "f2_helper.h"

struct last_basis_vector
{
	static std::vector<std::complex<float>> generate( const std::size_t number_qubits )
	{
		const std::size_t vector_length = fst::integral_pow_2( number_qubits );

		std::vector<std::complex<float>> statevector( vector_length, 0 );

		statevector.back() = { 0, 1 };

		return statevector;
	}

	static constexpr bool expected_result = true;
};

struct uniform_superposition
{
	static std::vector<std::complex<float>> generate( const std::size_t number_qubits )
	{
		const std::size_t vector_length = fst::integral_pow_2( number_qubits );
		const float normalisation_factor = static_cast<float>( 1.0f / std::sqrt( vector_length ) );

		return std::vector<std::complex<float>>( vector_length, normalisation_factor );
	}

	static constexpr bool expected_result = true;
};

struct non_stabiliser_state
{
	static std::vector<std::complex<float>> generate( const std::size_t number_qubits )
	{
		const std::size_t vector_length = fst::integral_pow_2( number_qubits );
		const float normalisation_factor = static_cast<float>( 1.0f / std::sqrt( vector_length ) );

		std::vector<std::complex<float>> statevector( vector_length, normalisation_factor );

		statevector.back() *= -1;

		return statevector;
	}

	static constexpr bool expected_result = false;
};

template<auto SimulationFunction, typename TData>
static void BM_Stabalizer( benchmark::State& state )
{
	const std::size_t number_qubits = state.range();
	const std::vector<std::complex<float>> stabalizer_data = TData::generate( number_qubits );

	for ( auto _ : state )
	{
		bool result = SimulationFunction( stabalizer_data );
		assert( result == TData::expected_result );
		benchmark::DoNotOptimize( result );
	}
}

BENCHMARK( BM_Stabalizer<fst::is_stabiliser_state, last_basis_vector> )->DenseRange( 3, 10 );
BENCHMARK( BM_Stabalizer<fst::is_stabiliser_state, uniform_superposition> )->DenseRange( 3, 10 );
BENCHMARK( BM_Stabalizer<fst::is_stabiliser_state, non_stabiliser_state> )->DenseRange( 3, 10 );
BENCHMARK( BM_Stabalizer<stim::stabilizer_state_vector_to_circuit, last_basis_vector> )->DenseRange( 3, 10 );
BENCHMARK( BM_Stabalizer<stim::stabilizer_state_vector_to_circuit, uniform_superposition> )->DenseRange( 3, 10 );
BENCHMARK( BM_Stabalizer<stim::stabilizer_state_vector_to_circuit, non_stabiliser_state> )->DenseRange( 3, 10 );
