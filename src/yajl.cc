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

/*
 * Macro from the sqlite3 bindings
 * http://github.com/grumdrig/node-sqlite/blob/master/sqlite3_bindings.cc
 * by Eric Fredricksen
 */
#define REQ_STR_ARG(I, VAR)                                             \
      if (args.Length() <= (I) || !args[I]->IsString())                     \
    return ThrowException(Exception::TypeError(                         \
                                  String::New("Argument " #I " must be a string"))); \
  String::Utf8Value VAR(args[I]->ToString());

namespace yajljs {
void Handle::Initialize ( v8::Handle<v8::Object> target )
{
    v8::HandleScope scope;

    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);

    t->Inherit( EventEmitter::constructor_template );
    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD( t, "parse", Parse );
    NODE_SET_PROTOTYPE_METHOD( t, "parseComplete", ParseComplete );
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
    callbacks.yajl_null = OnNull;
    callbacks.yajl_boolean = OnBoolean;
    callbacks.yajl_integer = OnInteger;
    callbacks.yajl_double = OnDouble;
    callbacks.yajl_number = OnNumber;
    callbacks.yajl_string = OnString;
    callbacks.yajl_start_map = OnStartMap;
    callbacks.yajl_map_key = OnMapKey;
    callbacks.yajl_end_map = OnEndMap;
    callbacks.yajl_start_array = OnStartArray;
    callbacks.yajl_end_array = OnEndArray;
    
    yc_handle = yajl_alloc( &callbacks, &cfg, NULL, this);
}

Handle::~Handle()
{
    yajl_free( yc_handle );
}

v8::Handle<Value> Handle::Parse( const Arguments& args )
{
    HandleScope scope;

    REQ_STR_ARG(0, parseString);

    Handle *yh = Unwrap<Handle>( args.This() );

    yh->Parse( const_cast<unsigned char*>( (unsigned char *) *parseString ), parseString.length() );
    //yajl_parse( yh->yc_handle, const_cast<unsigned char*>((unsigned char *)*parseString), parseString.length() );
    // fix this we probably want to return status, or maybe raise an exception
    return args.This();
}

v8::Handle<Value> Handle::ParseComplete( const Arguments& args )
{
    HandleScope scope;
    Handle *yh = Unwrap<Handle>( args.This() );
    yh->ParseComplete();
}

v8::Handle<Value> Handle::Parse( unsigned char* str, int len )
{
    int status = yajl_parse( yc_handle, str, len );
    if( status != yajl_status_ok
        && status != yajl_status_insufficient_data )
    {
        unsigned char *errorMsg = yajl_get_error( yc_handle, 1, str, len );
        Local<Value> args[1] = { String::New( (char *)errorMsg ) };
        Emit( String::New( "error" ), 1, args );
        yajl_free_error( yc_handle, errorMsg );
    }
}

v8::Handle<Value> Handle::ParseComplete()
{
    int status = yajl_parse_complete( yc_handle );
    if( status != yajl_status_ok
        && status != yajl_status_insufficient_data )
    {
        unsigned char *errorMsg = yajl_get_error( yc_handle, 1, (unsigned char *)"", 0 );
        Local<Value> args[1] = { String::New( (char *)errorMsg ) };
        Emit( String::New( "error" ), 1, args );
        yajl_free_error( yc_handle, errorMsg );
    }
}
}

extern "C" void
init (v8::Handle<v8::Object> target)
{
    v8::HandleScope scope;
    yajljs::Handle::Initialize(target);
}
