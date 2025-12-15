const mongoose = require('mongoose');
const Trip = require('../models/travlr'); // Register model
const Model = mongoose.model('Trip'); // Retrieve model

// GET: /trips - lists all the trips
// Regardless of outcome, response must include HTML status code
// and JSON message to the requesting client
const tripsList = async (req, res) => {
    const q = await Model
        .find({}) // No filter, return all records
        .exec();

    // Uncomment the following line to show results of query
    // on the console
    // console.log(q);

    if (!q) {
        // Database returned no data
        return res
            .status(404)
            .json(err);
    } else { // Return resulting trip list
        return res
            .status(200)
            .json(q);
    }
};

const tripsFindByCode = async (req, res) => {
    try {
        // Debug logging
        //console.log('Searching for trip with code:', req.params.tripCode);

        // First, let's see all trips to debug
        const allTrips = await Model.find({}).exec();
        //console.log('All trips in database:', allTrips.map(trip => ({ code: trip.code, name: trip.name })));

        const q = await Model
            .findOne({ 'code': req.params.tripCode }) // Use findOne for single result
            .exec();

        //console.log('Query result:', q);

        if (!q) {
            // Database returned no data
            return res
                .status(404)
                .json({ message: 'Trip not found', searchedCode: req.params.tripCode });
        } else { // Return resulting trip
            return res
                .status(200)
                .json(q);
        }
    } catch (err) {
        console.error('Error in tripsFindByCode:', err);
        return res
            .status(500)
            .json({ message: 'Error finding trip', error: err.message });
    }
};

// POST: /trips - Adds a new Trip
// Regardless of outcome, response must include HTML status code
// and JSON message to the requesting client
const tripsAddTrip = async (req, res) => {
    const newTrip = new Trip({
        code: req.body.code,
        name: req.body.name,
        length: req.body.length,
        start: req.body.start,
        resort: req.body.resort,
        perPerson: req.body.perPerson,
        image: req.body.image,
        description: req.body.description
    });

    const q = await newTrip.save();

    if (!q) { // Database returned no data
        return res
            .status(400)
            .json(err);
    } else { // Return new trip
        return res
            .status(201)
            .json(q);
    }

    // Uncomment the following line to show results of operation
    // on the console
    // console.log(q);
};

// PUT: /trips/:tripCode - Adds a new Trip
// Regardless of outcome, response must include HTML status code
// and JSON message to the requesting client
const tripsUpdateTrip = async (req, res) => {
    try {
        // Uncomment for debugging
        console.log('PUT request params:', req.params);
        console.log('PUT request body:', req.body);
        
        const q = await Model
            .findOneAndUpdate(
                { 'code': req.params.tripCode },
                {
                    code: req.body.code,
                    name: req.body.name,
                    length: req.body.length,
                    start: req.body.start,
                    resort: req.body.resort,
                    perPerson: req.body.perPerson,
                    image: req.body.image,
                    description: req.body.description
                },
                { new: true } // Return the updated document
            )
            .exec();
            
        if (!q) { // Database returned no data
            return res
                .status(404)
                .json({ message: 'Trip not found', searchedCode: req.params.tripCode });
        } else { // Return resulting updated trip
            return res
                .status(200)
                .json(q);
        }
    } catch (err) {
        console.error('Error updating trip:', err);
        return res
            .status(500)
            .json({ message: 'Error updating trip', error: err.message });
    }
};

// GET: /trips/search - Advanced search with multiple criteria
const tripsAdvancedSearch = async (req, res) => {
    try {
        console.log('Advanced search query params:', req.query);
        
        // Build MongoDB query object based on search parameters
        let searchQuery = {};
        
        // Price range search (convert perPerson string to number for comparison)
        if (req.query.minPrice || req.query.maxPrice) {
            const minPrice = req.query.minPrice ? parseFloat(req.query.minPrice) : 0;
            const maxPrice = req.query.maxPrice ? parseFloat(req.query.maxPrice) : Number.MAX_VALUE;
            
            // Get all trips first, then filter by price in JavaScript
            const allTrips = await Model.find({}).exec();
            const priceFilteredTrips = allTrips.filter(trip => {
                // Clean the price string and convert to number
                const cleanPrice = trip.perPerson.replace(/\$|,/g, '');
                const numericPrice = parseFloat(cleanPrice) || 0;
                return numericPrice >= minPrice && numericPrice <= maxPrice;
            });
            
            if (priceFilteredTrips.length > 0) {
                const priceFilteredCodes = priceFilteredTrips.map(trip => trip.code);
                searchQuery.code = { $in: priceFilteredCodes };
            } else {
                // No trips match price criteria
                return res.status(200).json([]);
            }
        }
        
        // Description search (case-insensitive partial match)
        if (req.query.description) {
            searchQuery.description = { 
                $regex: req.query.description, 
                $options: 'i' 
            };
        }
        
        // Duration/Length search
        if (req.query.duration) {
            // Support both exact match and partial match for duration
            searchQuery.length = { 
                $regex: req.query.duration, 
                $options: 'i' 
            };
        }
        
        // Name search (case-insensitive partial match)
        if (req.query.name) {
            searchQuery.name = { 
                $regex: req.query.name, 
                $options: 'i' 
            };
        }
        
        // Resort search (case-insensitive partial match)
        if (req.query.resort) {
            searchQuery.resort = { 
                $regex: req.query.resort, 
                $options: 'i' 
            };
        }
        
        // Date range search
        if (req.query.startDate || req.query.endDate) {
            searchQuery.start = {};
            if (req.query.startDate) {
                searchQuery.start.$gte = new Date(req.query.startDate);
            }
            if (req.query.endDate) {
                searchQuery.start.$lte = new Date(req.query.endDate);
            }
        }
        
        console.log('MongoDB search query:', JSON.stringify(searchQuery, null, 2));
        
        // Execute the search query
        const results = await Model
            .find(searchQuery)
            .sort({ start: 1, name: 1 }) // Sort by start date, then name
            .exec();
        
        console.log(`Found ${results.length} trips matching search criteria`);
        
        return res.status(200).json(results);
        
    } catch (err) {
        console.error('Error in advanced search:', err);
        return res
            .status(500)
            .json({ message: 'Error performing search', error: err.message });
    }
};

module.exports = {
    tripsList,
    tripsFindByCode,
    tripsAddTrip,
    tripsUpdateTrip,
    tripsAdvancedSearch
};