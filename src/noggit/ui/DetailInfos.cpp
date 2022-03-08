// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#include <noggit/ui/DetailInfos.h>

#include <QtWidgets/QFormLayout>
#include <QApplication>
#include <QClipboard.h>

namespace noggit
{
  namespace ui
  {
    detail_infos::detail_infos(QWidget* parent)
      : widget (parent)
    {
      setWindowFlags (Qt::Tool);
      auto layout (new QFormLayout (this));

      layout->addRow (_info_text = new QLabel (this));
      layout->addRow(_copy_button = new QPushButton(this));

      _copy_button->setText("Copy");
      // Connect button signal to appropriate slot
      connect(_copy_button, &QPushButton::released, this, &detail_infos::copyText);
    }

    void detail_infos::setText (const std::string& t)
    {
      _info_text->setText (t.c_str ());
    }

    void detail_infos::copyText()
    {
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(_info_text->text());
    }
  }
}
