find_package(ExtraLibDir)

FIND_LIB_IN_EXTRALIBS(OPENSSL *openssl* include lib)

SET(OPENSSL_FOUND FALSE)

Find_Path(OPENSSL_INCLUDE_DIR
	openssl/opensslv.h
	/usr/include/ /usr/local/include/
	${OPENSSL_EXTRALIB_INCLUDE_PATHS}
)

# seems to be called just 'libssl' on linux, so we use that
Find_Library(OPENSSL_LIBRARY
	ssl
	/usr/lib /usr/local/lib
	${OPENSSL_EXTRALIB_LIBRARY_PATHS}
)

# Also requires 'libcrypto' when using -Wl,--as-needed
Find_Library(CRYPTO_LIBRARY
	crypto
	/usr/lib /usr/local/lib
	${OPENSSL_EXTRALIB_LIBRARY_PATHS}
)

# win32 only, no idea.... we just use it whenever we find it
Find_Library(EAY32_LIBRARY
	eay32
	/usr/lib /usr/local/lib
	${OPENSSL_EXTRALIB_LIBRARY_PATHS}
)

IF(OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARY)
	IF(CRYPTO_LIBRARY)
		SET(OPENSSL_LIBRARY
			"${OPENSSL_LIBRARY}"
			"${CRYPTO_LIBRARY}"
		)
	ENDIF(CRYPTO_LIBRARY)
	IF(EAY32_LIBRARY)
		SET(OPENSSL_LIBRARY
			"${OPENSSL_LIBRARY}"
			"${EAY32_LIBRARY}"
		)
	ENDIF()
	SET(OPENSSL_FOUND TRUE)
	MESSAGE(STATUS "Found OpenSSL library")
ENDIF()
