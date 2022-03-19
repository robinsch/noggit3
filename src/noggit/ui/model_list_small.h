// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

#include <noggit/ui/widget.hpp>
#include <QtWidgets/QListWidget>
#include <unordered_set>
#include <string>
#include <math/vector_3d.hpp>

class QGridLayout;
class QPushButton;
class QDropEvent;
class QDragEnterEvent;
class QMouseEvent;
class QListWidget;
class QPoint;

class MapView;
class World;

namespace noggit
{
    namespace ui
    {
        class ModelList : public QListWidget
        {
        public:
            ModelList(QWidget* parent);

            void mousePressEvent(QMouseEvent* event) override;

        private:
            QPoint _start_pos;
        };

        class model_list_small : public widget
        {
            Q_OBJECT

        public:
            model_list_small(MapView* parent, World* world);

            void updateWidget();
            void updateValues();

            void showEvent(QShowEvent* event_) override;
            void hideEvent(QHideEvent* event_) override;

        signals:
            void selected(std::string);

        private:
            MapView* _mapView;
            World* _world;

            QGridLayout* layout;

            QListWidget* _model_list;
            std::vector<uint32_t> _list_item_uid;
            std::vector<math::vector_3d> _list_item_pos;

            math::vector_3d camera_offset;
        };
    }
}
