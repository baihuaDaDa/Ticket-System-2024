#ifndef BAIHUA_EXCEPTIONS_HPP
#define BAIHUA_EXCEPTIONS_HPP

#include <cstddef>
#include <cstring>
#include <string>

namespace baihua {

class exception {
protected:
	const std::string variant = "";
	std::string detail = "";
public:
	exception() {}
	exception(const exception &ec) : variant(ec.variant), detail(ec.detail) {}
	virtual std::string what() {
		return variant + " " + detail;
	}
};

class index_out_of_bound : public exception {
	/* __________________________ */
};

class runtime_error : public exception {
	/* __________________________ */
};

class invalid_iterator : public exception {
	/* __________________________ */
};

class container_is_empty : public exception {
	/* __________________________ */
};
class invalid_string : public exception {
    /* __________________________ */
};
}

#endif
