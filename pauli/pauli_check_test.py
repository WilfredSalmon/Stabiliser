from .pauli_check import is_pauli
import unittest
from .pauli_class import Pauli

class Test_Pauli_Check(unittest.TestCase):

    def test_is_pauli_accepts_single_qubit_pauli(self):
        pauli = Pauli(1, 1, 1, 1, 1).generate_matrix()

        self.assertTrue(is_pauli(pauli))

    def test_is_pauli_accepts_five_qubit_pauli(self):
        pauli = Pauli(5, 28, 3, 0, 1).generate_matrix()
    
        self.assertTrue(is_pauli(pauli))

    def test_is_pauli_rejects_five_qubit_pauli_with_global_factor(self):
        pauli = Pauli(5, 28, 3, 0, 1)
        matrix = 2*pauli.generate_matrix()
        
        self.assertFalse(is_pauli(matrix))

    def test_is_pauli_accpets_five_qubit_pauli_with_global_factor_when_flagged(self):
        pauli = Pauli(5, 28, 3, 0, 1)
        matrix = (3-2j)*pauli.generate_matrix()
        
        self.assertTrue(is_pauli(matrix, allow_global_factor = True))

    def test_is_pauli_rejects_pauli_with_extra_entry(self):
        pauli = Pauli(3, 4, 1, 0, 0)

        corrupt_pauli = pauli.generate_matrix()
        corrupt_pauli[1,1] = 1 # add an extra 1

        self.assertFalse(is_pauli(corrupt_pauli))

    def test_is_pauli_rejects_pauli_with_first_column_all_zeros(self):
        pauli = Pauli(3, 4, 1, 0, 0)

        corrupt_pauli = pauli.generate_matrix()
        corrupt_pauli[1,1] = 1 # add an extra 1
        corrupt_pauli[4,0] = 0 # remove only element in first column

        self.assertFalse(is_pauli(corrupt_pauli))

    def test_is_pauli_rejects_pauli_with_first_column_invalid_entry(self):
        pauli = Pauli(3, 4, 1, 0, 0)

        corrupt_pauli = pauli.generate_matrix()
        corrupt_pauli[4,0] = 1 + 1j # modify element in first column to be invalid 

        self.assertFalse(is_pauli(corrupt_pauli))

    def test_is_pauli_rejects_pauli_with_q_column_invalid_entry(self):
        pauli = Pauli(3, 4, 1, 0, 0)

        corrupt_pauli = pauli.generate_matrix()
        corrupt_pauli[5,1] = 1 + 1j # modify element in second column (q column) to be invalid

        self.assertFalse(is_pauli(corrupt_pauli))

    def test_is_pauli_rejects_pauli_with_remaining_column_invalid_entry(self):
        pauli = Pauli(3, 4, 1, 1, 0)

        corrupt_pauli = pauli.generate_matrix()
        corrupt_pauli[1,5] = -1 # modify element in 5th column (remaining column) to be incorrect

        self.assertFalse(is_pauli(corrupt_pauli))

if __name__ == '__main__':
    unittest.main()        