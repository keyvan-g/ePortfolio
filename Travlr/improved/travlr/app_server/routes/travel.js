var express = require('express');
var travelController = require('../controllers/travel');

var router = express.Router();

// GET Travel page
router.get('/', travelController.travel);

// GET Advanced search page
router.get('/search', travelController.searchPage);

// GET Search results
router.get('/search/results', travelController.searchResults);

module.exports = router;