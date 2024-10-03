#parallel computing

# Print the type of athlete_events_spark
print(type(athlete_events_spark))

# Print the schema of athlete_events_spark
print(athlete_events_spark.printSchema())

# Group by the Year, and find the mean Age
print(athlete_events_spark.groupBy('Year').mean('Age'))

# Group by the Year, and find the mean Age
print(athlete_events_spark.groupBy('Year').mean('Age').show())

'''
<class 'pyspark.sql.dataframe.DataFrame'>
root
 |-- ID: integer (nullable = true)
 |-- Name: string (nullable = true)
 |-- Sex: string (nullable = true)
 |-- Age: integer (nullable = true)
 |-- Height: string (nullable = true)
 |-- Weight: string (nullable = true)
 |-- Team: string (nullable = true)
 |-- NOC: string (nullable = true)
 |-- Games: string (nullable = true)
 |-- Year: integer (nullable = true)
 |-- Season: string (nullable = true)
 |-- City: string (nullable = true)
 |-- Sport: string (nullable = true)
 |-- Event: string (nullable = true)
 |-- Medal: string (nullable = true)

None
DataFrame[Year: int, avg(Age): double]
+----+------------------+
|Year|          avg(Age)|
+----+------------------+
|1896|23.580645161290324|
|1924|28.373324544056253|
|2006|25.959151072569604|
|1908|26.970228384991845|
|1952|26.161546085232903|
|1956|25.926673567977915|
|1988|24.079431552931485|
|1994|24.422102596580114|
|1968|24.248045555448314|
|2014|25.987323655694134|
|1904| 26.69814995131451|
|2004|25.639514989213716|
|1932| 32.58207957204948|
|1996|24.915045018878885|
|1998|25.163197335553704|
|1960|25.168848457954294|
|2012| 25.96137770897833|
|1912| 27.53861997940268|
|2016| 26.20791934541204|
|1936|27.530328324986087|
+----+------------------+
only showing top 20 rows
'''

#airflow

# Import necessary modules
from airflow import DAG
from airflow.utils.dates import days_ago

# Create the DAG object
dag = DAG(
    dag_id="car_factory_simulation",
    default_args={
        "owner": "airflow",
        "start_date": days_ago(2)
    },
    schedule_interval="0 * * * *"
)

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