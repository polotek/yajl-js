#ifndef YAJLJS_CALLBACKS_H
#define YAJLJS_CALLBACKS_H

#include <v8.h>
#include <yajl/yajl_parse.h>

namespace yajljs {

int onNull( void *ctx );

int onBoolean( void *ctx, int b );

}

#endif
