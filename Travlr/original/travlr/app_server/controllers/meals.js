const fs = require('fs');
const path = require('path');

var meallist = JSON.parse(fs.readFileSync('./data/meals.json', 'utf8'));

// GET meals list

const meals = (req, res) => {
    res.render('meals', {
        title: 'Meals',
        meals: meallist
    });
};

module.exports = {
    meals
};

