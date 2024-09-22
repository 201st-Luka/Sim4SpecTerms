"""
This module contains the Configuration and Configurations classes.
"""

import math
from typing import Iterator

from .subshells import SubShells, SubShell

ARROW_UP = "\u21bF"
ARROW_DOWN = "\u21C2"
ARROW_BOTH = "\u296E"


class Configuration:
    """
    The Configuration class is used to represent a configuration of a given electron configuration at a specific state.
    """

    def __init__(self, s: SubShell, p: SubShell, d: SubShell, f: SubShell, ms: float = None, ml: int = None):
        """
        The constructor of the Possibility class.

        Args:
            s (SubShell):
                The s subshell.

            p (SubShell):
                The p subshell.

            d (SubShell):
                The d subshell.

            f (SubShell):
                The f subshell.
        """
        self.__configuration = (s, p, d, f)
        """int: The configuration, bits from left to right: 2 s, 6 p, 10 d, 14 f."""
        self.ms = ms or s.ms + p.ms + d.ms + f.ms
        """float: The ms value of the configuration."""
        self.ml = ml or s.ml + p.ml + d.ml + f.ml
        """int: The ml value of the configuration."""

    def __calculate_ms_ml(self, subshell: int, start_: int, multiplier: int):
        """
        Calculate the ms and ml values of the configuration.

        Args:
            subshell (int):
                The subshell to calculate the ms and ml values.

            start_ (int):
                The start bit of the subshell.

            multiplier (int):
                The multiplier for the ml value.
        """
        for i in range(start_, -2, -2):
            # Get the electron at position i.
            bit_i = (subshell & (1 << i)) >> i
            # Get the electron at position i + 1.
            bit_i_plus_1 = (subshell & (1 << (i + 1))) >> (i + 1)

            self.ms += bit_i_plus_1 - bit_i
            self.ml += (bit_i_plus_1 + bit_i) * (multiplier - (i >> 1))

    @property
    def configuration(self) -> int:
        """
        Get the configuration of the atom.

        Returns:
            int: The configuration of the atom.
            Note that the electrons are stored in bits from left to right: 2 s, 6 p, 10 d, 14 f in the following format:
            ssppppddddddffffffffffffff, 32 electrons in total. The first electron of each subshell is the spin up
            electron followed by the spin down electron (second electron).
        """
        return (
                ((
                         ((
                                  (self.__configuration[0].shell << 6)
                                  | self.__configuration[1].shell) << 10)
                         | self.__configuration[2].shell) << 14)
                | self.__configuration[3].shell)

    def __repr__(self) -> str:
        return f"<Configuration {self.ms=}, {self.ml=}>"

    @staticmethod
    def __shell_to_arrow(shell: SubShell, shell_index: int) -> str:
        bits_to_arrow = {
            0: "",
            1: ARROW_DOWN,
            2: ARROW_UP,
            3: ARROW_BOTH,
        }

        shell_index *= 2

        return bits_to_arrow[(shell.shell & (3 << shell_index)) >> shell_index]

    def __shell_to_arrows(self) -> list[str]:
        """
        Convert the configuration to arrows.

        Returns:
            list[str]: The list of arrows.
        """

        return [self.__shell_to_arrow(self.__configuration[0], 0)] + [
            self.__shell_to_arrow(self.__configuration[0], i) for i in range(0, 3)[::-1]
        ] + [
            self.__shell_to_arrow(self.__configuration[0], i) for i in range(0, 5)[::-1]
        ] + [
            self.__shell_to_arrow(self.__configuration[0], i) for i in range(0, 7)[::-1]
        ]

    def to_list(self) -> list:
        """
        Convert the configuration to a list.

        Returns:
            list: The list of arrows and the ml and ms value.
        """
        return self.__shell_to_arrows() + [self.ml, self.ms]

    @property
    def s_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[0], 0)

    @property
    def p_1_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[1], 2)

    @property
    def p0_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[1], 1)

    @property
    def p1_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[1], 0)

    @property
    def d_2_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[2], 4)

    @property
    def d_1_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[2], 3)

    @property
    def d0_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[2], 2)

    @property
    def d1_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[2], 1)

    @property
    def d2_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[2], 0)

    @property
    def f_3_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[3], 6)

    @property
    def f_2_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[3], 5)

    @property
    def f_1_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[3], 4)

    @property
    def f0_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[3], 3)

    @property
    def f1_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[3], 2)

    @property
    def f2_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[3], 1)

    @property
    def f3_arrow(self) -> str:
        return self.__shell_to_arrow(self.__configuration[3], 0)


class Configurations:
    """
    The Configurations class is used to represent all the possible configurations of a given electron configuration.
    """

    __max_possible_ms = 8
    """int: The maximum possible value of the ms value."""
    __max_possible_ml = 20
    """int: The maximum possible value of the ml value."""

    def __init__(self, s: int, p: int, d: int, f: int):
        """
        The constructor of the Possibility class.

        Args:
            s (int):
                The s subshell electron count.

            p (int):
                The p subshell electron count.

            d (int):
                The d subshell electron count.

            f (int):
                The f subshell electron count.
        """
        self.__electrons = (s, p, d, f)
        """tuple: The number of electrons in each subshell."""
        self.__combinations_count: tuple = (math.comb(2, s), math.comb(6, p),
                                            math.comb(10, d), math.comb(14, f))
        """tuple: The number of combinations for each subshell."""
        self.__combinations = (SubShells(s, 2), SubShells(p, 6),
                               SubShells(d, 10), SubShells(f, 14))
        """tuple: The possibilities for each subshell."""
        self.__index = self.__build_index()
        """list[list[dict[int, Configuration]]]: A nested list of the configurations for faster access and filtering"""

    def __build_index(self) -> list[list[list[int]]]:
        """
        Build the index of the configurations.

        Returns:
            list[list[list[int]]]: The index of the configurations.
        """
        index = [
            [
                [] for _ in range(-self.__max_possible_ms, self.__max_possible_ms + 1)
            ] for _ in range(-self.__max_possible_ml, self.__max_possible_ml + 1)
        ]

        for i, config in enumerate(self):
            index[config.ml][int(config.ms)].append(i)  # ms and ml can be negative -> negative index in index list

        return index

    def __combinations_iterator(self) -> Iterator[Configuration]:
        """
        Iterate over all the possible configurations.

        Returns:
            Iterator[Configuration]: The iterator of configurations.
        """
        for s in self.__combinations[0].possibilities:
            for p in self.__combinations[1].possibilities:
                sp_ms, sp_ml = s.ms + p.ms, s.ml + p.ml
                for d in self.__combinations[2].possibilities:
                    spd_ms, spd_ml = sp_ms + d.ms, sp_ml + d.ml
                    for f in self.__combinations[3].possibilities:
                        yield Configuration(s, p, d, f, spd_ms + f.ms, spd_ml + f.ml)

    def __get_single_configuration(self, index: int) -> Configuration:
        """
        Get a single configuration from the index.

        Args:
            index:
                The index of the configuration.

        Returns:
            Configuration: The configuration.
        """
        # getting the subshell possibilities from the index by extracting the indexes of the possibilities subshells
        s = self.__combinations[0].possibilities[
            index // (self.__combinations_count[1] * self.__combinations_count[2] * self.__combinations_count[3])
            ]
        p = self.__combinations[1].possibilities[
            (index // (self.__combinations_count[2] * self.__combinations_count[3])) % self.__combinations_count[1]
            ]
        d = self.__combinations[2].possibilities[
            (index // self.__combinations_count[3]) % self.__combinations_count[2]
            ]
        f = self.__combinations[3].possibilities[
            index % self.__combinations_count[3]
            ]

        return Configuration(s, p, d, f)

    def __get_slice_configurations(self, index: slice) -> list[Configuration]:
        """
        Get a list of configurations from the slice index.

        Args:
            index:
                The slice index.
        Returns:
            list[Configuration]: The list of configurations.
        """
        return [self.__get_single_configuration(i) for i in range(*index.indices(len(self)))]

    @property
    def electrons(self) -> tuple[int, int, int, int]:
        """
        Get the number of electrons in each subshell.

        Returns:
            tuple[int, int, int, int]: The number of electrons in each subshell.
        """
        return self.__electrons

    def filter(self, terms: list['Term'] = None, start_index: int = 0, length: int = None):
        """
        Filter the configurations by the ms and ml values.

        Args:
            terms (list[Term]):
                The list of terms that is should be filtered.

            start_index (int):
                The start index of the configurations.

            length (int):
                The length of the slice.

        Returns:
            list[Configuration]: The list of filtered configurations.
        """
        if terms:
            filtered_configurations = set()
            for term in terms:
                for l in range(-term.l, term.l + 1):
                    for s in range(int(-term.s), int(term.s) + 1):
                        filtered_configurations.update(self.__index[l][s])
            # filtered_configurations.update(self.__index[term.l][int(term.s)])
            # filtered_configurations.update(self.__index[term.l][int(-term.s)])
            # filtered_configurations.update(self.__index[-term.l][int(-term.s)])
            # filtered_configurations.update(self.__index[-term.l][int(term.s)])

            sorted_filtered_configurations = sorted(filtered_configurations)
            end_index = (start_index + length) if length else len(sorted_filtered_configurations)

            return [self[i] for i in sorted_filtered_configurations[start_index:end_index]]
        else:
            return self[start_index:start_index + length or len(self)]

    def max_ms(self) -> float:
        """
        Get the maximum ms value.

        Returns:
            float: The maximum ms value.
        """
        for i in range(Configurations.__max_possible_ms, -1, -1):
            if self.__index[i]:
                if sum(self.__electrons) % 2 == 0:
                    return i
                return i + 0.5
        return 0


    def max_ml(self) -> int:
        """
        Get the maximum ml value.

        Returns:
            int: The maximum ml value.
        """
        for i in range(Configurations.__max_possible_ml, -1, -1):
            if self.__index[i]:
                return i
        return 0

    def count_ml_ms(self) -> list[list[int]]:
        """
        Count the number of configurations for each ml and ms value.

        Returns:
            list[list[int]]:
                The list of configurations for each ml and ms value.

                Each row represents the ml value and each column represents the ms value.
        """
        return [
            [
                len(index) for index in indexes[:Configurations.__max_possible_ms + 1]
            ] for indexes in self.__index[:Configurations.__max_possible_ml + 1]
        ]

    def __getitem__(self, index: int | slice) -> Configuration | list[Configuration]:
        if isinstance(index, slice):
            return self.__get_slice_configurations(index)
        return self.__get_single_configuration(index)

    def __len__(self) -> int:
        return (self.__combinations_count[0] * self.__combinations_count[1]
                * self.__combinations_count[2] * self.__combinations_count[3])

    def __iter__(self) -> Iterator[Configuration]:
        return iter(self.__combinations_iterator())

    def __repr__(self) -> str:
        return f"<Configurations {self.__electrons=}>"

    def __hash__(self):
        return hash(self.__electrons)
