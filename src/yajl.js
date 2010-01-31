var yajl = require('./binding_yajl');
exports.Generator = function( conf ) {
}

/**
 * Create a new YAJL handle to parse JSON
 * Parameters:
 *   parser_config - a literal object (optional)
 *     default:
 *      { allowComments : true, checkUTF8 : true }
 */
exports.createHandle = function(parser_config) {
    cfg = process.mixin( {
                allowComments : true
              , checkUTF8 : true
            }, parser_config );
    return new yajl.Handle( cfg );
}
