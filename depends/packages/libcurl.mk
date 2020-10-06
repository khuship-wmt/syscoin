package=curl
$(package)_version=7.52.1
$(package)_dependencies=gnutls
$(package)_download_path=https://curl.haxx.se/download
$(package)_file_name=curl-$($(package)_version).tar.gz
$(package)_sha256_hash=a8984e8b20880b621f61a62d95ff3c0763a3152093a9f9ce4287cfd614add6ae

# default settings
$(package)_config_opts=LIBS="-lnettle -lhogweed -lgmp" --without-nghttp2 --disable-shared --enable-static --disable-ftp --without-ssl --with-gnutls="$(PREFIX_DIR)" --disable-ntlm-wb --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher --enable-proxy --without-ca-path --without-ca-bundle --with-ca-fallback --enable-threaded-resolver

# mingw specific settings
$(package)_config_opts_mingw32=LIBS="-lcrypt32 -lnettle -lhogweed -lgmp -lgnutls" --without-nghttp2 CFLAGS="-DCURL_STATICLIB" --disable-shared --enable-static --disable-ftp --without-ssl --with-gnutls="$(PREFIX_DIR)" --disable-ntlm-wb --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher --enable-proxy --without-ca-path --without-ca-bundle --with-ca-fallback

# darwin specific settings
$(package)_config_opts_darwin=--with-sysroot="$(DARWIN_SDK_PATH)" --disable-shared --enable-static --disable-ftp --without-gnutls --with-darwinssl --without-ssl --disable-ntlm-wb --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher --enable-proxy --enable-threaded-resolver

define $(package)_config_cmds
  $($(package)_autoconf)
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef