from pyspark.sql import SparkSession
from pyspark.sql.functions import sum, count

spark = SparkSession.builder.appName("SalesETL").getOrCreate()

# Read data from PostgreSQL
df = spark.read.jdbc("jdbc:postgresql://localhost:5432/sales_db",
                     "daily_sales",
                     properties={"user":"airflow","password":"airflow"})

# Transform data
daily_summary = df.groupBy("date").agg(
    sum("amount").alias("total_sales"),
    count("transaction_id").alias("total_transactions")
)

# Write summary back to PostgreSQL
daily_summary.write.jdbc("jdbc:postgresql://localhost:5432/sales_db",
                         "daily_sales_summary",
                         mode="overwrite",
                         properties={"user":"airflow","password":"airflow"})

# Also save as Parquet
daily_summary.write.parquet("/path/to/daily_summary.parquet", mode="overwrite")