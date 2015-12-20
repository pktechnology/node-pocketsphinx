# PocketSphinx for Node.js

This module aims to allow basic speech recognition on portable devices
through the use of PocketSphinx.

## Installation

To build this module you need to have following dependencies:

  * sphinxbase latest from github
  * pocketsphinx latest from github
  * swig 3.0
  * pkg-config
  
To build simply use npm install, it should detect everything automatically. Carefully read
build logs in order to see if there are any issues.

## Example

```javascript
var fs = require('fs');

var ps = require('pocketsphinx').ps;

modeldir = "../../pocketsphinx/model/en-us/"

var config = new ps.Decoder.defaultConfig();
config.setString("-hmm", modeldir + "en-us");
config.setString("-dict", modeldir + "cmudict-en-us.dict");
config.setString("-lm", modeldir + "en-us.lm.bin");
var decoder = new ps.Decoder(config);

fs.readFile("../../pocketsphinx/test/data/goforward.raw", function(err, data) {
    if (err) throw err;
    decoder.startUtt();
    decoder.processRaw(data, false, false);
    decoder.endUtt();
    console.log(decoder.hyp())
});
```
