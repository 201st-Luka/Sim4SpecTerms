"""
This module contains utility functions that are used in the simulator.
"""
from typing import Iterable, Generator


def float_range(
        stop: int | float, start: int | float = None, step: int | float = 1
) -> Generator[float | int, None, None]:
    """
    A generator that generates a range of floating point numbers.

    Args:
        stop (int | float):
            The end of the range.

        start (int | float):
            The start of the range.

        step (int | float):
            The step of the range.

    Yields:
        int | float:
            The next number in the range.
    """
    if start is None:
        start = stop - int(stop)
    while start < stop:
        yield start
        start += step


def float_range_equal(stop: int | float, start: int | float = None, step: int | float = 1) -> Iterable[float | int]:
    """
    A generator that generates a range of floating point numbers that includes the stop value.

    Args:
        stop (int | float):
            The end of the range.

        start (int | float):
            The start of the range.

        step (int | float):
            The step of the range.

    Yields:
        int | float:
            The next number in the range.
    """
    if start is None:
        start = stop - int(stop)
    while start <= stop:
        yield start
        start += step


class InstanceCache:
    __cache = {}
    __cache_max_size = 100

    def __new__(cls, *args, **kwargs):
        # Get the cache for the class
        cache = cls.__cache.setdefault(cls, {})

        if args in cache:
            # Move the configurations to the end of the cache
            configurations = cache.pop(args)
            cache[args] = configurations

        else:
            configurations = super().__new__(cls)
            cache[args] = configurations

            if len(cache) > cls.__cache_max_size:
                # Remove the first element from the cache
                cache.pop(next(iter(cache)))

        return configurations
