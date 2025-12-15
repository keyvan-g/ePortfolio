import csv
from pymongo import MongoClient
import sys

# --- Configuration ---
# !! Update these values to match your MongoDB setup !!
MONGO_CONNECTION_STRING = "mongodb://localhost:27017/"
DATABASE_NAME = "animalshelter"
COLLECTION_NAME = "animals"

CSV_FILE_PATH = "aac_shelter_outcomes.csv"
BATCH_SIZE = 1000  # Insert documents in batches of 1000
# ---------------------

client = None  # Initialize client to None for the 'finally' block

try:
    # --- 1. Connect to MongoDB ---
    print(f"Connecting to MongoDB at {MONGO_CONNECTION_STRING}...")
    client = MongoClient(MONGO_CONNECTION_STRING)
    
    # Ping the server to confirm a successful connection
    client.admin.command('ping')
    print("MongoDB connection successful.")
    
    db = client[DATABASE_NAME]
    collection = db[COLLECTION_NAME]
    print(f"Using database '{DATABASE_NAME}' and collection '{COLLECTION_NAME}'.")

    # --- 2. Read CSV and Insert Data ---
    documents_to_insert = []
    records_processed = 0
    records_skipped = 0
    
    print(f"Opening CSV file: {CSV_FILE_PATH}...")
    with open(CSV_FILE_PATH, mode='r', encoding='utf-8') as file:
        # Use DictReader to read CSV rows as dictionaries
        reader = csv.DictReader(file)
        
        for row in reader:
            records_processed += 1
            try:
                # --- 3. Build the Document ---
                # Create a document matching the target structure.
                # Handle type conversions and empty strings.
                
                doc = {
                    # Convert the first unnamed column ('') to int
                    'rec_num': int(row['']), 
                    
                    # String fields (no conversion needed)
                    'age_upon_outcome': row['age_upon_outcome'],
                    'animal_id': row['animal_id'],
                    'animal_type': row['animal_type'],
                    'breed': row['breed'],
                    'color': row['color'],
                    'date_of_birth': row['date_of_birth'],
                    'datetime': row['datetime'],
                    'monthyear': row['monthyear'],
                    'name': row['name'],
                    'outcome_subtype': row['outcome_subtype'],
                    'outcome_type': row['outcome_type'],
                    'sex_upon_outcome': row['sex_upon_outcome'],
                    
                    # Numeric fields (convert to float, handle empty strings)
                    'location_lat': float(row['location_lat']) if row['location_lat'] else None,
                    'location_long': float(row['location_long']) if row['location_long'] else None,
                    'age_upon_outcome_in_weeks': float(row['age_upon_outcome_in_weeks']) if row['age_upon_outcome_in_weeks'] else None,
                }
                
                documents_to_insert.append(doc)
                
                # --- 4. Insert in Batches ---
                # When the batch is full, insert it into MongoDB
                if len(documents_to_insert) >= BATCH_SIZE:
                    collection.insert_many(documents_to_insert)
                    print(f"  > Inserted batch of {len(documents_to_insert)} documents (Total processed: {records_processed})")
                    documents_to_insert = []  # Reset the batch

            except ValueError as e:
                # Handle rows with bad data (e.g., "N/A" in a number field)
                records_skipped += 1
                print(f"Skipping row {records_processed} due to data conversion error: {e}. Data: {row}")
            except Exception as e:
                records_skipped += 1
                print(f"An unexpected error occurred at row {records_processed}: {e}. Data: {row}")

        # --- 5. Insert any Remaining Documents ---
        if documents_to_insert:
            collection.insert_many(documents_to_insert)
            print(f"  > Inserted final batch of {len(documents_to_insert)} documents.")

    print("\n--- Import Complete ---")
    print(f"Total rows processed: {records_processed}")
    print(f"Total documents inserted: {records_processed - records_skipped}")
    print(f"Total rows skipped (due to errors): {records_skipped}")

except FileNotFoundError:
    print(f"Error: The file '{CSV_FILE_PATH}' was not found.")
    sys.exit(1)
except ImportError:
    print("Error: The 'pymongo' library is required. Please install it using 'pip install pymongo'")
    sys.exit(1)
except Exception as e:
    # This will catch MongoDB connection errors and other major issues
    print(f"An error occurred: {e}")
    sys.exit(1)
finally:
    # --- 6. Close the Connection ---
    if client:
        client.close()
        print("MongoDB connection closed.")