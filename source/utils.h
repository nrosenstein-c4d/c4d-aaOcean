/**
 * Port of Amaan Akram's aaOcean suite for Cinema 4D.
 * Copyright (C) 2017  Niklas Rosenstein
 * Copyright (C) 2020  Kent Barber
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "c4d.h"

namespace utils
{

template <class F>
void ForEachParameter(Description* desc, F func)
{
  void* handle = desc->BrowseInit();
  DescID cid, gid;
  BaseContainer const* pbc = nullptr;
  while (desc->GetNext(handle, &pbc, cid, gid)) {
    if (!func(cid, *pbc)) break;
  }
  desc->BrowseFree(handle);
}


inline void InitContainerDefaults(BaseList2D* node)
{
  auto* data = node->GetDataInstance();
  if (!data) return;

  AutoAlloc<Description> desc;
  if (!desc) return;
    node->GetDescription(desc, DESCFLAGS_DESC::NONE);
  ForEachParameter(desc, [&](DescID const& id, BaseContainer const& bc) {
    if (!data->GetDataPointer(id[0].id)) {
      GeData const* dat = bc.GetDataPointer(DESC_DEFAULT);
      if (dat) data->SetData(id[0].id, *dat);
    }
    return true;
  });
}

} // namespace utils
