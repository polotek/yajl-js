// The complete reformatter in javascript
var sys = require('sys');
var posix = require('posix');
var yajl = require('./src/yajl');

var callbacks = {
    onNull : function(gen) {
        gen.addNull();
    },
    onBoolean : function(gen, bool) {
        gen.boolean( bool );
    },
    onNumber : function(gen, str) {
        gen.number( str );
    },
    onString : function(gen, str) {
        gen.string( str );
    },
    onMapKey : function(gen, key) {
        gen.string( key );
    },
    onStartMap : function(gen) {
        g.openMap();
    },
    onEndMap : function(gen) {
        g.closeMap();
    },
    onStartArray : function(gen) {
        g.openArray();
    },
    onEndArray : function(gen) {
        g.closeArray();
    }
};

function usage() {
    sys.error( "Usage: %s <filename>\n"
               +"    -m Minimize json rather than beautify (default)\n"
               +"    -u allow invalid UTF8 inside strings during parsing\n");
    process.exit(1);
}

var conf = {
    beautify: 1,
    indentString: "  "
};

var cfg = {
    allowComments: 1,
    checkUTF8: 1
};

if( process.ARGV.length == 3 ) {
    if( process.ARGV[2] == "-m" )
        conf.beautify = 0;
    else if( process.ARGV[2] == "-u" )
        cfg.checkUTF8 = 0;
    else
        usage();
}
else if( process.ARGV.length != 2 ) {
    usage();
}

var g = new yajl.Generator( conf, null );

var handle = yajl.createHandle();
handle.addListener( "null", function() {
    sys.debug("Nullify");
});

var done = false;

process.stdio.open();
process.stdio.addListener( "data", function( data ) {
    var stat = handle.parse(data);
//    if( stat != yajl.STATUS_OK &&
//        stat != yajl.STATUS_INSUFFICIENT_DATA ) {
//        sys.error( handle.error( yajl.ERROR_VERBOSE, data ) );
//    }
});
//
//process.stdio.addListener( "close", function() {
//    var stat = handle.parseComplete();
//    if( stat != yajl.STATUS_OK )
//        sys.error( handle.error( yajl.ERROR_VERBOSE, data ) );
//    else
//        process.stdio.write( g.buffer() );
//});
