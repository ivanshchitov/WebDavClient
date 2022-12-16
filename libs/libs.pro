# SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
# SPDX-License-Identifier: BSD-3-Clause

TEMPLATE = aux

# QtWebDav bulid directory
WEBDAV_BUILD_PATH=$$OUT_PWD/../qwebdavlib

webdav_library_install.path = /usr/share/ru.auroraos.AuroraOsWebDavGuiClient/lib/
webdav_library_install.files = $$WEBDAV_BUILD_PATH/*.so*
webdav_library_install.CONFIG = no_check_exist

INSTALLS += \
    webdav_library_install \
