import time
import pickle
from Benchmarking_Data import Benchmarking_Data
from benchmarking_config import configs
import cProfile
from typing import List

profile = cProfile.Profile()

def time_function_with_generator(function_to_time, generator, qubit_numbers : List[int], reps : int) -> List[List[float]]:
    times = []

    for n in qubit_numbers:
        print(f'n is {n}')
        recorded_times = [.0]*reps

        for j in range(reps):
            if (j % 100 == 0):
                print(f'iteration {j}')
            input = generator(n)

            if type(input) is tuple:
                st = time.perf_counter()
                # profile.enable()
                function_to_time(*input)
                # profile.disable()
                et = time.perf_counter()
            else:
                st = time.perf_counter()
                # profile.enable()
                function_to_time(input)
                # profile.disable()
                et = time.perf_counter()

            recorded_times[j] = et - st
    
        recorded_times.sort()
        times.append(recorded_times)

    return times

base_filestring = './data'

def append_benchmarking_data(pre_string='', functions_to_time=[], 
                             function_strings=[], generation_types=[], generation_strings=[], min_qubit_number = 1, max_qubit_number = 1, reps = 1):
    num_functions = len(functions_to_time)
    num_generators = len(generation_types)

    assert num_functions == len(function_strings)
    assert num_generators == len(generation_strings)

    qubit_numbers = list(range(min_qubit_number, max_qubit_number + 1))

    for function_index in range(num_functions):
        for generation_index in range(num_generators):
            function_string = function_strings[function_index]
            generation_string = generation_strings[generation_index]

            filename = f'{base_filestring}/{pre_string} {function_string} on {generation_string}.npy'

            print(f'timing {function_string} with {generation_string}')

            times = time_function_with_generator(
                functions_to_time[function_index], 
                generation_types[generation_index],
                qubit_numbers,
                reps
            )

            data = Benchmarking_Data(
                function_string, generation_string, qubit_numbers, times, reps)

            with open(filename, 'wb') as fl:
                pickle.dump(data, fl)


if __name__ == '__main__':
    for config in configs:
        append_benchmarking_data(**config)
