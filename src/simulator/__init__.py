"""
This module contains the classes that are used to simulate the terms and groups of a given electron configuration.
"""

__all__ = (
    'Configuration',
    'Configurations',
    'Groups',
    'SubShells',
    'simulate',
    'analyse',
)


from functools import lru_cache

from simulator.configuration import Configuration, Configurations
from simulator.groups import Groups
from simulator.subshells import SubShells


_CACHE_SIZE = 1000


@lru_cache(maxsize=_CACHE_SIZE)
def simulate(s: int, p: int, d: int, f: int) -> Configurations:
    return Configurations(s, p, d, f)

@lru_cache(maxsize=_CACHE_SIZE)
def analyse(config: Configurations) -> Groups:
    return Groups(config)



