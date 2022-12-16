# SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
# SPDX-License-Identifier: BSD-3-Clause

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = qwebdavlib libs core app

libs.depends = qwebdavlib
core.depends = libs
app.depends = libs core

OTHER_FILES += \
    $$files(rpm/*) \
    README.md \
    README.ru.md \
    docs/build.md \
    docs/build.ru.md \
    docs/coding_conventions.md

