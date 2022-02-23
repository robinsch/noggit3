// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#include <noggit/ui/model_list_small.h>

#include <noggit/ui/font_awesome.hpp>
#include <noggit/ui/TexturingGUI.h>
#include <noggit/ui/CurrentTexture.h>
#include <noggit/Selection.h>
#include <noggit/World.h>
#include <noggit/ModelInstance.h> // ModelInstance
#include <noggit/WMOInstance.h>   // WMOInstance

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QApplication>
#include <QtGui/QDropEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDrag>
#include <QMimeData>

#include <unordered_set>
#include <string>
#include <algorithm>

#include <boost/optional.hpp>

namespace noggit
{
    namespace ui
    {
        ModelList::ModelList(QWidget* parent) : QListWidget(parent)
        {
            setIconSize(QSize(100, 100));
            setViewMode(QListWidget::IconMode);
            setFlow(QListWidget::LeftToRight);
            setWrapping(false);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setSelectionMode(QAbstractItemView::SingleSelection);
            setAcceptDrops(false);
        }

        void ModelList::mousePressEvent(QMouseEvent* event)
        {
            if (event->button() == Qt::LeftButton)
                _start_pos = event->pos();

            QListWidget::mousePressEvent(event);
        }

        model_list_small::model_list_small(QWidget* parent, World* world)
            : widget(parent)
            , _world(world)
            , layout(new ::QGridLayout(this)
            )
        {
            setWindowTitle("Model Spawn List");
            setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
            setMinimumSize(100, 100);

            _model_list = new ModelList(this);
            //_model_list->setGridSize(QSize(0, sizeHint().height()));

            layout->addWidget(_model_list, 0, 0);

            connect(_model_list, &QListWidget::itemDoubleClicked
                , this
                , [=](QListWidgetItem* item)
                {
                    emit selected("tileset/" + item->toolTip().toStdString());
                }
            );

            updateWidget();

        }
        void model_list_small::updateWidget()
        {
            setFixedSize(QSize(std::max<int>(static_cast<int>(170 * 3), 65 + (106 * 1)), 132));
        }

        void model_list_small::showEvent(QShowEvent* event_)
        {
            if (_world->has_multiple_model_selected())
                return;

            auto entry = _world->get_last_selected_model();
            if (entry)
            {
                selection_type selection = entry.get();

                std::string filename = selection.which() == eEntry_Model
                    ? boost::get<selected_model_type>(selection)->model->filename
                    : boost::get<selected_wmo_type>(selection)->wmo->filename
                    ;

                for (auto& model : _world->get_models_by_filename(filename))
                {
                    QListWidgetItem* list_item = new QListWidgetItem(_model_list);
                    list_item->setText(QString("ID %1, server: (%2, %3, %4)")
                        .arg(model->uid)
                        .arg(ZEROPOINT - model->pos.z)
                        .arg(ZEROPOINT - model->pos.x)
                        .arg(model->pos.y)
                        );
                    list_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

                    updateWidget();
                    _model_list->addItem(list_item);
                }

                for (auto& model : _world->get_wmos_by_filename(filename))
                {
                    QListWidgetItem* list_item = new QListWidgetItem(_model_list);
                    list_item->setText(QString("ID %1, server: (%2, %3, %4)")
                        .arg(model->mUniqueID)
                        .arg(ZEROPOINT - model->pos.z)
                        .arg(ZEROPOINT - model->pos.x)
                        .arg(model->pos.y)
                    );
                    list_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

                    updateWidget();
                    _model_list->addItem(list_item);
                }
            }
        }

        void model_list_small::hideEvent(QHideEvent* event_)
        {
        }
    }
}
