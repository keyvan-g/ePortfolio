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


module.exports = {
    travel
};

