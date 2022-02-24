// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

#include <math/trig.hpp>
#include <math/vector_3d.hpp>

#include <noggit/Selection.h>

#include <vector>
#include <string>

struct SelectionData
{
    std::string filename = "";
    float scale = 1.f;
    math::degrees::vec3 dir;
    math::vector_3d pos;
};

class SelectionCache
{
private:
    SelectionCache();
    ~SelectionCache();

public:
    static SelectionCache* instance();

    void AddSelectedObject(selection_type selection);
    void Clear();
    std::vector<SelectionData>& GetSelectedObjects();

private:
    std::vector<SelectionData> _selected;
};
