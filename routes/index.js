var express = require('express');
var router = express.Router();
var yw = require('weather-yahoo');
var ans = {};

/* GET home page. */
router.get('/', function(req, res, next) {
	if(req.url == '/') {
		res.render('index');
	}
	else {
		if(req.url.indexOf("capitalOne") != -1) {
			var n = new Date();
			console.log(n);
  			res.send(n);
  		}
  		else if(req.url.indexOf("dateTime") != -1) {
  			var n = new Date();
			console.log(n);
  			res.send(n);
  		}
  		else if(req.url.indexOf("weather") != -1){
  			yw.getSimpleWeather('west lafayette,in').then(function(res){
    		console.log(res);
    		ans=res;
			});
  			res.send("49F 5MPH Cloudy");
  		}
  		else {
  			res.render('index' , {title: 'Express'});
  		}
  	}
});

module.exports = router;
