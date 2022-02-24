// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

#include <math/vector_3d.hpp>
#include <noggit/Selection.h>
#include <noggit/bool_toggle_property.hpp>
#include <noggit/history.hpp>
#include <noggit/ModelInstance.h> // ModelInstance
#include <noggit/WMOInstance.h>   // WMOInstance

#include <QLabel>
#include <QWidget>
#include <QSettings>

#include <boost/optional.hpp>

class MapView;
class QButtonGroup;
class World;

namespace noggit
{
  namespace ui
  {
    class model_import;
    class rotation_editor;
    class helper_models;
    class model_list_small;
  }
}

enum ModelPasteMode
{
  PASTE_ON_TERRAIN,
  PASTE_ON_SELECTION,
  PASTE_ON_CAMERA,
  PASTE_MODE_COUNT
};

enum class object_editor_action : std::uint8_t
{
    move,
    remove,
    add
};

struct object_editor_history
{
    object_editor_history(selection_type entry, object_editor_action action_)
    {
        action = action_;
        type = eSelectionEntryTypes(entry.which());

        std::uint32_t uid_ = entry.which() == eEntry_Model
            ? boost::get<selected_model_type>(entry)->uid
            : boost::get<selected_wmo_type>(entry)->mUniqueID
            ;

        math::vector_3d pos_ = entry.which() == eEntry_Model
            ? boost::get<selected_model_type>(entry)->pos
            : boost::get<selected_wmo_type>(entry)->pos
            ;

        math::degrees::vec3 dir_ = entry.which() == eEntry_Model
            ? boost::get<selected_model_type>(entry)->dir
            : boost::get<selected_wmo_type>(entry)->dir
            ;

        std::string filename_ = entry.which() == eEntry_Model
            ? boost::get<selected_model_type>(entry)->model->filename
            : boost::get<selected_wmo_type>(entry)->wmo->filename
            ;

        float scale_ = entry.which() == eEntry_Model
            ? boost::get<selected_model_type>(entry)->scale
            : 1.f
            ;

        filename = filename_;
        scale = scale_;
        uid = uid_;
        dir = dir_;
        pos = pos_;
        undo_once = true;
    }

    object_editor_action action;
    eSelectionEntryTypes type;
    std::string filename;
    float scale;
    std::uint32_t uid;
    math::degrees::vec3 dir;
    math::vector_3d pos;
    bool undo_once;
};

namespace noggit
{
  struct object_paste_params
  {
    float minRotation = 0.f;
    float maxRotation = 360.f;
    float minTilt = -5.f;
    float maxTilt = 5.f;
    float minScale = 0.9f;
    float maxScale = 1.1f;
  };

  namespace ui
  {
    class object_editor : public QWidget
    {
    public:
      object_editor ( MapView*
                    , World*
                    , bool_toggle_property* move_model_to_cursor_position
                    , bool_toggle_property* snap_multi_selection_to_ground
                    , bool_toggle_property* use_median_pivot_point
                    , object_paste_params*
                    , bool_toggle_property* rotate_along_ground
                    , bool_toggle_property* rotate_along_ground_smooth
                    , bool_toggle_property* rotate_along_ground_random
                    , bool_toggle_property* rotate_fixed
                    , QWidget* parent = nullptr
                    );

      ~object_editor();

      void import_last_model_from_wmv(int type);
      void import_last_map();
      void copy(std::string const& filename);
      void copy_global();
      void copy_current_selection(World* world);
      void pasteObject ( math::vector_3d cursor_pos
                       , math::vector_3d camera_pos
                       , World*
                       , object_paste_params*
                       , bool select_pasted_objects
                       );
      void togglePasteMode();

      model_import *modelImport;
      rotation_editor* rotationEditor;
      model_list_small* modelList;
      helper_models* helper_models_widget;
      QSize sizeHint() const override;

      noggit::history<std::vector<object_editor_history>>& get_history() { return _history; }

    private:
      QSettings* _settings;

      QButtonGroup* pasteModeGroup;
      QLabel* _filename;

      bool _copy_model_stats;
      bool _use_median_pivot_point;

      std::vector<selection_type> selected;
      std::vector<selection_type> _model_instance_created;
      
      void replace_selection(std::vector<selection_type> new_selection);

      void showImportModels();
      void SaveObjecttoTXT (World*);
      int pasteMode;

      noggit::history<std::vector<object_editor_history>> _history;
    };
  }
}
