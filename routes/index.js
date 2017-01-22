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
  			res.send("49#5#Cloudy");
  		}
  		else if(req.url.indexOf("stocks") != -1){
  			res.send("MSFT#62.74!FB#127.04!COF#87.34!ENVA#14");
  		}
  		else {
  			res.render('index' , {title: 'Express'});
  		}
  	}
});

module.exports = router;
