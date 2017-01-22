var express = require('express');
var router = express.Router();

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
  			res.send("Weather");
  		}
  		else {
  			res.render('index' , {title: 'Express'});
  		}
  	}
});

module.exports = router;
