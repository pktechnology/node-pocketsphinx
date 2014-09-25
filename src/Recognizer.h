#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <pocketsphinx.h>
#include <sphinxbase/err.h>
#include <sphinxbase/jsgf.h>

class Recognizer : public node::ObjectWrap
{
public:
	static void Init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module);

private:
	explicit Recognizer();
	~Recognizer();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	static v8::Handle<v8::Value> Start(const v8::Arguments& args);
	static v8::Handle<v8::Value> Stop(const v8::Arguments& args);
	static v8::Handle<v8::Value> Restart(const v8::Arguments& args);

	static v8::Handle<v8::Value> Write(const v8::Arguments& args);

	static v8::Handle<v8::Value> AddKeyphraseSearch(const v8::Arguments& args);
	static v8::Handle<v8::Value> AddKeywordsSearch(const v8::Arguments& args);
	static v8::Handle<v8::Value> AddGrammarSearch(const v8::Arguments& args);
	static v8::Handle<v8::Value> AddNgramSearch(const v8::Arguments& args);

	static v8::Handle<v8::Value> GetSearch(const v8::Local<v8::String> property, const v8::AccessorInfo& info);
	static void SetSearch(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Persistent<v8::Function> constructor;
	static void AsyncWorker(uv_work_t* request);
	static void AsyncAfter(uv_work_t* request);

	static v8::Local<v8::Value> Default(v8::Local<v8::Value> value, v8::Local<v8::Value> fallback);

	ps_decoder_t* ps;
	v8::Persistent<v8::Function> callback;
};

typedef struct AsyncData {
  Recognizer* instance;
  v8::Handle<v8::Value> exception;
  bool hasException;
  float* data;
  size_t length;
  const char* uttid;
  int32 score;
  const char* hyp;
} AsyncData;

#endif