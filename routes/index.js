var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
	if(req.url == '/') {
		res.render('index');
	}
	else {
  		res.render('index', { title: 'Express', request: req.name});
  	}
});

module.exports = router;
