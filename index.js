var PocketSphinx = require('./build/Release/PocketSphinx.node');

util.inherits(PocketSphinx, require("stream").Writable);
module.exports = PocketSphinx;