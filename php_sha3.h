#ifndef PHP_SHA3_H
#define PHP_SHA3_H

#include "hash.h"

PHP_FUNCTION(sha3);
PHP_FUNCTION(shake);

extern zend_module_entry sha3_module_entry;
#define phpext_sha3_ptr &sha3_module_entry

#endif