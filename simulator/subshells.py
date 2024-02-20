"""
The subshells module contains the SubShell and SubShells classes.
"""


class SubShell:
    """
    The SubShell class represents a subshell state of an orbital.

    Notes:
        The subshell is represented as a bit pattern, where the first bit of a sequence of 2 bits represents the spin up
        electron and the second bit represents the spin down electron. The bit pattern is read from left to right, where
        the leftmost bit represents the lowest energy electron and the rightmost bit represents the highest energy.
    """
    def __init__(self, shell: int, size: int):
        """
        The constructor of the SubShell class.

        Args:
            shell (int):
                The shell of the subshell.
        """
        self.__shell = shell
        """int: The shell of the subshell."""
        self.__size = size
        """int: The size of the subshell."""
        self.ms: float = 0
        """float: The ms value of the subshell."""
        self.ml: int = 0
        """int: The ml value of the subshell."""
        self.__calculate_ms_ml(size - 2, ((size // 2) - 1) // 2)

    def __calculate_ms_ml(self, start: int, multiplier: int):
        """
        Calculate the ms and ml values of the subshell.

        Args:
            start (int):
                The starting index of the bit pattern.

            multiplier (int):
                The multiplier of the ml value.
        """
        # The ms and ml values are calculated by iterating through the bit pattern and counting the number of spin up
        # and spin down electrons and the ml value is calculated by multiplying the number of electrons in the
        # subshell by the multiplier.
        for i in range(start, -2, -2):
            bit_i = (self.__shell & (1 << i)) >> i
            bit_i_plus_1 = (self.__shell & (1 << (i + 1))) >> (i + 1)
            self.ms += bit_i_plus_1 - bit_i
            self.ml += (bit_i_plus_1 + bit_i) * (multiplier - (i >> 1))
        self.ms /= 2

    @property
    def shell(self) -> int:
        return self.__shell

    @property
    def size(self) -> int:
        return self.__size


class SubShells:
    """
    The SubShells class represents the possible subshells of an orbital.
    """
    def __init__(self, electrons: int, max_electrons: int):
        """
        The constructor of the Possibilities class.

        Args:
            electrons (int):
                The number of electrons in the atom.

            max_electrons (int):
                The maximum number of electrons in the atom.
        """
        self.electrons = electrons
        """int: The number of electrons in the atom."""
        self.max_electrons = max_electrons
        """int: The maximum number of electrons in the atom."""
        self.possibilities = []
        """list: The list of possibilities."""
        self.__calculate_possibilities()

    def __calculate_possibilities(self):
        """
        Calculate the possible subshells of an orbital.

        Returns:
            list:
                The list of possible subshells.
        """
        def permute(perm, ones, length, i):
            """
            Permute the bit pattern to calculate the possible subshells.

            Notes:
                This is a recursive function that calculates the possible subshells by permuting the bit pattern of the
                subshell.

            Args:
                perm (int):
                    The bit pattern of the subshell.

                ones (int):
                    The number of electrons in the subshell.

                length (int):
                    The length of the bit pattern.

                i (int):
                    The current index of the bit pattern.
            """
            if ones <= 0:
                # If there are no more electrons to place in the subshell, then the subshell is added to the list of
                # possibilities.
                self.possibilities.append(SubShell(perm << (length - i), length))
            elif i < length:
                # If there are still electrons to place in the subshell, then the bit pattern is permuted by placing a
                # spin up or spin down electron in the subshell.
                permute((perm << 1) | 1, ones - 1, length, i + 1)

                if length - i > ones:
                    # If it is possible to place the remaining electrons in the subshell without placing an electron
                    # at the current position, then the bit pattern is permuted by not placing an electron at the
                    # current position.
                    permute(perm << 1, ones, length, i + 1)

        permute(0, self.electrons, self.max_electrons, 0)

    def __repr__(self) -> str:
        return f"<Possibilities {self.possibilities:0{self.max_electrons}b}>"

    def __iter__(self):
        return iter(self.possibilities)
