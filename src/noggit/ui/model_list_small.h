// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

#include <noggit/ui/widget.hpp>
#include <QtWidgets/QListWidget>
#include <unordered_set>
#include <string>

class QGridLayout;
class QPushButton;
class QDropEvent;
class QDragEnterEvent;
class QMouseEvent;
class QListWidget;
class QPoint;

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
        model_list_small(QWidget* parent, World* world);

        void updateWidget();

        void showEvent(QShowEvent* event_) override;
        void hideEvent(QHideEvent* event_) override;

    signals:
        void selected(std::string);

    private:
        World* _world;

        QGridLayout* layout;

        QListWidget* _model_list;
        std::unordered_set<std::string> _model_list_entries;
    };
}
}
