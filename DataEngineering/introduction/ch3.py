#fetch from external api
import requests

# Fetch the Hackernews post
resp = requests.get("https://hacker-news.firebaseio.com/v0/item/16222426.json")

# Print the response parsed as JSON
print(resp.json())

# Assign the score of the test to post_score
post_score = resp.json()["score"]
print(post_score)

#{'by': 'neis', 'descendants': 0, 'id': 16222426, 'score': 17, 'time': 1516800333, 'title': 'Duolingo-Style Learning for Data Science: DataCamp for Mobile', 'type': 'story', 'url': 'https://medium.com/datacamp/duolingo-style-learning-for-data-science-datacamp-for-mobile-3861d1bc02df'}
#17

# Function to extract table to a pandas DataFrame
def extract_table_to_pandas(tablename, db_engine):
    query = "SELECT * FROM {tablename}".format(tablename=tablename)
    return pd.read_sql(query, db_engine)

# Connect to the database using the connection URI
connection_uri = "postgresql://repl:password@localhost:5432/pagila" 
db_engine = sqlalchemy.create_engine(connection_uri)

# Extract the film table into a pandas DataFrame
extract_table_to_pandas("film", db_engine)

# Extract the customer table into a pandas DataFrame
extract_table_to_pandas("customer", db_engine)
'''

     customer_id  store_id first_name  last_name                                  email  address_id  activebool create_date               last_update  active
0              1         1       MARY      SMITH          MARY.SMITH@sakilacustomer.org           5        True  2017-02-14 2017-02-15 09:57:20+00:00       1
1              2         1   PATRICIA    JOHNSON    PATRICIA.JOHNSON@sakilacustomer.org           6        True  2017-02-14 2017-02-15 09:57:20+00:00       1
2              3         1      LINDA   WILLIAMS      LINDA.WILLIAMS@sakilacustomer.org           7        True  2017-02-14 2017-02-15 09:57:20+00:00       1
3              4         2    BARBARA      JONES       BARBARA.JONES@sakilacustomer.org           8        True  2017-02-14 2017-02-15 09:57:20+00:00       1
4              5         1  ELIZABETH      BROWN     ELIZABETH.BROWN@sakilacustomer.org           9        True  2017-02-14 2017-02-15 09:57:20+00:00       1
..           ...       ...        ...        ...                                    ...         ...         ...         ...                       ...     ...
594          595         1   TERRENCE  GUNDERSON  TERRENCE.GUNDERSON@sakilacustomer.org         601        True  2017-02-14 2017-02-15 09:57:20+00:00       1
595          596         1    ENRIQUE   FORSYTHE    ENRIQUE.FORSYTHE@sakilacustomer.org         602        True  2017-02-14 2017-02-15 09:57:20+00:00       1
596          597         1    FREDDIE     DUGGAN      FREDDIE.DUGGAN@sakilacustomer.org         603        True  2017-02-14 2017-02-15 09:57:20+00:00       1
597          598         1       WADE   DELVALLE       WADE.DELVALLE@sakilacustomer.org         604        True  2017-02-14 2017-02-15 09:57:20+00:00       1
598          599         2     AUSTIN    CINTRON      AUSTIN.CINTRON@sakilacustomer.org         605        True  2017-02-14 2017-02-15 09:57:20+00:00       1

[599 rows x 10 columns]
'''

#another part

# Get the rental rate column as a string
rental_rate_str = film_df.rental_rate.astype("str")

# Split up and expand the column
rental_rate_expanded = rental_rate_str.str.split(".", expand=True)

# Assign the columns to film_df
film_df = film_df.assign(
    rental_rate_dollar=rental_rate_expanded[0],
    rental_rate_cents=rental_rate_expanded[1],
)

#another part

# Use groupBy and mean to aggregate the column
ratings_per_film_df = rating_df.groupBy('film_id').mean('rating')

# Join the tables using the film_id column
film_df_with_ratings = film_df.join(
    ratings_per_film_df,
    film_df.film_id==ratings_per_film_df.film_id
)

# Show the 5 first results
print(film_df_with_ratings.show(5))
'''
+-------+----------------+--------------------+------------+-----------+--------------------+---------------+-----------+------+----------------+------+--------------------+--------------------+--------------------+-------+------------------+
|film_id|           title|         description|release_year|language_id|original_language_id|rental_duration|rental_rate|length|replacement_cost|rating|         last_update|    special_features|            fulltext|film_id|       avg(rating)|
+-------+----------------+--------------------+------------+-----------+--------------------+---------------+-----------+------+----------------+------+--------------------+--------------------+--------------------+-------+------------------+
|      1|ACADEMY DINOSAUR|A Epic Drama of a...|        2006|          1|                null|              6|       0.99|    86|           20.99|    PG|2017-09-10 17:46:...|[Deleted Scenes, ...|'academi':1 'batt...|      1| 4.646341463414634|
|      2|  ACE GOLDFINGER|A Astounding Epis...|        2006|          1|                null|              3|       4.99|    48|           12.99|     G|2017-09-10 17:46:...|[Trailers, Delete...|'ace':1 'administ...|      2|4.7555555555555555|
|      3|ADAPTATION HOLES|A Astounding Refl...|        2006|          1|                null|              7|       2.99|    50|           18.99| NC-17|2017-09-10 17:46:...|[Trailers, Delete...|'adapt':1 'astoun...|      3| 4.209677419354839|
|      4|AFFAIR PREJUDICE|A Fanciful Docume...|        2006|          1|                null|              5|       2.99|   117|           26.99|     G|2017-09-10 17:46:...|[Commentaries, Be...|'affair':1 'chase...|      4|               4.2|
|      5|     AFRICAN EGG|A Fast-Paced Docu...|        2006|          1|                null|              6|       2.99|   130|           22.99|     G|2017-09-10 17:46:...|    [Deleted Scenes]|'african':1 'chef...|      5|  4.63448275862069|
+-------+----------------+--------------------+------------+-----------+--------------------+---------------+-----------+------+----------------+------+--------------------+--------------------+--------------------+-------+------------------+
only showing top 5 rows
'''

#another part
# Import necessary libraries
import pandas as pd
from pyspark.sql import SparkSession

# Assume film_pdf (pandas DataFrame) and film_sdf (PySpark DataFrame) already exist
# If not, you would need to create or load them, for example:
# film_pdf = pd.read_csv('films.csv')
# spark = SparkSession.builder.appName("ParquetExample").getOrCreate()
# film_sdf = spark.read.csv('films.csv', header=True, inferSchema=True)

# Write the pandas DataFrame to parquet
film_pdf.to_parquet("films_pdf.parquet")

# Write the PySpark DataFrame to parquet
film_sdf.write.parquet("films_sdf.parquet")

#another part

# Finish the connection URI
connection_uri = "postgresql://repl:password@localhost:5432/dwh"
db_engine_dwh = sqlalchemy.create_engine(connection_uri)

# Transformation step, join with recommendations data
film_pdf_joined = film_pdf.join(recommendations)

# Finish the .to_sql() call to write to store.film
film_pdf_joined.to_sql("film", db_engine_dwh, schema="store", if_exists="replace")

# Run the query to fetch the data
pd.read_sql("SELECT film_id, recommended_film_ids FROM store.film", db_engine_dwh)
'''
     film_id recommended_film_ids
0          1          {691,617,4}
1          2            {413,683}
2          3         {11,744,811}
3          4            {132,793}
4          5                 {11}
..       ...                  ...
995      996        {164,780,937}
996      997         {449,43,130}
997      998                {778}
998      999            {908,570}
999     1000                {704}

[1000 rows x 2 columns]
'''

#all together


# Define the ETL function
def etl():
    film_df = extract_film_to_pandas()
    film_df = transform_rental_rate(film_df)
    load_dataframe_to_film(film_df)

# Define the ETL task using PythonOperator
etl_task = PythonOperator(task_id='etl_film',
                          python_callable=etl,
                          dag=dag)

# Set the upstream to wait_for_table and sample run etl()
etl_task.set_upstream(wait_for_table)
etl()

#Extracting film table...
#Transforming the rental_rate column...
#Loading film DataFrame to table...
