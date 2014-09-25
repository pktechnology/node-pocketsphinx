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
  tpl->PrototypeTemplate()->Set(String::NewSymbol("addKeywordsSearch"), FunctionTemplate::New(AddKeywordsSearch)->GetFunction());
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

  String::AsciiValue hmmValue(Default(options->Get(String::NewSymbol("hmm")), String::NewSymbol(MODELDIR "/hmm/en_US/hub4wsj_sc_8k")));
  String::AsciiValue lmValue(Default(options->Get(String::NewSymbol("lm")), String::NewSymbol(MODELDIR "/lm/en_US/hub4.5000.DMP")));
  String::AsciiValue dictValue(Default(options->Get(String::NewSymbol("dict")), String::NewSymbol(MODELDIR "/lm/en_US/cmu07a.dic")));
  String::AsciiValue samprateValue(Default(options->Get(String::NewSymbol("samprate")), String::NewSymbol("44100")));
  String::AsciiValue nfftValue(Default(options->Get(String::NewSymbol("nfft")), String::NewSymbol("2048")));
  String::AsciiValue kws_thresholdValue(options->Get(String::NewSymbol("kws_threshold")));
  String::AsciiValue logfnValue(Default(options->Get(String::NewSymbol("logfn")), String::NewSymbol("/dev/null")));
  String::AsciiValue mmapValue(options->Get(String::NewSymbol("mmap")));

  cmd_ln_t* config = cmd_ln_init(NULL, ps_args(), TRUE,
    "-hmm", *hmmValue,
    "-lm", *lmValue,
    "-dict", *dictValue,
    "-samprate", *samprateValue,
    "-nfft", *nfftValue,
    //"-kws_threshold", *kws_thresholdValue,
    //"-logfn", *logfnValue,
    //"-mmap", *mmapValue,
    NULL);

  instance->ps = ps_init(config);

  instance->Wrap(args.This());

  return args.This();
}

Handle<Value> Recognizer::AddKeyphraseSearch(const Arguments& args) {
  HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  if(args.Length() < 2) {
    ThrowException(Exception::TypeError(String::NewSymbol("Incorrect number of arguments, expected name and keyphrase")));
    return scope.Close(args.This());
  }

  if(!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(Exception::TypeError(String::NewSymbol("Expected both name and keyphrase to be strings")));
    return scope.Close(args.This());
  }

  String::AsciiValue name(args[0]);
  String::AsciiValue keyphrase(args[1]);

  int result = ps_set_keyphrase(instance->ps, *name, *keyphrase);
  if(!result)
    ThrowException(Exception::Error(String::NewSymbol("Failed to add keyphrase search to recognizer")));

  return scope.Close(args.This());
}

Handle<Value> Recognizer::AddKeywordsSearch(const Arguments& args) {
  HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  if(args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Incorrect number of arguments, expected name and file")));
    return scope.Close(args.This());
  }

  if(!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(Exception::TypeError(String::New("Expected both name and file to be strings")));
    return scope.Close(args.This());
  }

  String::AsciiValue name(args[0]);
  String::AsciiValue file(args[1]);

  int result = ps_set_kws(instance->ps, *name, *file);
  if(!result)
    ThrowException(Exception::Error(String::New("Failed to add keywords search to recognizer")));

  return scope.Close(args.This());
}

Handle<Value> Recognizer::AddGrammarSearch(const Arguments& args) {
  HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  if(args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Incorrect number of arguments, expected name and file")));
    return scope.Close(args.This());
  }

  if(!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(Exception::TypeError(String::New("Expected both name and file to be strings")));
    return scope.Close(args.This());
  }

  String::AsciiValue name(args[0]);
  String::AsciiValue file(args[1]);

  int result = ps_set_jsgf_file(instance->ps, *name, *file);
  if(!result)
    ThrowException(Exception::Error(String::New("Failed to add grammar search to recognizer")));

  return scope.Close(args.This());
}

Handle<Value> Recognizer::AddNgramSearch(const Arguments& args) {
  HandleScope scope;
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  if(args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Incorrect number of arguments, expected name and file")));
    return scope.Close(args.This());
  }

  if(!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(Exception::TypeError(String::New("Expected both name and file to be strings")));
    return scope.Close(args.This());
  }

  String::AsciiValue name(args[0]);
  String::AsciiValue file(args[1]);

  int result = ps_set_lm_file(instance->ps, *name, *file);
  if(!result)
    ThrowException(Exception::Error(String::New("Failed to add Ngram search to recognizer")));

  return scope.Close(args.This());
}

Handle<Value> Recognizer::GetSearch(const Local<String> property, const AccessorInfo& info) {
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(info.This());

  Local<Value> search = String::NewSymbol(ps_get_search(instance->ps));

  return search;
}

void Recognizer::SetSearch(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(info.This());

  String::AsciiValue search(value);

  ps_set_search(instance->ps, *search);
}

Handle<Value> Recognizer::Start(const Arguments& args) {
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  int result = ps_start_utt(instance->ps, NULL);
  if(result)
    ThrowException(Exception::Error(String::New("Failed to start PocketSphinx processing")));

  return args.This();
}

Handle<Value> Recognizer::Stop(const Arguments& args) {
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  int result = ps_end_utt(instance->ps);
  if(result)
    ThrowException(Exception::Error(String::New("Failed to end PocketSphinx processing")));

  return args.This();
}

Handle<Value> Recognizer::Restart(const Arguments& args) {
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  int result = ps_start_utt(instance->ps, NULL);
  if(result)
    ThrowException(Exception::Error(String::New("Failed to start PocketSphinx processing")));
  
  result = ps_end_utt(instance->ps);
  if(result)
    ThrowException(Exception::Error(String::New("Failed to restart PocketSphinx processing")));

  return args.This();
}

Handle<Value> Recognizer::Write(const Arguments& args) {
  Recognizer* instance = node::ObjectWrap::Unwrap<Recognizer>(args.This());

  if(!args.Length()) {
    ThrowException(Exception::TypeError(String::NewSymbol("Expected a data buffer to be provided")));
    return args.This();
  }

  if(!node::Buffer::HasInstance(args[0])) {
    Local<Value> argv[1] = { Exception::Error(String::NewSymbol("Expected data to be a buffer")) };
    instance->callback->Call(Context::GetCurrent()->Global(), 1, argv);
    return args.This();
  }

  AsyncData* data = new AsyncData();
  data->instance = instance;
  data->data = (float*) node::Buffer::Data(args[0]);
  data->length = node::Buffer::Length(args[0]) / sizeof(float);

  uv_work_t* req = new uv_work_t();
  req->data = data;

  uv_queue_work(uv_default_loop(), req, AsyncWorker, (uv_after_work_cb)AsyncAfter);

  return args.This();
}

void Recognizer::AsyncWorker(uv_work_t* request) {
  AsyncData* data = reinterpret_cast<AsyncData*>(request->data);

  int16* downsampled = new int16[data->length];
  for(size_t i = 0; i < data->length; i++) downsampled[i] = data->data[i] * 32768;

  if(ps_process_raw(data->instance->ps, downsampled, data->length, FALSE, FALSE)) {
    data->hasException = TRUE;
    data->exception = Persistent<Value>::New(Exception::Error(String::NewSymbol("Failed to process audio data")));
    delete [] downsampled;
    return;
  }
  
  const char* uttid;
  int32 score;
  const char* hyp = ps_get_hyp(data->instance->ps, &score, &uttid);

  data->score = score;
  data->uttid = uttid;
  data->hyp = hyp;

  delete [] downsampled;
}

void Recognizer::AsyncAfter(uv_work_t* request) {
  AsyncData* data = reinterpret_cast<AsyncData*>(request->data);

  if(data->hasException) {
    Local<Value> argv[1] = { Local<Value>::New(data->exception) };
    data->exception.Reset();
    data->instance->callback->Call(Context::GetCurrent()->Global(), 1, argv);
  } else {
    Local<Value> argv[4] = { Local<Value>::New(Undefined()), String::NewSymbol(data->hyp), NumberObject::New(data->score), String::NewSymbol(data->uttid) };
    data->instance->callback->Call(Context::GetCurrent()->Global(), 4, argv);
  }
}

Local<Value> Recognizer::Default(Local<Value> value, Local<Value> fallback) {
  if(value->IsUndefined()) return fallback;
  return value;
}