"""
This module contains the Groups class, which is used to calculate the terms of a given configuration.
"""

from collections import Counter

from .configuration import Configurations
from .utils import float_range, float_range_equal


class Term:
    """
    The Term class is used to represent a term of a given group.
    """
    def __init__(self, s: float, l: int, count: int = 1):
        """
        The constructor of the Term class.

        Args:
            s (float):
                The s value of the term.

            l (int):
                The l value of the term.

            count (int):
                The count of the term.
        """
        self.term_letter: str
        self.sup: int
        self.sub: list[float]

        self.count = count
        self.s = s
        self.l = l

        self.__build_term(s, l)

    def __build_term(self, s: float, l: int):
        # dictionary to translate the l value to the term letter
        ml_matcher = {
            0: "S",
            1: "P",
            2: "D",
            3: "F",
            4: "G",
            5: "H",
            6: "I",
            7: "K",
            8: "L",
            9: "M",
            10: "N",
            11: "O",
            12: "Q",
            13: "R",
            14: "T",
            15: "U",
            16: "V",
            17: "W",
            18: "X",
            19: "Y",
            20: "Z",
        }

        self.term_letter = ml_matcher[l]

        self.sup = int(2 * s) + 1
        self.sub = [l_ for l_ in float_range_equal(l + s, abs(l - s))]

    def __repr__(self) -> str:
        return f"<Term {self.sup=}, {self.term_letter=}, {self.sub=}>"

    def to_list(self) -> list:
        return [self.l, self.s, self.count, self.sup, self.term_letter, self.sub]


class Groups:
    """
    The Groups class is used to calculate the terms of a given configuration.
    """
    def __init__(self, configurations: Configurations):
        """
        The constructor of the Groups class.
        Args:
            configurations (Configurations):
                The configurations.
        """
        configurations.positive_ms_ml = True

        self.__groups = self.__group(configurations)
        self.__max_ms, self.__max_ml = self.__find_max()
        self.__terms = self.__calculate_terms()

        configurations.positive_ms_ml = False

    @staticmethod
    def __group(configurations: Configurations) -> dict[tuple[float, int], int]:
        """
        Group the configurations by ms and ml.

        Args:
            configurations (Configurations):
                The configurations.

        Returns:
            dict[tuple[float, int], int]:
                The grouped configurations.
        """
        return Counter((conf.ms, conf.ml) for conf in configurations if conf.ml >= 0 and conf.ms >= 0)

    def __find_max(self) -> tuple[float, int]:
        """
        Find the maximum ms and ml values.

        Returns:
            tuple[float, int]:
                The maximum ms and ml values.
        """
        max_ms, max_ml = 0, 0

        # find the maximum ms and ml values
        for ms, ml in self.__groups.keys():
            if ms > max_ms:
                max_ms = ms
            if ml > max_ml:
                max_ml = ml
        # The buildin `max` function cannot be used here because it would return the maximum value of the biggest ms
        # and ml values in the same configuration state and not the maximum ms and ml values of the whole electron
        # configuration. Calling the `max` function twice is not effective because that would mean that the
        # `Configurations` object must be iterated twice.

        return max_ms, max_ml

    @classmethod
    def from_spdf(cls, s: int, p: int, d: int, f: int) -> "Groups":
        return cls(Configurations(s, p, d, f))

    @property
    def groups(self) -> dict:
        return self.__groups

    def __calculate_terms(self) -> list[Term]:
        """
        Calculate the terms of the groups.

        Returns:
            list[Term]:
                The terms of the groups.
        """
        # copy the groups to not modify the original groups
        group_cpy = self.__groups.copy()

        terms = []

        # the possible ms values
        ms_values = list(float_range(self.__max_ms + 1))

        # iterate through the ms and ml values in reverse order
        for ms in ms_values[::-1]:
            for ml in range(self.__max_ml, -1, -1):
                if (count := group_cpy[(ms, ml)]) > 0:
                    terms.append(Term(ms, ml, count))
                    # decreasing the counts of the other terms of this group
                    for ms_ in float_range(ms + 1):
                        for ml_ in range(ml + 1):
                            group_cpy[(ms_, ml_)] -= count

        return terms

    def to_matrix(self) -> list[list[int]]:
        """
        Convert the groups to a matrix.

        Returns:
            list[list[int]]:
                The groups as a matrix.
        """
        return [
            [self.__groups[(ms, ml)] if (ms, ml) in self.__groups else 0
             for ml in range(self.__max_ml + 1)]
            for ms in float_range(self.__max_ms + 1)
        ]

    @property
    def terms(self) -> list[Term]:
        """
        Get the terms of the groups.

        Returns:
            list[Term]:
                The terms of the groups.
        """
        return self.__terms

    def __repr__(self) -> str:
        return f"<Groups {self.__groups=}>"
