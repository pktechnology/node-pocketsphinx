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

	}, function(hypothesis, score, id) {
		socket.emit('utterance', { phrase: hypothesis, id: id, score: score });
	});

	sphinx.start();

	socket.on('audio', function(data) {
		sphinx.write(data);
	});

	socket.on('restart', function() {
		sphinx.restart();
	});
});

server.listen(process.env.port || 3000, function() {
	console.log('Listening on port %d', process.env.port || 3000);
});