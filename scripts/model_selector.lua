-- This file is part of Noggit3, licensed under GNU General Public License (version 3).
local model_selector = brush("Model Selector");

function model_selector:on_left_hold(evt)
    local sel = select_origin(evt:pos(), evt:outer_radius(), evt:outer_radius())
    for i,model in pairs(sel:models()) do
        model:select()
    end

    sel:apply()
end