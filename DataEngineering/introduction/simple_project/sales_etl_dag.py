from airflow import DAG
from airflow.operators.python_operator import PythonOperator
from airflow.providers.apache.spark.operators.spark_submit import SparkSubmitOperator
from airflow.providers.postgres.operators.postgres import PostgresOperator
from datetime import datetime, timedelta
import random

default_args = {
    'owner': 'airflow',
    'depends_on_past': False,
    'start_date': datetime(2023, 1, 1),
    'email_on_failure': False,
    'email_on_retry': False,
    'retries': 1,
    'retry_delay': timedelta(minutes=5),
}

dag = DAG(
    'sales_etl',
    default_args=default_args,
    description='A simple sales ETL pipeline',
    schedule_interval=timedelta(days=1),
)

create_table = PostgresOperator(
    task_id='create_table',
    postgres_conn_id='postgres_default',
    sql="""
    CREATE TABLE IF NOT EXISTS daily_sales (
        transaction_id SERIAL PRIMARY KEY,
        date DATE,
        amount DECIMAL(10, 2)
    )
    """,
    dag=dag,
)

def insert_sample_data(**kwargs):
    from airflow.providers.postgres.hooks.postgres import PostgresHook
    
    pg_hook = PostgresHook(postgres_conn_id='postgres_default')
    conn = pg_hook.get_conn()
    cursor = conn.cursor()
    
    # Generate sample data for the current date
    current_date = kwargs['execution_date'].strftime('%Y-%m-%d')
    for _ in range(100):  # Insert 100 sample transactions
        amount = round(random.uniform(10, 1000), 2)
        cursor.execute(
            "INSERT INTO daily_sales (date, amount) VALUES (%s, %s)",
            (current_date, amount)
        )
    
    conn.commit()
    cursor.close()
    conn.close()

insert_data = PythonOperator(
    task_id='insert_sample_data',
    python_callable=insert_sample_data,
    provide_context=True,
    dag=dag,
)

process_sales = SparkSubmitOperator(
    task_id='process_sales',
    application='/path/to/process_sales.py',
    conn_id='spark_default',
    dag=dag,
)

create_table >> insert_data >> process_sales