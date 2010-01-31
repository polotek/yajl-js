#ifndef YAJLJS_H
#define YAJLJS_H

#include <yajl/yajl_parse.h>
#include <yajl/yajl_gen.h>

#include <v8.h>
#include <node.h>
#include <node/node_events.h>

namespace yajljs {
class Handle : public node::EventEmitter
{
  public:
    static void
    Initialize ( v8::Handle<v8::Object> target );

    static v8::Handle<v8::Value> Parse( const v8::Arguments& args );

  protected:
    static v8::Handle<v8::Value>
    New (const v8::Arguments& args);

    Handle( yajl_parser_config cfg );
    ~Handle();

  private:
    yajl_handle yc_handle;
    yajl_callbacks callbacks;
};
}

#endif
