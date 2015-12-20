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
      "actions": [
         {
            "action_name": "swig_sb",
            "inputs": ["<!(pkg-config --variable=datadir sphinxbase)" "/swig/sphinxbase.i"],
            "outputs": [ "sb.cpp" ],
            "message": "Creating sphinxbase wrapper",
            "action": [
               "swig",
               "-javascript",
               "-node",
               "-DV8_VERSION=0x032872",
               "-o", "sb.cpp",
               "<!(pkg-config --variable=datadir sphinxbase)" "/swig/sphinxbase.i"
            ]
         }
       ]
    },
    {
      "target_name": "Pocketsphinx",
      "cflags": ["<!(pkg-config --cflags pocketsphinx)"],
      "libraries": ["<!(pkg-config --libs pocketsphinx)"],
      "xcode_settings": {
    	"OTHER_CFLAGS": ["<!(pkg-config --cflags pocketsphinx)"],
    	"OTHER_LDFLAGS": ["<!(pkg-config --libs pocketsphinx)"],
      },
      "sources" : ["wrap_ps.cpp"],
      "actions": [
         {
            "action_name": "swig_ps",
            "inputs": ["<!(pkg-config --variable=datadir pocketsphinx)" "/swig/pocketsphinx.i"],
            "outputs": [ "ps.cpp" ],
            "message": "Creating pocketsphinx wrapper",
            "action": [
               "swig",
               "-I" "<!(pkg-config --variable=datadir sphinxbase)" "/swig",
               "-javascript",
               "-node",
               "-DV8_VERSION=0x032872",
               "-o", "ps.cpp",
               "<!(pkg-config --variable=datadir pocketsphinx)" "/swig/pocketsphinx.i"
            ]
         }
       ]
    },
   ]
}
