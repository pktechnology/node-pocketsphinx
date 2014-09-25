var PocketSphinx = require('../'),
	express = require('express'),
	socket_io = require('socket.io'),
	http = require('http');

var app = express();

app.use(function(req, res) {
	return res.status(200).sendFile('index.html', { root: __dirname + "/public" });
});


var server = http.Server(app),
	io = socket_io(server);

io.on('connection', function(socket) {
	var sphinx = new PocketSphinx({
		samprate: '16000'
	}, function(err, hypothesis, score, id) {
		if(err) console.error(err);
		socket.emit('utterance', { phrase: hypothesis, id: id, score: score });
	});

	sphinx.addGrammarSearch('digits', __dirname + '/digits.gram');
	sphinx.search = 'digits';

	sphinx.start();

	socket.emit('ready');

	socket.on('audio', function(data) {
		var resampled = PocketSphinx.fromFloat(data);
		sphinx.writeSync(resampled);
	});

	socket.on('restart', function() {
		sphinx.restart();
	});
});

server.listen(process.env.port || 3000, function() {
	console.log('Listening on port %d', process.env.port || 3000);
});