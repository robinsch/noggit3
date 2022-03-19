// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#include <noggit/ui/model_list_small.h>

#include <noggit/ui/font_awesome.hpp>
#include <noggit/ui/TexturingGUI.h>
#include <noggit/ui/CurrentTexture.h>
#include <noggit/Selection.h>
#include <noggit/World.h>
#include <noggit/ModelInstance.h> // ModelInstance
#include <noggit/WMOInstance.h>   // WMOInstance
#include <noggit/MapView.h>

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
            setViewMode(QListWidget::ListMode);
            setFlow(QListWidget::TopToBottom);
            setWrapping(false);
            setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
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

        model_list_small::model_list_small(MapView* parent, World* world)
            : widget(parent)
            , _mapView(parent)
            , _world(world)
            , layout(new ::QGridLayout(this)
            )
        {
            setWindowTitle("");
            setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
            setMinimumSize(100, 100);

            _model_list = new ModelList(this);

            layout->addWidget(_model_list, 0, 0);

            connect(_model_list, &QListWidget::itemDoubleClicked
                , this
                , [=](QListWidgetItem* item)
                {
                    auto entry = _world->get_last_selected_model();
                    if (entry)
                    {
                        selection_type selection = entry.get();

                        math::vector_3d model_pos = selection.which() == eEntry_Model
                            ? boost::get<selected_model_type>(selection)->pos
                            : boost::get<selected_wmo_type>(selection)->pos
                            ;

                        uint32_t idx = _model_list->row(item);
                        math::vector_3d pos = _list_item_pos[idx];
                        _mapView->_camera.position = pos + camera_offset;

                        // @robinsch: on click set selection to clicked model instance
                        {
                            auto model = _world->get_model(_list_item_uid[idx]);
                            if (!model)
                                return;

                            selection_type entry = model.get();
                            _world->set_current_selection(entry);
                        }
                    }
                }
            );

            updateWidget();

        }
        void model_list_small::updateWidget()
        {
            setFixedSize(QSize(std::max<int>(static_cast<int>(170 * 3), 65 + (106 * 1)), 132));
        }

        void model_list_small::updateValues()
        {
            _model_list->clear();
            _list_item_pos.clear();
            _list_item_uid.clear();

            if (_world->has_multiple_model_selected())
                return;

            auto entry = _world->get_last_selected_model();
            if (entry)
            {
                selection_type selection = entry.get();

                std::string sel_filename = selection.which() == eEntry_Model
                    ? boost::get<selected_model_type>(selection)->model->filename
                    : boost::get<selected_wmo_type>(selection)->wmo->filename
                    ;

                std::uint32_t sel_uid = selection.which() == eEntry_Model
                    ? boost::get<selected_model_type>(selection)->uid
                    : boost::get<selected_wmo_type>(selection)->mUniqueID
                    ;

                math::vector_3d sel_pos = selection.which() == eEntry_Model
                    ? boost::get<selected_model_type>(selection)->pos
                    : boost::get<selected_wmo_type>(selection)->pos
                    ;

                setWindowTitle(QString("Spawns: ") + QString(sel_filename.c_str()));

                // @robinsch: show selected model first
                {
                    QListWidgetItem* list_item = new QListWidgetItem(_model_list);
                    list_item->setText(QString("** ID %1, pos: (%2, %3, %4) **")
                        .arg(sel_uid)
                        .arg(ZEROPOINT - sel_pos.z)
                        .arg(ZEROPOINT - sel_pos.x)
                        .arg(sel_pos.y)
                    );

                    updateWidget();
                    _model_list->addItem(list_item);
                    _list_item_uid.push_back(sel_uid);
                    _list_item_pos.push_back(sel_pos);
                }

                // @robinsch: store camera offset
                camera_offset = (_mapView->_camera.position - sel_pos);

                for (ModelInstance* model : _world->get_models_by_filename(sel_filename))
                {
                    if (model->pos == sel_pos)
                        continue;

                    QListWidgetItem* list_item = new QListWidgetItem(_model_list);
                    list_item->setText(QString("pos: (%2, %3, %4)")
                        .arg(ZEROPOINT - model->pos.z)
                        .arg(ZEROPOINT - model->pos.x)
                        .arg(model->pos.y)
                    );
                    list_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

                    updateWidget();
                    _model_list->addItem(list_item);
                    _list_item_uid.push_back(model->uid);
                    _list_item_pos.push_back(model->pos);
                }

                for (WMOInstance* wmo : _world->get_wmos_by_filename(sel_filename))
                {
                    if (wmo->pos == sel_pos)
                        continue;

                    QListWidgetItem* list_item = new QListWidgetItem(_model_list);
                    list_item->setText(QString("pos: (%1, %2, %3)")
                        .arg(ZEROPOINT - wmo->pos.z)
                        .arg(ZEROPOINT - wmo->pos.x)
                        .arg(wmo->pos.y)
                    );
                    list_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

                    updateWidget();
                    _model_list->addItem(list_item);
                    _list_item_uid.push_back(wmo->mUniqueID);
                    _list_item_pos.push_back(wmo->pos);
                }
            }
        }

        void model_list_small::showEvent(QShowEvent* event_)
        {
            updateValues();
        }

        void model_list_small::hideEvent(QHideEvent* event_)
        {
            _model_list->clear();
            _list_item_uid.clear();
            _list_item_pos.clear();
        }
    }
}
