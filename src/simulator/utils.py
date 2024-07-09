"""
This module contains utility functions that are used in the simulator.
"""
from typing import Iterable


def float_range(stop: int | float, start: int | float = None, step: int | float = 1) -> float | int:
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
