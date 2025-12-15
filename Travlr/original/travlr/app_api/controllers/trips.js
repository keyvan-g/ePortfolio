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

module.exports = {
    tripsList,
    tripsFindByCode,
    tripsAddTrip,
    tripsUpdateTrip
};