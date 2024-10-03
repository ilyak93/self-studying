import pandas as pd
from multiprocessing import Pool
import time
from functools import wraps

# Decorator for timing function execution
def print_timing(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        end = time.time()
        print(f'{func.__name__} took {end - start:.2f} seconds')
        return result
    return wrapper

# Function to apply a function over multiple cores
@print_timing
def parallel_apply(apply_func, groups, nb_cores):
    with Pool(nb_cores) as p:
        results = p.map(apply_func, groups)
    return pd.concat(results)

# Assume we have a DataFrame called athlete_events
# If not, you would need to load it, e.g.:
# athlete_events = pd.read_csv('athlete_events.csv')

# Function to calculate mean age (to be applied to each group)
def take_mean_age(group):
    return pd.DataFrame({'mean_age': group['Age'].mean()}, index=[group.name])

# Parallel apply using 1 core
parallel_apply(take_mean_age, athlete_events.groupby('Year'), 1)

# Parallel apply using 2 cores
parallel_apply(take_mean_age, athlete_events.groupby('Year'), 2)

# Parallel apply using 4 cores
parallel_apply(take_mean_age, athlete_events.groupby('Year'), 4)


#part 2

import dask.dataframe as dd

# Set the number of partitions
athlete_events_dask = dd.from_pandas(athlete_events, npartitions=4)