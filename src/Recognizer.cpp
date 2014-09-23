#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "Recognizer.h"

using namespace v8;

Persistent<Function> Recognizer::constructor;

Recognizer::Recognizer() {

}

Recognizer::~Recognizer() {
  ps_free(ps);
}

void Recognizer::Init(Handle<Object> exports, Handle<Object> module) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Recognizer"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(String::NewSymbol("start"), FunctionTemplate::New(Start)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("stop"), FunctionTemplate::New(Stop)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("restart"), FunctionTemplate::New(Restart)->GetFunction());

  tpl->PrototypeTemplate()->Set(String::NewSymbol("addKeyphraseSearch"), FunctionTemplate::New(AddKeyphraseSearch)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("addKeywordSearch"), FunctionTemplate::New(AddKeywordSearch)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("addGrammarSearch"), FunctionTemplate::New(AddGrammarSearch)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("addNgramSearch"), FunctionTemplate::New(AddNgramSearch)->GetFunction());

  tpl->PrototypeTemplate()->SetAccessor(String::NewSymbol("search"), GetSearch, SetSearch);

  tpl->PrototypeTemplate()->Set(String::NewSymbol("write"), FunctionTemplate::New(Write)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
  module->Set(String::NewSymbol("exports"), constructor);
}

Handle<Value> Recognizer::New(const Arguments& args) {
  HandleScope scope;

  if(!args.IsConstructCall()) {
    const int argc = 2;
    Local<Value> argv[argc] = { args[0], args[1] };
    return scope.Close(constructor->NewInstance(argc, argv));
  }

  if(args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Incorrect number of arguments, expected options and callback")));
    return scope.Close(Undefined());
  }

  if(!args[0]->IsObject()) {
    ThrowException(Exception::TypeError(String::New("Expected options to be an object")));
    return scope.Close(Undefined());
  }

  if(!args[1]->IsFunction()) {
    ThrowException(Exception::TypeError(String::New("Expected callback to be a function")));
    return scope.Close(Undefined());
  }
  
  Recognizer* instance = new Recognizer();

  Handle<Object> options = args[0]->ToObject();
  instance->callback = Persistent<Function>::New(Local<Function>::Cast(args[1]));

  v8::String::AsciiValue hmmValue(options->Get(String::NewSymbol("hmm")));
  v8::String::AsciiValue lmValue(options->Get(String::NewSymbol("lm")));
  v8::String::AsciiValue dictValue(options->Get(String::NewSymbol("dict")));
  v8::String::AsciiValue samprateValue(options->Get(String::NewSymbol("samprate")));
  v8::String::AsciiValue nfftValue(options->Get(String::NewSymbol("nfft")));
  v8::String::AsciiValue kws_thresholdValue(options->Get(String::NewSymbol("kws_threshold")));
  v8::String::AsciiValue logfnValue(options->Get(String::NewSymbol("logfn")));
  v8::String::AsciiValue mmapValue(options->Get(String::NewSymbol("mmap")));

  cmd_ln_t* config = cmd_ln_init(NULL, ps_args(), TRUE,
    "-hmm", *hmmValue,
    "-lm", *lmValue,
    "-dict", *dictValue,
    "-samprate", *samprateValue,
    "-nfft", *nfftValue,
    "-kws_threshold": *kws_thresholdValue,
    "-logfn", *logfnValue,
    "-mmap", *mmapValue
    NULL);

  instance->ps = ps_init(config);

  instance.Wrap(args.This());

  return args.This();
}

Handle<Value> Recognizer::AddKeyphraseSearch(const Arguments& args) {
  v8::HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  if(args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Incorrect number of arguments, expected name and keyphrase")));
    return scope.Close(args.This());
  }

  if(!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(Exception::TypeError(String::New("Expected both name and keyphrase to be strings")));
    return scope.Close(args.This());
  }

  v8::String::AsciiValue name = args[0];
  v8::String::AsciiValue keyphrase = args[1];

  int result = ps_set_keyphrase(instance->ps, *name, *keyphrase);
  if(!result)
    ThrowException(Exception::Error(String::New("Failed to add keyphrase search to recognizer")));

  return scope.Close(args.This());
}

Handle<Value> Recognizer::AddKeywordSearch(const Arguments& args) {
  v8::HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  if(args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Incorrect number of arguments, expected name and file")));
    return scope.Close(args.This());
  }

  if(!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(Exception::TypeError(String::New("Expected both name and file to be strings")));
    return scope.Close(args.This());
  }

  v8::String::AsciiValue name = args[0];
  v8::String::AsciiValue file = args[1];

  int result = ps_set_keyword(instance->ps, *name, *file);
  if(!result)
    ThrowException(Exception::Error(String::New("Failed to add keywords search to recognizer")));

  return scope.Close(args.This());
}

Handle<Value> Recognizer::AddGrammarSearch(const Arguments& args) {
  v8::HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  if(args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Incorrect number of arguments, expected name and file")));
    return scope.Close(args.This());
  }

  if(!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(Exception::TypeError(String::New("Expected both name and file to be strings")));
    return scope.Close(args.This());
  }

  v8::String::AsciiValue name = args[0];
  v8::String::AsciiValue file = args[1];

  int result = ps_set_jsgf_file(instance->ps, *name, *file);
  if(!result)
    ThrowException(Exception::Error(String::New("Failed to add grammar search to recognizer")));

  return scope.Close(args.This());
}

Handle<Value> Recognizer::AddNgramSearch(const Arguments& args) {
  v8::HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  if(args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Incorrect number of arguments, expected name and file")));
    return scope.Close(args.This());
  }

  if(!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(Exception::TypeError(String::New("Expected both name and file to be strings")));
    return scope.Close(args.This());
  }

  v8::String::AsciiValue name = args[0];
  v8::String::AsciiValue file = args[1];

  int result = ps_set_lm_file(instance->ps, *name, *file);
  if(!result)
    ThrowException(Exception::Error(String::New("Failed to add Ngram search to recognizer")));

  return scope.Close(args.This());
}

Handle<Value> Recognizer::GetSearch(const Local<String> property, const AccessorInfo& info) {
  v8::HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  v8::String::AsciiValue search = String::New(ps_get_search(instance->ps));

  return scope.Close(search);
}

Handle<Value> Recognizer::SetSearch(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  v8::HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  v8::String::AsciiValue search = String::Cast(value);

  ps_set_search(instance->ps, *search);

  return scope.Close(Undefined());
}

Handle<Value> Recognizer::Start(const Arguments& args) {
  v8::HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  int result = ps_start_utt(instance->ps, NULL);
  if(!result)
    ThrowException(Exception::Error(String::New("Failed to start PocketSphinx processing")));

  return scope.Close(args.This());
}

Handle<Value> Recognizer::Stop(const Arguments& args) {
  v8::HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  int result = ps_end_utt(instance->ps);
  if(!result)
    ThrowException(Exception::Error(String::New("Failed to end PocketSphinx processing")));

  return scope.Close(args.This());
}

Handle<Value> Recognizer::Restart(const Arguments& args) {
  v8::HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  int result = ps_start_utt(instance->ps, NULL);
  if(!result)
    ThrowException(Exception::Error(String::New("Failed to start PocketSphinx processing")));
  
  result = ps_end_utt(instance->ps);
  if(!result)
    ThrowException(Exception::Error(String::New("Failed to restart PocketSphinx processing")));

  return scope.Close(args.This());
}

Handle<Value> Recognizer::Write(const Arguments& args) {
  v8::HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  if(!args.Length()) {
    ThrowException(Exception::TypeError("Expected a data buffer to be provided"));
    return scope.Close(args.This());
  }

  if(!node::Buffer::HasInstance(args[0])) {
    Local<Value> argv[1] = { Exception::Error(String::New("Expected data to be a buffer")) };
    instance->callback->Call(Context::GetCurrent()->Global(), 1, argv);
    return scope.Close(args.This());
  }

  AsyncData* data = new AsyncData();
  data->instance = instance;
  data->data = (float*) node::Buffer::Data(args[0]);
  data->length = node::Buffer::Length(args[0]) / sizeof(float);

  uv_work_t* req = new uv_work_t();
  req->data = data;

  uv_queue_work(uv_default_loop(), req, AsyncWorker, AsyncAfter);

  return scope.Close(args.This());
}

void Recognizer::AsyncWorker(uv_work_t* request) {
  AsyncData* data = (AsyncData*)request->data;

  int16* downsampled = new int16(data->length);
  for(size_t i = 0; i < data->length; i++) downsampled[i] = data->data[i] * 32768;
  
  const char* uttid;
  int32 score;
  const char* hyp = ps_get_hyp(data->instance->ps, &score, &uttid);

  data->score = score;
  data->uttid = uttid;
  data->hyp = hyp;

  delete downsampled;
}

void Recognizer::AsyncAfter(uv_work_t* request) {
  AsyncData* data = (AsyncData*)request->data;
  
  Local<Value> argv[3] = { String::NewSymbol(data->hyp), NumberObject::New(data->score), String::NewSymbol(data->uttid) };
  data->instance->callback->Call(Context::GetCurrent()->Global(), 3, argv);
}