#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Last Updated on Sun Jun 22 
Created on Sat May 31 15:24:04 2025

@author: keyvanghareht_snhu
"""
from pymongo import MongoClient
from bson.objectid import ObjectId

class AnimalShelter(object): 
    """ CRUD operations for Animal Collection"""
    
    def __init__(self, USER, PASS):
        self.USER = USER
        self.PASS = PASS
        print("connecting...")
        #USER = 'aacuser'
        #PASS= 'aacuser'
        HOST='localhost'
        PORT= 27017
        DB = 'animalshelter'
        COL = 'animals'
        
        self.client = MongoClient('mongodb://%s:%d' % (HOST,PORT))
        self.database = self.client['%s' % (DB)]
        self.collection = self.database[ '%s' % (COL)]

    def create(self,data):
        try:
            if data is not None:
                self.database.animals.insert_one(data)
                return True
            else:
                print("No data provided.")

        except Exception as e: 
            print(e)
        
        # if reached this point, insert has not been successful
        return False 

    # query the database using specified key-value pairs in data parameter
    def read(self,query):
        try:
            
            cursor = self.collection.find(query)
            results = []
            for document in cursor:
                results.append(document)
                    
            return results
        
        except Exception as e:
            print(e)
            return []
    
    # update all matching rows with key/value pairs in the last argument
    # need to use $ operators such as $set or $inc 
    # returns the count of updated rows
    def update(self, query, updates):
        try:  
            count = self.collection.update_many(query, updates)
            return count.modified_count    
        
        except Exception as e:
            print(e)
            return 0
    
    # delete all matching rows, return the number deleted
    def delete(self, query):
        try:
            count = self.collection.delete_many(query)
            return count.deleted_count
            
        except Exception as e:
            print(e)
            return 0
    
if __name__ == "__main__": 
    client = AnimalShelter('','')
    client.create( {'rec_num': 18,
    'age_upon_outcome': '3 months',
    'animal_id': 'A693288',
    'animal_type': 'Cat',
    'breed': 'Maincoon',
    'color': 'Black ',
    'date_of_birth': '2014-09-28',
    'datetime': '2014-12-09 18:36:00',
    'monthyear': '2014-12-09T18:36:00',
    'name': 'Joey',
    'outcome_subtype': '',
    'outcome_type': 'Adoption',
    'sex_upon_outcome': 'Male',
    'location_lat': 30.4527678292931,
    'location_long': -97.4620507167676,
    'age_upon_outcome_in_weeks': 12})

    print(client.read({'breed': 'Maincoon'}))
    print("%d updated" % client.update({'breed':'Maincoon'}, { '$set': {'rec_num' : 500} } ))
    #print("%d deleted" % client.delete({'breed': 'Maincoon'}))