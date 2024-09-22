"""
This module contains the Groups class, which is used to calculate the terms of a given configuration.
"""

from functools import lru_cache

from .configuration import Configurations
from .utils import float_range, float_range_equal


@lru_cache(maxsize=100)
def get_groups_and_configurations(s: int, p: int, d: int, f: int) -> tuple['Groups', 'Configurations']:
    """
    Get the groups and configurations of the given configuration.

    Args:
        s (int):
            The s value of the configuration.

        p (int):
            The p value of the configuration.

        d (int):
            The d value of the configuration.

        f (int):
            The f value of the configuration.

    Returns:
        tuple[Groups, Configurations]:
            The groups and configurations of the given configuration.
    """
    configurations = Configurations(s, p, d, f)
    groups = Groups(configurations)

    return groups, configurations


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
        self.count = count
        self.s = s
        self.l = l

        self.term_letter, self.sup, self.sub = Term.__build_term(s, l)

    @staticmethod
    def __build_term(s: float, l: int) -> tuple[str, int, list[float]]:
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

        term_letter = ml_matcher[l]

        sup = int(2 * s) + 1
        sub = [l_ for l_ in float_range_equal(l + s, abs(l - s))]

        return term_letter, sup, sub

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
        self.__configurations = configurations
        self.__max_ms, self.__max_ml = configurations.max_ms(), configurations.max_ml()
        self.__terms = self.__calculate_terms()

    @classmethod
    def from_spdf(cls, s: int, p: int, d: int, f: int) -> "Groups":
        return cls(Configurations(s, p, d, f))

    def __calculate_terms(self) -> list[Term]:
        """
        Calculate the terms of the groups.

        Returns:
            list[Term]:
                The terms of the groups.
        """
        terms = []

        # the possible ms values
        ms_values = list(float_range(self.__max_ms + 1))

        groups = self.__configurations.count_ml_ms()
        # iterate through the ms and ml values in reverse order
        for ms in ms_values[::-1]:
            for ml in range(self.__max_ml, -1, -1):
                if (count := groups[ml][int(ms)]) > 0:
                    terms.append(Term(ms, ml, count))
                    # decreasing the counts of the other terms of this group
                    for ms_ in float_range(ms + 1):
                        for ml_ in range(ml + 1):
                            groups[ml_][int(ms_)] -= count

        return terms

    @property
    def terms(self) -> list[Term]:
        """
        Get the terms of the groups.

        Returns:
            list[Term]:
                The terms of the groups.
        """
        return self.__terms
