import math
import unittest
import numpy as np
import stabiliser_state.stabiliser_check as sc
from stabiliser_state.Stabiliser_State import Stabiliser_State

class Test_Stabiliser_State_Check(unittest.TestCase):

    three_stab_state = Stabiliser_State(3, [3], 1, 2, [6,1], 4)
    five_stab_state = Stabiliser_State(5, [3,5], 4, 2, [9, 22, 16], 1)
    
    def test_is_stab_state_accepts_basis_state(self):
        state = np.array([0,1])

        self.assertTrue(sc.Stabiliser_Checker().load_vector(state).is_stab_state)

    def test_is_stab_state_accepts_three_qubit_state(self):
        state = self.three_stab_state.generate_state_vector()

        self.assertTrue(sc.Stabiliser_Checker().load_vector(state).is_stab_state)

    def test_is_stab_state_rejects_with_incorrect_support_size(self):
        state = self.three_stab_state.generate_state_vector()
        
        state[0] = 1 # add element to the support

        self.assertFalse(sc.Stabiliser_Checker().load_vector(state).is_stab_state)

    def test_is_stab_state_rejects_when_support_not_affine_space(self):
        state = self.five_stab_state.generate_state_vector()
        
        state[1] = 0 # remove element of affine space
        state[0] = 1 # add non-affine space element to support

        self.assertFalse(sc.Stabiliser_Checker().load_vector(state).is_stab_state)

    def test_is_stab_state_rejects_with_global_factor(self):
        state = np.exp(2j)*self.three_stab_state.generate_state_vector()

        self.assertFalse(sc.Stabiliser_Checker().load_vector(state).is_stab_state)
    
    def test_is_stab_state_accpets_with_global_factor_when_flagged(self):
        state = (1/math.sqrt(2))*(1+1j)*self.five_stab_state.generate_state_vector()

        self.assertTrue(sc.Stabiliser_Checker().load_vector(state, allow_global_factor = True).is_stab_state)

    def test_is_stab_state_rejects_with_invalid_linear_entry(self):
        state = self.five_stab_state.generate_state_vector()
        
        state[7] = 2 # invalid weight 1 term (first basis vector)

        self.assertFalse(sc.Stabiliser_Checker().load_vector(state).is_stab_state)

    def test_is_stab_state_rejects_with_invalid_quadratic_entry(self):
        state = self.five_stab_state.generate_state_vector()
        
        state[14] = -2j # invalid weight 2 term (e1 + e2)

        self.assertFalse(sc.Stabiliser_Checker().load_vector(state).is_stab_state)

    def test_is_stab_state_rejects_with_inconsistent_entry(self):
        state = self.five_stab_state.generate_state_vector()
        
        state[30] = -1j # change (e1+e2+e3) element (should be 1j)

        self.assertFalse(sc.Stabiliser_Checker().load_vector(state).is_stab_state)

    def test_get_stab_state_returns_correct_state(self):
        state_vector = np.array([0,-1j, 1, 0, 1j, 0, 0, 1])

        state = sc.Stabiliser_Checker().load_vector(state_vector, allow_global_factor = True).get_stab_state()

        self.assertEqual(state.shift, 1)
        self.assertEqual(state.vector_basis, [3, 5])
        self.assertEqual(state.real_linear_part, 2)
        self.assertEqual(state.imaginary_part, 1)
        self.assertEqual(state.quadratic_form, [3])
        self.assertEqual(state.global_factor, -2j)

    def test_get_stab_state_raises_error_on_invalid_state(self):
        state_vector = np.array([1, (1/math.sqrt(2))*(1+1j)])

        state = sc.Stabiliser_Checker().load_vector(state_vector, allow_global_factor = True)

        self.assertRaises(ValueError, state.get_stab_state)

    def test_get_stab_state_raises_error_when_state_not_loaded(self):
        state = sc.Stabiliser_Checker()

        self.assertRaises(ValueError, state.get_stab_state)