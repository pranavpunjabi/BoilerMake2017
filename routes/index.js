var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
	if(req.url == '/') {
		res.render('index');
	}
	else {
		console.log( req )
  		res.render('index', { title: 'Express', request: req.url});
  	}
});

module.exports = router;
