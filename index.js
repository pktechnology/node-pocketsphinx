module.exports = PocketSphinx;
var PocketSphinx = require('./build/Release/recognizer');

util.inherits(PocketSphinx, require("stream").Writable);