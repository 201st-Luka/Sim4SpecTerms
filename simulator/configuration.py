"""
This module contains the Configuration and Configurations classes.
"""

import math
from typing import Iterator

from .subshells import SubShells, SubShell


ARROW_UP   = "\u21bF"
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

    def __calculate_ms_ml(self, subshell: int, start: int, multiplier: int):
        """
        Calculate the ms and ml values of the configuration.

        Args:
            subshell (int):
                The subshell to calculate the ms and ml values.

            start (int):
                The start bit of the subshell.

            multiplier (int):
                The multiplier for the ml value.
        """
        for i in range(start, -2, -2):
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

    def __shell_to_arrows(self) -> list[str]:
        """
        Convert the configuration to arrows.

        Returns:
            list[str]: The list of arrows.
        """
        bits_to_arrow = {
            0: "",
            1: ARROW_DOWN,
            2: ARROW_UP,
            3: ARROW_BOTH,
        }

        return [bits_to_arrow[self.__configuration[0].shell]] + [
            bits_to_arrow[(self.__configuration[1].shell & (3 << i)) >> i] for i in range(0, 6, 2)[::-1]
        ] + [
            bits_to_arrow[(self.__configuration[2].shell & (3 << i)) >> i] for i in range(0, 10, 2)[::-1]
        ] + [
            bits_to_arrow[(self.__configuration[3].shell & (3 << i)) >> i] for i in range(0, 14, 2)[::-1]
        ]

    def to_list(self) -> list:
        """
        Convert the configuration to a list.

        Returns:
            list: The list of arrows and the ml and ms value.
        """
        return self.__shell_to_arrows() + [self.ml, self.ms]


class Configurations:
    """
    The Configurations class is used to represent all the possible configurations of a given electron configuration.
    """
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
        self.positive_ms_ml = False
        """bool: Whether to include only positive ms and ml values on iterations (default: False). This variable is 
        used to speed up the group creation process."""

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

    def __combinations_iterator_positive_ms_ml(self) -> Iterator[Configuration]:
        """
        Iterate over all the possible configurations with positive ms and ml values.

        Notes:
            The returned generator is used to speed up the group creation process.

        Returns:
            Iterator[Configuration]: The iterator of configurations.
        """
        for s in self.__combinations[0].possibilities:
            for p in self.__combinations[1].possibilities:
                sp_ms, sp_ml = s.ms + p.ms, s.ml + p.ml
                for d in self.__combinations[2].possibilities:
                    spd_ms, spd_ml = sp_ms + d.ms, sp_ml + d.ml
                    for f in self.__combinations[3].possibilities:
                        if (ms := spd_ms + f.ms) >= 0 and (ml := spd_ml + f.ml) >= 0:
                            yield Configuration(s, p, d, f, ms, ml)

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

    def __getitem__(self, index: int | slice) -> Configuration | list[Configuration]:
        if isinstance(index, slice):
            return self.__get_slice_configurations(index)
        return self.__get_single_configuration(index)

    def __len__(self) -> int:
        return (self.__combinations_count[0] * self.__combinations_count[1]
                * self.__combinations_count[2] * self.__combinations_count[3])

    def __iter__(self) -> Iterator[Configuration]:
        if self.positive_ms_ml:
            return iter(self.__combinations_iterator_positive_ms_ml())
        return iter(self.__combinations_iterator())

    def __repr__(self) -> str:
        return f"<Configurations {self.__electrons=}>"
