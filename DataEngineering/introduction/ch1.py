# Import required libraries
import pandas as pd
from sqlalchemy import create_engine

# Assume db_engine is already created
# If not, you would create it like this:
# db_engine = create_engine('your_database_connection_string_here')

# Execute SQL query and load results into a DataFrame
data = pd.read_sql("""
SELECT first_name, last_name FROM "Customer"
ORDER BY last_name, first_name
""", db_engine)

# Show the first 3 rows of the DataFrame
print(data.head(3))

# Show the info of the DataFrame
print(data.info())


# Execute SQL query and load results into a DataFrame
data = pd.read_sql("""
SELECT * FROM "Customer"
INNER JOIN "Order"
ON "Order"."customer_id"="Customer"."id"
""", db_engine)

# Show the id column of data
print(data.id)