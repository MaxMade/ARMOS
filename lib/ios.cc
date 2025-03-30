#include <ios.h>

lib::ostream& lib::boolalpha(lib::ostream& str) {
	str.setf(lib::ostream::boolalpha);
	return str;
}

lib::ostream& lib::noboolalpha(lib::ostream& str) {
	str.unsetf(lib::ostream::boolalpha);
	return str;
}

lib::ostream& lib::showbase(lib::ostream& str) {
	str.setf(lib::ostream::showbase);
	return str;
}

lib::ostream& lib::noshowbase(lib::ostream& str) {
	str.unsetf(lib::ostream::showbase);
	return str;
}

lib::ostream& lib::uppercase(lib::ostream& str) {
	str.setf(lib::ostream::uppercase);
	return str;
}

lib::ostream& lib::nouppercase(lib::ostream& str) {
	str.unsetf(lib::ostream::uppercase);
	return str;
}

lib::ostream& lib::left(lib::ostream& str) {
	str.setf(lib::ostream::left, lib::ostream::adjustfield);
	return str;
}

lib::ostream& lib::right(lib::ostream& str) {
	str.setf(lib::ostream::right, lib::ostream::adjustfield);
	return str;
}

lib::ostream& lib::dec(lib::ostream& str) {
	str.setf(lib::ostream::dec, lib::ostream::basefield);
	return str;
}

lib::ostream& lib::hex(lib::ostream& str) {
	str.setf(lib::ostream::hex, lib::ostream::basefield);
	return str;
}

lib::ostream& lib::oct(lib::ostream& str) {
	str.setf(lib::ostream::oct, lib::ostream::basefield);
	return str;
}

lib::ostream& lib::bin(lib::ostream& str) {
	str.setf(lib::ostream::bin, lib::ostream::basefield);
	return str;
}

lib::ostreamFill lib::setfill(char c) {
	return ostreamFill(c);
}

lib::ostreamWidth lib::setw(size_t w) {
	return ostreamWidth(w);
}
