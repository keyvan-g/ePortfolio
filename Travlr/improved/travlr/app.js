
var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./app_server/routes/index');
var usersRouter = require('./app_server/routes/users');
var travelRouter = require('./app_server/routes/travel');
var mealsRouter = require('./app_server/routes/meals');
var apiRouter = require('./app_api/routes/index');
var handlebars = require('hbs');

require('dotenv').config();
require('./app_api/models/db');

var app = express();

// Wire in our authentication module
var passport = require('passport');
require('./app_api/config/passport');

// view engine setup
app.set('views', path.join(__dirname, '/app_server/views'));
handlebars.registerPartials(__dirname + '/app_server/views/partials');

// Register Handlebars helpers
handlebars.registerHelper('formatDate', function(date) {
  if (!date) return 'TBD';
  try {
    const d = new Date(date);
    if (isNaN(d.getTime())) return 'TBD'; // Invalid date
    return d.toLocaleDateString('en-US', { 
      year: 'numeric', 
      month: 'long', 
      day: 'numeric' 
    });
  } catch (err) {
    return 'TBD'; // Fallback for any errors
  }
});

app.set('view engine', 'hbs');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use(passport.initialize());

// Catch unauthorized error and create 401
app.use((err, req, res, next) => {
  if (err.name === 'UnauthorizedError') {
    res
      .status(401)
      .json({ "message": err.name + ": " + err.message });
  }
});

// Debug middleware for API requests
app.use('/api', function (req, res, next) {
  console.log(`API Request: ${req.method} ${req.url}`);
  next();
});

// Enable CORS for API requests
app.use('/api', function (req, res, next) {
  res.header("Access-Control-Allow-Origin", "http://localhost:4200");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, Authorization");
  res.header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  res.header("Access-Control-Allow-Credentials", "true");

  // Handle preflight requests
  if (req.method === 'OPTIONS') {
    res.sendStatus(200);
  } else {
    next();
  }
});

app.use('/', indexRouter);
app.use('/users', usersRouter);
app.use('/travel', travelRouter);
app.use('/meals', mealsRouter);
app.use('/api', apiRouter);

// catch 404 and forward to error handler
app.use(function (req, res, next) {
  console.log(req.url);
  next(createError(404));
});

// error handler
app.use(function (err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = app;
