{
  "targets": [
    {
      "target_name": "PocketSphinx",
      "cflags": ["<!(pkg-config --cflags pocketsphinx sphinxbase)"],
	    "ldflags": ["<!(pkg-config --libs pocketsphinx sphinxbase)"],
	    "xcode_settings": {
	    	"OTHER_CFLAGS": ["<!(pkg-config --cflags pocketsphinx sphinxbase)"],
	    	"OTHER_LDFLAGS": ["<!(pkg-config --libs pocketsphinx sphinxbase)"],
			},
      "sources": [ "src/Factory.cpp", "src/Recognizer.cpp" ]
    }
  ]
}