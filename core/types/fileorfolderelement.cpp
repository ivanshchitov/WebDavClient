// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "types/fileorfolderelement.h"

/*!
 * \brief FileOrFolderElement::operator ==
 * \param other
 * \return true, if some filelds is same to other
 */
bool FileOrFolderElement::operator==(const FileOrFolderElement &other)
{
    return other.timestamp == timestamp && other.name == name && other.busy == busy
            && other.selected == selected;
}

/*!
 * \brief FileOrFolderElement::operator !=
 * \param other
 * \return true if other not same this
 */
bool FileOrFolderElement::operator!=(const FileOrFolderElement &other)
{
    return !(*this == other);
}
