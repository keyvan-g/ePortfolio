//var fs = require('fs');
//var trips = JSON.parse(fs.readFileSync('./data/trips.json', 'utf8'));

const fetch = require('node-fetch');

const tripsEndpoint = 'http://localhost:3000/api/trips';
const options = {
    method: 'GET',
    headers: { 'Content-Type': 'application/json' }
};

const travel = async (req, res, next) => {

    await fetch(tripsEndpoint, options)
        .then((res) => res.json())
        .then((json) => {
            let message = null;
            if (! json instanceof Array) {
                message = "API lookup error";
                json = [];
            } else if (json.length === 0) {
                message = "No trips found";
            }

            res.render('travel', {
                title: 'Travlr Getaways', trips: json, message
            });
        })
        .catch((error) => res.status(500).send(error.message));
};

// Render the advanced search page
const searchPage = (req, res, next) => {
    res.render('search', {
        title: 'Advanced Trip Search - Travlr Getaways'
    });
};

// Handle advanced search results
const searchResults = async (req, res, next) => {
    // Build query string from search parameters
    const searchParams = new URLSearchParams();
    
    if (req.query.minPrice) searchParams.append('minPrice', req.query.minPrice);
    if (req.query.maxPrice) searchParams.append('maxPrice', req.query.maxPrice);
    if (req.query.description) searchParams.append('description', req.query.description);
    if (req.query.duration) searchParams.append('duration', req.query.duration);
    if (req.query.name) searchParams.append('name', req.query.name);
    if (req.query.resort) searchParams.append('resort', req.query.resort);
    if (req.query.startDate) searchParams.append('startDate', req.query.startDate);
    if (req.query.endDate) searchParams.append('endDate', req.query.endDate);
    
    const searchEndpoint = `http://localhost:3000/api/trips/search?${searchParams.toString()}`;
    
    console.log('Searching with endpoint:', searchEndpoint);
    
    await fetch(searchEndpoint, options)
        .then((res) => res.json())
        .then((json) => {
            let message = null;
            if (! json instanceof Array) {
                message = "Search API error";
                json = [];
            } else if (json.length === 0) {
                message = "No trips found matching your search criteria";
            } else {
                message = `Found ${json.length} trip(s) matching your search`;
            }

            res.render('search-results', {
                title: 'Search Results - Travlr Getaways', 
                trips: json, 
                message,
                searchQuery: req.query
            });
        })
        .catch((error) => {
            console.error('Search error:', error);
            res.render('search-results', {
                title: 'Search Results - Travlr Getaways',
                trips: [],
                message: 'Error performing search: ' + error.message,
                searchQuery: req.query
            });
        });
};

module.exports = {
    travel,
    searchPage,
    searchResults
};

