// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#include "SelectionCache.h"

#include "WMOInstance.h"
#include "ModelInstance.h"

SelectionCache::SelectionCache() { }

SelectionCache::~SelectionCache() { }

SelectionCache* SelectionCache::instance()
{
    static SelectionCache instance;
    return &instance;
}

std::vector<SelectionData>& SelectionCache::GetSelectedObjects()
{
    return _selected;
}

void SelectionCache::AddSelectedObject(selection_type selection)
{
    std::uint32_t uid = selection.which() == eEntry_Model
        ? boost::get<selected_model_type>(selection)->uid
        : boost::get<selected_wmo_type>(selection)->mUniqueID
        ;

    math::vector_3d pos = selection.which() == eEntry_Model
        ? boost::get<selected_model_type>(selection)->pos
        : boost::get<selected_wmo_type>(selection)->pos
        ;

    math::degrees::vec3 dir = selection.which() == eEntry_Model
        ? boost::get<selected_model_type>(selection)->dir
        : boost::get<selected_wmo_type>(selection)->dir
        ;

    std::string filename = selection.which() == eEntry_Model
        ? boost::get<selected_model_type>(selection)->model->filename
        : boost::get<selected_wmo_type>(selection)->wmo->filename
        ;

    float scale = selection.which() == eEntry_Model
        ? boost::get<selected_model_type>(selection)->scale
        : 1.f
        ;

    SelectionData data;
    data.filename = filename;
    data.dir = dir;
    data.pos = pos;
    data.scale = scale;

    _selected.push_back(data);
}

void SelectionCache::Clear()
{
    _selected.clear();
}
