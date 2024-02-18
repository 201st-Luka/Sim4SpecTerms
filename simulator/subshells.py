class SubShell:
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
        self.__ms: float = 0
        """float: The ms value of the subshell."""
        self.__ml: int = 0
        """int: The ml value of the subshell."""
        self.__calculate_ms_ml(size - 2, ((size // 2) - 1) // 2)

    def __calculate_ms_ml(self, start: int, multiplier: int):
        for i in range(start, -2, -2):
            bit_i = (self.__shell & (1 << i)) >> i
            bit_i_plus_1 = (self.__shell & (1 << (i + 1))) >> (i + 1)
            self.__ms += bit_i_plus_1 - bit_i
            self.__ml += (bit_i_plus_1 + bit_i) * (multiplier - (i >> 1))
        self.__ms /= 2

    @property
    def shell(self) -> int:
        return self.__shell

    @property
    def size(self) -> int:
        return self.__size

    @property
    def ms(self) -> float:
        return self.__ms

    @property
    def ml(self) -> int:
        return self.__ml


class SubShells:
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
        def permute(perm, ones, length, i):
            if ones <= 0:
                self.possibilities.append(SubShell(perm << (length - i), length))
            elif i < length:
                permute((perm << 1) | 1, ones - 1, length, i + 1)
                if length - i > ones:
                    permute(perm << 1, ones, length, i + 1)

        permute(0, self.electrons, self.max_electrons, 0)

    def __repr__(self) -> str:
        return f"<Possibilities {self.possibilities:0{self.max_electrons}b}>"

    def __iter__(self):
        return iter(self.possibilities)
