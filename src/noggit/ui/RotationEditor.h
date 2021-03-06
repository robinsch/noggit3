// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

#include <noggit/Selection.h>

#include <boost/optional.hpp>

#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QWidget>
#include <QDockWidget>

class World;

namespace noggit
{
  namespace ui
  {
    class object_editor;

    class rotation_editor : public QWidget
    {
    public:
      rotation_editor(QWidget* parent, World* world, noggit::ui::object_editor* object_editor_);

      bool* use_median_pivot_point;

      void updateValues(World* world);

      void showEvent(QShowEvent* event_) override;
      void hideEvent(QHideEvent* event_) override;
    private:
        World* _world;
        object_editor* _object_editor;

      // for single selection
      void set_model_rotation(World* world);
      // for multi selection
      void change_models_rotation(World* world);

      QDoubleSpinBox* _rotation_x;
      QDoubleSpinBox* _rotation_z;
      QDoubleSpinBox* _rotation_y;
      QDoubleSpinBox* _position_x;
      QDoubleSpinBox* _position_z;
      QDoubleSpinBox* _position_y;
      QDoubleSpinBox* _scale;
    };
  }
}
