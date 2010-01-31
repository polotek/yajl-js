#ifndef YAJLJS_CALLBACKS_H
#define YAJLJS_CALLBACKS_H

#include <v8.h>
#include <yajl/yajl_parse.h>

namespace yajljs {
int OnNull( void *ctx );
int OnBoolean( void *ctx, int b );
int OnInteger( void *ctx, long b );
int OnDouble( void *ctx, double b );
int OnNumber( void *ctx, const char *numberVal, unsigned int numberLen );
int OnString( void *ctx, const unsigned char *stringVal, unsigned int stringLen );
int OnStartMap( void *ctx );
int OnMapKey( void *ctx, const unsigned char *key, unsigned int stringLen );
int OnEndMap( void *ctx );
int OnStartArray( void *ctx );
int OnEndArray( void *ctx );
}

#endif
