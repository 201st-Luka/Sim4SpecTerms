from .configuration import Configurations
from .utils import float_range, float_range_equal


class Term:
    def __init__(self, ms: float, ml: int, count: int = 1):
        self.__term_letter: str
        self.__sup: int
        self.__sub: list[float]

        self.__count = count
        self.__ms = ms
        self.__ml = ml

        self.__build_term(ms, ml)

    def __build_term(self, ms: float, ml: int):
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

        self.__term_letter = ml_matcher[ml]

        self.__sup = int(2 * ms) + 1
        self.__sub = [ml_ for ml_ in float_range_equal(ml + ms, abs(ml - ms))]

    @property
    def term(self) -> str:
        return self.__term_letter

    @property
    def sup(self) -> int:
        return self.__sup

    @property
    def sub(self) -> list[float]:
        return self.__sub

    @property
    def count(self) -> int:
        return self.__count

    @property
    def ms(self) -> float:
        return self.__ms

    @property
    def ml(self) -> int:
        return self.__ml

    def __repr__(self) -> str:
        return f"<Term {self.sup=}, {self.term=}, {self.sub=}>"

    def to_list(self) -> list:
        return [self.__ml, self.__ms, self.__count, self.__sup, self.__term_letter, self.__sub]


class Groups:
    def __init__(self, configurations: Configurations):
        self.__max_ms, self.__max_ml = self.__find_max(configurations)

        self.__groups = self.__group(configurations)
        self.__reduced_groups: dict | None = None

        self.__terms = self.__calculate_terms()

    def __group(self, configurations: Configurations) -> dict[tuple[float, int], int]:
        groups = {(ms, ml): 0
                  for ms in float_range(self.__max_ms + 1)
                  for ml in range(self.__max_ml + 1)
                  }

        for configuration in configurations:
            if configuration.ml >= 0 and configuration.ms >= 0:
                groups[(configuration.ms, configuration.ml)] += 1

        return groups

    @staticmethod
    def __find_max(configurations: Configurations) -> tuple[float, int]:
        max_ms, max_ml = 0, 0

        for configuration in configurations:
            if configuration.ms > max_ms:
                max_ms = configuration.ms
            if configuration.ml > max_ml:
                max_ml = configuration.ml

        return max_ms, max_ml

    @classmethod
    def from_spdf(cls, s: int, p: int, d: int, f: int) -> "Groups":
        return cls(Configurations(s, p, d, f))

    @property
    def groups(self) -> dict:
        return self.__groups

    def __calculate_terms(self) -> list[Term]:
        group_cpy = self.__groups.copy()

        terms = []

        ms_values = list(float_range(self.__max_ms + 1))

        for ms in ms_values[::-1]:
            for ml in range(self.__max_ml, -1, -1):
                if (count := group_cpy[(ms, ml)]) > 0:
                    terms.append(Term(ms, ml, count))
                    for ms_ in float_range(ms + 1):
                        for ml_ in range(ml + 1):
                            group_cpy[(ms_, ml_)] -= count

        return terms

    def to_matrix(self) -> list[list[int]]:
        return [
            [self.__groups[(ms, ml)] for ml in range(self.__max_ml + 1)]
            for ms in float_range(self.__max_ms + 1)
        ]

    @property
    def reduced_groups(self):
        if self.__reduced_groups is None:
            self.__reduced_groups = {key: value for key, value in self.__groups.items() if value > 0}
        return self.__reduced_groups

    @property
    def terms(self):
        return self.__terms

    def __repr__(self) -> str:
        return f"<Groups {self.__groups=}>"

