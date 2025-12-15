var express = require('express');
var travelController = require('../controllers/travel');

var router = express.Router();

// GET Travel page
router.get('/', travelController.travel);

module.exports = router;