# Setup the Jupyter version of Dash
#from jupyter_dash import JupyterDash
from dash import Dash

# Configure the necessary Python module imports for dashboard components
import dash_leaflet as dl
from dash import dcc
from dash import html
import plotly.express as px
from dash import dash_table
from dash.dependencies import Input, Output, State
import base64

# Configure OS routines
import os

# Configure the plotting routines
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from aaclib import AnimalShelter

###########################
# Data Manipulation / Model
###########################
# read database credentials from OS
username = os.environ.get("AACUSER")
password = os.environ.get("AACPASS") 
# Connect to database via CRUD Module
print(f'connecting to db {username}/{password}')
db = AnimalShelter(username, password)

# class read method must support return of list object and accept projection json input
# sending the read method an empty document requests all documents be returned
df = pd.DataFrame.from_records(db.read({}))

# Generic filters for animal_type and breed options
animal_type_options = [{'label': i, 'value': i} for i in df['animal_type'].unique()]
breed_options = [{'label': i, 'value': i} for i in df['breed'].unique()]

# Add an 'All' option to the top
animal_type_options.insert(0, {'label': 'All Types', 'value': 'All'})
breed_options.insert(0, {'label': 'All Breeds', 'value': 'All'})

# MongoDB v5+ is going to return the '_id' column and that is going to have an 
# invlaid object type of 'ObjectID' - which will cause the data_table to crash - so we remove
# it in the dataframe here. The df.drop command allows us to drop the column. If we do not set
# inplace=True - it will reeturn a new dataframe that does not contain the dropped column(s)
df.drop(columns=['_id'],inplace=True)

## Debug
# print(len(df.to_dict(orient='records')))
# print(df.columns)


#########################
# Dashboard Layout / View
#########################
app = Dash('Grazios')
# For WSGI compatibility
server =app.server

#Add in Grazioso Salvare’s logo
image_filename = 'Grazioso Salvare Logo.png' # replace with your own image
encoded_image = base64.b64encode(open(image_filename, 'rb').read())

app.layout = html.Div([
    html.Img(src='data:image/png;base64,{}'.format(encoded_image.decode()), 
             style={'width' : 100, 'height' : 100}),
#    html.Div(id='hidden-div', style={'display':'none'}),
    html.Center(html.B(html.H1('CS-340 Dashboard'))),
    html.Center(html.B(html.H3("Keyvan's Unique Dash"))),
    #html.Hr(),
    html.Div([
        dcc.RadioItems([ { 'label': 'Water Rescue', 'value' : 1},
                           {'label': 'Mountain or Wilderness Rescue', 'value': 2},
                            {'label': 'Disaster Rescue', 'value': 3},
                             {'label' :'Reset', 'value': 0}], 
                        inline=True,
                        id='filter-type')    ,    
    
        # NEW Generic FILTERS
        html.Div([
            html.Label('Filter by Animal Type:'),
            dcc.Dropdown(
                id='animal-type-dropdown',
                options=animal_type_options,
                value='All',
                clearable=False
            ),
        ], style={'width': '48%', 'display': 'inline-block', 'padding': '5px'}),

        html.Div([
            html.Label('Filter by Breed:'),
            dcc.Dropdown(
                id='breed-dropdown',
                options=breed_options,
                value='All',
                clearable=False
            ),
        ], style={'width': '48%', 'display': 'inline-block', 'padding': '5px'})

    ]),
    
    # Rest of the layout...
    html.Hr(),
    dash_table.DataTable(id='datatable-id',
                         columns=[{"name": i, "id": i, "deletable": False, "selectable": True} for i in df.columns],               
        data=df.to_dict('records'),
        row_selectable='single',
        sort_action='native',
        page_action="native",
        page_current= 0,
        page_size= 10,
        selected_rows=[0]
                  ),
    html.Br(),
    html.Hr(),
    
    #This sets up the dashboard so that your chart and your geolocation chart are side-by-side
    html.Div(className='row',
         style={'display' : 'flex'},
             children=[
        html.Div(
            id='graph-id',
            className='col s12 m6',

            ),
        html.Div(
            id='map-id',
            className='col s12 m6',
            )
        ])
])

#############################################
# Interaction Between Components / Controller
#############################################

@app.callback(
    Output('datatable-id', 'data'),
    [Input('filter-type', 'value'),
     Input('animal-type-dropdown', 'value'),  # NEW INPUT
     Input('breed-dropdown', 'value')]        # NEW INPUT
)
def update_dashboard(filter_type, animal_type, breed):
## filter interactive data table with MongoDB queries       
        columns=[{"name": i, "id": i, "deletable": False, "selectable": True} for i in df.columns]
    
        mongo_filter = {}
        if filter_type == 1:
            mongo_filter = {'breed' : { '$in' : ['Labrador Retriever Mix', 
                                                 'Chesapeake Bay Retriever',
                                                 'Newfoundland']},
                            'sex_upon_outcome' : 'Intact Female' ,
                            'age_upon_outcome_in_weeks' : {'$gte' : 26 , '$lte':156 }
                           }
        elif filter_type == 2:
            mongo_filter = {'breed' : { '$in' : ['German Shepherd', 
                                                 'Alaskan Malamute',
                                                 'Old English Sheepdog',
                                                 'Siberian Husky',
                                                 'Rottweiler']},
                            'sex_upon_outcome' : 'Intact Male' ,
                            'age_upon_outcome_in_weeks' : {'$gte' : 26 , '$lte':156 }
                           }
        elif filter_type == 3:
            mongo_filter = {'breed' : { '$in' : ['Doberman Pinscher', 
                                                 'German Shepherd',
                                                 'Golden Retriever',
                                                 'Bloodhound',
                                                 'Rottweiler']},
                            'sex_upon_outcome' : 'Intact Male' ,
                            'age_upon_outcome_in_weeks' : {'$gte' : 20 , '$lte':300 }
                           }

        # set the additional filters for the MongoDB query based on user selection
        if animal_type != 'All' :
            mongo_filter['animal_type'] = animal_type
        if breed != 'All': 
            mongo_filter['breed'] = breed
                            
        try:
            new_df  = pd.DataFrame.from_records(db.read(mongo_filter))
            new_df.drop(columns=['_id'],inplace=True)

            data=new_df.to_dict('records')
            # Uncomment to debug 
            # print(data)
            return data
        except Exception as e:
            print(e)
            return []
       
    

# Display the breeds of animal based on quantity represented in
# the data table
@app.callback(
    Output('graph-id', "children"),
    [Input('datatable-id', "derived_virtual_data")])
def update_graphs(viewData):
    # add code for chart of your choice (e.g. pie chart) #
    if viewData == None:
        return
    #print(viewData)
    return [
        dcc.Graph(            
            figure = px.pie(viewData, names='breed', title='Preferred Animals')
        )    
    ]
    
    
#This callback will highlight a cell on the data table when the user selects it
@app.callback(
    Output('datatable-id', 'style_data_conditional'),
    [Input('datatable-id', 'selected_rows')]
)
def update_styles(selected_rows):
    if selected_rows != None:
        return [{
            'if': { 'row_index': i },
            'background_color': '#D2F3FF'
        } for i in selected_rows]
    


# This callback will update the geo-location chart for the selected data entry
# derived_virtual_data will be the set of data available from the datatable in the form of 
# a dictionary.
# derived_virtual_selected_rows will be the selected row(s) in the table in the form of
# a list. For this application, we are only permitting single row selection so there is only
# one value in the list.
# The iloc method allows for a row, column notation to pull data from the datatable
@app.callback(
    Output('map-id', "children"),
    [Input('datatable-id', "derived_virtual_data"),
     Input('datatable-id', "derived_virtual_selected_rows")])
def update_map(viewData, index):  
    if viewData is None:
        return
    elif index is None:
        return
    
    dff = pd.DataFrame.from_dict(viewData)
    # Because we only allow single row selection, the list can be converted to a row index here
    if index is None:
        row = 0
    else: 
        row = index[0]
        
    # Austin TX is at [30.75,-97.48]
    return [
        dl.Map(style={'width': '1000px', 'height': '500px'}, center=[30.75,-97.48], zoom=10, children=[
            dl.TileLayer(id="base-layer-id"),
            # Marker with tool tip and popup
            # Column 13 and 14 define the grid-coordinates for the map
            # Column 4 defines the breed for the animal
            # Column 9 defines the name of the animal
            dl.Marker(position=[dff.iloc[row,13],dff.iloc[row,14]], children=[
                dl.Tooltip(dff.iloc[row,4]),
                dl.Popup([
                    html.H1("Animal Name"),
                    html.P(dff.iloc[row,9])
                ])
            ])
        ])
    ]


if __name__ == '__main__':
    app.run(debug=True)