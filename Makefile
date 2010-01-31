all: yajl-js doc

yajl-js:
	node-waf configure build

install: yajl-js
	node-waf install

clean:
	node-waf clean

distclean:
	node-waf distclean

uninstall:
	node-waf uninstall

doc: doc/README.html

doc/README.html: doc/README.txt
	asciidoc -a toc -a linkcss -o doc/README.html doc/README.txt
