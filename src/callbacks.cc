#include <iostream>
#include <v8.h>
#include <node.h>
#include <yajl/yajl_parse.h>

#include "callbacks.h"
#include "yajl.h"

using namespace v8;
using namespace node;

#define INIT_CB( ctx )\
    HandleScope scope;\
    Handle *yh = static_cast<Handle *>( ctx )

namespace yajljs {
int OnNull( void *ctx )
{
    INIT_CB( ctx );
    yh->Emit( v8::String::New("null"), 0, NULL );
    return 1;
}

int OnBoolean( void *ctx, int b )
{
    INIT_CB( ctx );
    Local<Value> args[1] = { Integer::New( b ) };
    yh->Emit( v8::String::New("boolean"), 1, args );
    return 1;
}

int OnInteger( void *ctx, long b )
{
    INIT_CB( ctx );

    Local<Value> args[1] = { Integer::New( b ) };
    yh->Emit( String::New("integer"), 1, args );
    return 1;
}

int OnDouble( void *ctx, double b )
{
    INIT_CB( ctx );

    Local<Value> args[1] = { Number::New( b ) };
    yh->Emit( String::New("double"), 1, args );
    return 1;
}

int OnNumber( void *ctx, const char *numberVal, unsigned int numberLen )
{
    INIT_CB( ctx );

    Local<Value> args[1] = { String::New( numberVal, numberLen ) };
    yh->Emit( String::New("number"), 1, args );
    return 1;
}

int OnString( void *ctx, const unsigned char *stringVal, unsigned int stringLen )
{
    INIT_CB( ctx );
    Local<Value> args[1] = { String::New( (char *)stringVal, stringLen ) };
    yh->Emit( String::New("string"), 1, args );
    return 1;
}

int OnStartMap( void *ctx )
{
    INIT_CB( ctx );
    yh->Emit( String::New("startMap"), 0, NULL );
    return 1;
}

int OnMapKey( void *ctx, const unsigned char *key, unsigned int stringLen )
{
    INIT_CB( ctx );
    Local<Value> args[1] = { String::New( (char *)key, stringLen ) };
    yh->Emit( String::New("mapKey"), 1, args );
    return 1;
}

int OnEndMap( void *ctx )
{
    INIT_CB( ctx );
    yh->Emit( String::New("endMap"), 0, NULL );
    return 1;
}

int OnStartArray( void *ctx )
{
    INIT_CB( ctx );
    yh->Emit( String::New("startArray"), 0, NULL );
    return 1;
}

int OnEndArray( void *ctx )
{
    INIT_CB( ctx );
    yh->Emit( String::New("endArray"), 0, NULL );
    return 1;
}

void
FillCallbacks( Local<Object> obj, yajl_callbacks *cbs )
{
    Local<Array> props = obj->GetPropertyNames();
    for( int i = 0; i < props->Length(); i++ )
    {
        Local<Value> k = props->Get(Number::New(i));
        String::AsciiValue key(k->ToString());
        String::AsciiValue val( obj->Get( k->ToString() )->ToString() );
        std::cout << *key << ":" << *val << "\n";
    }
}
}
