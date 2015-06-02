{
  "targets": [
    {
      "target_name": "PocketSphinx",
      "cflags": ["-DMODELDIR=\"<!(pkg-config --variable=modeldir pocketsphinx)\"", "<!(pkg-config --cflags pocketsphinx sphinxbase)"],
      "libraries": ["<!(pkg-config --libs pocketsphinx sphinxbase)"],
      "xcode_settings": {
    	"OTHER_CFLAGS": ["-DMODELDIR=\"<!(pkg-config --variable=modeldir pocketsphinx)\"", "<!(pkg-config --cflags pocketsphinx sphinxbase)"],
    	"OTHER_LDFLAGS": ["<!(pkg-config --libs pocketsphinx sphinxbase)"],
      },
      "sources": [ "src/Factory.cpp", "src/Recognizer.cpp" ]
    }
  ]
}
