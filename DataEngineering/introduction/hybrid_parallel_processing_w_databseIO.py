import pandas as pd
from multiprocessing import Pool
from concurrent.futures import ThreadPoolExecutor
import sqlalchemy

def fetch_and_calculate_mean_age(year):
    # Create a new database connection for each process
    engine = sqlalchemy.create_engine('postgresql://user:password@localhost:5432/database')
    
    def fetch_data_for_year(year):
        query = f"SELECT Age FROM athletes WHERE Year = {year}"
        return pd.read_sql(query, engine)
    
    with ThreadPoolExecutor(max_workers=5) as executor:
        data = executor.submit(fetch_data_for_year, year).result()
    
    mean_age = data['Age'].mean()
    engine.dispose()  # Close the database connection
    return pd.DataFrame({"Age": mean_age}, index=[year])

if __name__ == '__main__':
    years = range(1896, 2021, 4)  # Olympic years from 1896 to 2020
    
    with Pool(4) as p:
        results = p.map(fetch_and_calculate_mean_age, years)
    
    result_df = pd.concat(results)
    print(result_df)
