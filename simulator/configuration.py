from typing import Iterator

from .subshells import SubShells, SubShell


ARROW_UP   = "\u21bF"
ARROW_DOWN = "\u21C2"
ARROW_BOTH = "\u296E"


class Configuration:
    def __init__(self, s: SubShell, p: SubShell, d: SubShell, f: SubShell):
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
        self.__ms = s.ms + p.ms + d.ms + f.ms
        """float: The ms value of the configuration."""
        self.__ml = s.ml + p.ml + d.ml + f.ml
        """int: The ml value of the configuration."""

    def __calculate_ms_ml(self, subshell: int, start: int, multiplier: int):
        for i in range(start, -2, -2):
            bit_i = (subshell & (1 << i)) >> i
            bit_i_plus_1 = (subshell & (1 << (i + 1))) >> (i + 1)
            self.__ms += bit_i_plus_1 - bit_i
            self.__ml += (bit_i_plus_1 + bit_i) * (multiplier - (i >> 1))

    @property
    def configuration(self) -> int:
        return (
                ((
                         ((
                          (self.__configuration[0].shell << 6)
                          | self.__configuration[1].shell) << 10)
                         | self.__configuration[2].shell) << 14)
                | self.__configuration[3].shell)

    @property
    def ms(self) -> float:
        return self.__ms

    @property
    def ml(self) -> int:
        return self.__ml

    def __repr__(self) -> str:
        return f"<Configuration {self.ms=}, {self.ml=}>"

    def __shell_to_arrows(self) -> list[str]:
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
        return self.__shell_to_arrows() + [self.ml, self.ms]


class Configurations:
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
        self.__configurations: list
        """list: The list of configurations."""
        self.__calculate_configurations(s, p, d, f)

    def __calculate_configurations(self, s: int, p: int, d: int, f: int):
        possibilities_s = SubShells(s, 2)
        possibilities_p = SubShells(p, 6)
        possibilities_d = SubShells(d, 10)
        possibilities_f = SubShells(f, 14)

        self.__configurations = [
            Configuration(s, p, d, f)
            for s in possibilities_s
            for p in possibilities_p
            for d in possibilities_d
            for f in possibilities_f
        ]

    def __getitem__(self, index: int | slice) -> Configuration | list[Configuration]:
        if isinstance(index, slice):
            return self.__configurations[index]
        return self.__configurations[index]

    def __len__(self) -> int:
        return len(self.__configurations)

    def __iter__(self) -> Iterator[Configuration]:
        return iter(self.__configurations)

    def __repr__(self) -> str:
        return f"<Configurations {self.__configurations=}>"
