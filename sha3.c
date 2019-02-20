/*
 * В файле config.m4 находится инструкция, которая запускает тестирование расширения перед компиляцией и говорит, какие файлы нужно скомпилировать.
 */


/*
 * Подключение заголовков
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_sha3.h"
/*
 * Подключение заголовков - конец
 */

// Версия расширения для phpinfo()
#define VERSION "1.0"

// Zend-структура для хранения функций расширения
zend_function_entry sha3_functions[] = {
	PHP_FE(sha3, NULL)
    PHP_FE(shake, NULL)
	PHP_FE_END
};

// Вывод информации о расширении в phpinfo()
PHP_MINFO_FUNCTION(sha3)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "sha3 support", "enabled");
    php_info_print_table_row(2, "extension version",  VERSION);
    php_info_print_table_end();
}

// Zend-структура для хранения информации о расширении
zend_module_entry sha3_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "sha3",
    sha3_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_MINFO(sha3),
#if ZEND_MODULE_API_NO >= 20010901
     VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

// Проверка на существование уже скомпиленного расширения
#ifdef COMPILE_DL_SHA3
ZEND_GET_MODULE(sha3)
#endif

/*
 * Функция sha3
 */
PHP_FUNCTION(sha3)
{
	unsigned char *message; // Сообщение
#if ZEND_MODULE_API_NO >= 20151012    
	size_t message_len; // Длина сообщения в байтах
	zend_long digest_len = 512; // Стандартная длина ключа, если не задана пользователем (в битах)
#else
    unsigned long message_len;
    unsigned short int digest_len = 512;
#endif    
    // Получение параметров, переданных пользователем в функцию
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &message, &message_len, &digest_len) == FAILURE) {
        return;
    }

    if (check_sha3_security_level(digest_len) == HASH_FAILURE) {
        zend_error(E_WARNING, "Unsupported sha3() digest length");
        RETURN_FALSE;
    }

	unsigned char digest[digest_len / 4]; // Объявление массива с символами дайджеста размером {длина ключа}/4 (в байтах)

	hash(digest_len, digest_len, message, digest, (unsigned char) 0x06); // запуск алгоритма с исходными данными
#if ZEND_MODULE_API_NO >= 20151012
	RETVAL_STRINGL(digest, digest_len / 4); // Возврат дайджеста в поток
#else
    RETURN_STRINGL(digest, digest_len / 4, 1);
#endif    
}

/*
 * Функция shake
 */
PHP_FUNCTION(shake)
{
    unsigned char *message; // Сообщение
#if ZEND_MODULE_API_NO >= 20151012    
    size_t message_len; // Длина сообщения в байтах
    zend_long security_level = 256;
    zend_long digest_len = 0; // Стандартная длина ключа, если не задана пользователем (в битах)
#else
    unsigned long message_len;
    unsigned short int security_level = 256
    unsigned long long digest_len = 0;
#endif    
    // Получение параметров, переданных пользователем в функцию
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &message, &message_len, &security_level, &digest_len) == FAILURE) {
        return;
    }

    if (digest_len == 0) {
        digest_len = security_level * 2;
    }

    if (check_shake_security_level(security_level) == HASH_FAILURE) {
        zend_error(E_WARNING, "Unsupported shake() security level");
        RETURN_FALSE;
    }

    unsigned char digest[digest_len / 4]; // Объявление массива с символами дайджеста размером {длина ключа}/4 (в байтах)
    
    hash(security_level, digest_len, message, digest, (unsigned char) 0x1f); // запуск алгоритма с исходными данными
#if ZEND_MODULE_API_NO >= 20151012
    RETVAL_STRINGL(digest, digest_len / 4); // Возврат дайджеста в поток
#else
    RETURN_STRINGL(digest, digest_len / 4, 1);
#endif 
}