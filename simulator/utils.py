def float_range(stop: int | float, start: int | float = None, step: int | float = 1):
    if start is None:
        start = stop - int(stop)
    while start < stop:
        yield start
        start += step


def float_range_equal(stop: int | float, start: int | float = None, step: int | float = 1):
    if start is None:
        start = stop - int(stop)
    while start <= stop:
        yield start
        start += step
