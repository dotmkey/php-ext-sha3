PHP_ARG_ENABLE(sha3, to enable the sha3 extension, [--enable-sha3   enable the sha3 extension.])

if test "$PHP_SHA3" != no; then 
  PHP_NEW_EXTENSION(sha3, sha3.c hash.c sponge.c keccak.c, $ext_shared)
fi