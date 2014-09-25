#include <node.h>
#include "Recognizer.h"

using namespace v8;

extern "C" {
	void InitAll(Handle<Object> exports, Handle<Object> module) {
		Recognizer::Init(exports, module);
	}

	NODE_MODULE(PocketSphinx, InitAll);
}