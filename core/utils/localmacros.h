// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOCALMACROS_H
#define LOCALMACROS_H

#define MAKE_INTERFACE_PROPERTY(type, pname, Pname)                                                \
private:                                                                                           \
    Q_PROPERTY(type pname READ pname WRITE set##Pname NOTIFY pname##Changed)                       \
protected:                                                                                         \
    type m_##pname;                                                                                \
Q_SIGNALS:                                                                                         \
    void pname##Changed();                                                                         \
                                                                                                   \
public:                                                                                            \
    inline type pname() const                                                                      \
    {                                                                                              \
        return m_##pname;                                                                          \
    }                                                                                              \
    inline void set##Pname(type pname)                                                             \
    {                                                                                              \
        if (m_##pname == pname)                                                                    \
            return;                                                                                \
        m_##pname = pname;                                                                         \
        emit pname##Changed();                                                                     \
    }                                                                                              \
                                                                                                   \
private:

#endif // LOCALMACROS_H
