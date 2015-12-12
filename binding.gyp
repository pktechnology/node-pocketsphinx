{
  "targets": [
    {
      "target_name": "SphinxBase",
      "cflags": ["<!(pkg-config --cflags sphinxbase)"],
      "libraries": ["<!(pkg-config --libs sphinxbase)"],
      "xcode_settings": {
    	"OTHER_CFLAGS": ["<!(pkg-config --cflags sphinxbase)"],
    	"OTHER_LDFLAGS": ["<!(pkg-config --libs sphinxbase)"],
      },
      "sources" : ["wrap_sb.cpp"],
    },
    {
      "target_name": "PocketSphinx",
      "cflags": ["-DMODELDIR=\"<!(pkg-config --variable=modeldir pocketsphinx)\"", "<!(pkg-config --cflags pocketsphinx sphinxbase)"],
      "libraries": ["<!(pkg-config --libs pocketsphinx sphinxbase)"],
      "xcode_settings": {
    	"OTHER_CFLAGS": ["-DMODELDIR=\"<!(pkg-config --variable=modeldir pocketsphinx)\"", "<!(pkg-config --cflags pocketsphinx sphinxbase)"],
    	"OTHER_LDFLAGS": ["<!(pkg-config --libs pocketsphinx sphinxbase)"],
      },
      "sources" : ["wrap_ps.cpp"],
    },
  ]
}
