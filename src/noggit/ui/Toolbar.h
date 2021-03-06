// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

#include <functional>

#include <QtWidgets/QActionGroup>
#include <QtWidgets/QToolBar>

#include <noggit/tool_enums.hpp>
#include <noggit/ui/font_awesome.hpp>

namespace noggit
{
  namespace ui
  {
    class toolbar: public QToolBar
    {
    public:
      toolbar(std::function<void (editing_mode)> set_editing_mode, std::function<void()> history_undo, std::function<void()> history_redo);

      void check_tool(editing_mode);

    private:
      std::function<void (editing_mode)> _set_editing_mode;
      std::function<void()> _history_undo;
      std::function<void()> _history_redo;

      QActionGroup _tool_group;

      void add_tool_icon(editing_mode mode, const QString& name, const font_awesome::icons& icon);

      void add_undo_icon(const QString& name, const font_awesome::icons& icon);
      void add_redo_icon(const QString& name, const font_awesome::icons& icon);
    };
  }
}
