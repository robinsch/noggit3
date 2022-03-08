-- This file is part of Noggit3, licensed under GNU General Public License (version 3).
local model_replacer = brush("Model Replacer");

local s_model_name = model_replacer:add_string_tag("Search","")
local r_model_name = model_replacer:add_string_tag("Replace","")

function model_replacer:on_left_hold(evt)
    local sel = select_origin(evt:pos(), evt:outer_radius(), evt:outer_radius())
    for i,model in pairs(sel:models()) do
        if(model:get_filename() == s_model_name:get()) then
            model:replace(r_model_name:get())
        end
    end

    sel:apply()
end