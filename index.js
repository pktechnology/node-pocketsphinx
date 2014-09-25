var PocketSphinx = require('./build/Release/recognizer.node');

util.inherits(PocketSphinx, require("stream").Writable);
module.exports = PocketSphinx;