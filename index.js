var PocketSphinx = require('./build/Release/PocketSphinx.node');

require('util').inherits(PocketSphinx, require("stream").Writable);
module.exports = PocketSphinx;