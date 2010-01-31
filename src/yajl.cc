#include <iostream>

#include <yajl/yajl_parse.h>
#include <yajl/yajl_gen.h>

#include <v8.h>
#include <node.h>
#include <node/node_events.h>

#include "callbacks.h"
#include "yajl.h"

using namespace v8;
using namespace node;

namespace yajljs {
void Handle::Initialize ( v8::Handle<v8::Object> target )
{
    v8::HandleScope scope;

    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);

    t->Inherit( EventEmitter::constructor_template );
    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD( t, "parse", Parse );
    target->Set( v8::String::NewSymbol( "Handle"), t->GetFunction() );
}

v8::Handle<v8::Value>
Handle::New (const v8::Arguments& args)
{
    v8::HandleScope scope;

    yajl_parser_config cfg;

    if( args.Length() < 1 || !args[0]->IsObject() )
    {
        return ThrowException( Exception::TypeError(
                    String::New( "Argument 0 must be a yajl_parser_config like object" ) ) );
    }

    Local<Object> obj = args[0]->ToObject();

    Local<Integer> allowComments = obj->Get( String::New( "allowComments" ) )->ToInteger();

    cfg.allowComments = allowComments->Value();
    Handle *handle = new Handle( cfg );
    handle->Wrap( args.This() );
    return args.This();
}

Handle::Handle( yajl_parser_config cfg ) : EventEmitter()
{
    callbacks.yajl_null = onNull;
    callbacks.yajl_boolean = onBoolean;
    
    // TODO expose
    yc_handle = yajl_alloc( &callbacks, &cfg, NULL, NULL );
}

Handle::~Handle()
{
    yajl_free( yc_handle );
}

}

v8::Handle<Value> yajljs::Handle::Parse( const Arguments& args )
{
    HandleScope scope;
    std::cerr << "PARSING\n";
    return args.This();
}

extern "C" void
init (v8::Handle<v8::Object> target)
{
    v8::HandleScope scope;
    yajljs::Handle::Initialize(target);
}
