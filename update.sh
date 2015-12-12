#!/bin/sh

swig \
    -I`pkg-config --variable=datadir sphinxbase`/swig \
    -javascript \
    -node \
    -DV8_VERSION=0x032872 \
    -o wrap_ps.cpp \
    `pkg-config --variable=datadir pocketsphinx`/swig/pocketsphinx.i


swig \
    -javascript \
    -node \
    -DV8_VERSION=0x032872 \
    -o wrap_sb.cpp \
    `pkg-config --variable=datadir sphinxbase`/swig/sphinxbase.i

