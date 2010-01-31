#include <iostream>
#include <v8.h>
#include <node.h>
#include <yajl/yajl_parse.h>

#include "callbacks.h"
#include "yajl.h"

using namespace v8;
using namespace node;

namespace yajljs {
int onNull( void *ctx )
{
    Handle *yh = static_cast<Handle *>( ctx );
    yh->Emit( v8::String::New("null"), 0, NULL );
}

int onBoolean( void *ctx, int b )
{
    HandleScope scope;
    Handle *yh = static_cast<Handle *>( ctx );

    Local<Value> args[] = { Integer::New( b ) };
    yh->Emit( v8::String::New("boolean"), 1, args );
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
