#include "itemdelegate.h"

ComboDelegate::ComboDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{
    mapItems.insert(0,"int8");
    mapItems.insert(1,"uint8");
    mapItems.insert(2,"int16");
    mapItems.insert(3,"uint16");
    mapItems.insert(4,"int32");
    mapItems.insert(5,"uint32");
}

void ComboDelegate::setItems(QMap<int, QString> items)
{
    //mapItems = items;
}

QWidget *ComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    QMap<int,QString>::const_iterator it = mapItems.begin();
    while(it!=mapItems.end()){
        editor->addItem(it.value(),it.key());
        ++it;
    }
    //editor->installEventFilter(const_cast<ComboDelegate*>(this));
    return editor;
}

void ComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant data = index.model()->data(index,Qt::EditRole);

    QComboBox *box = static_cast<QComboBox*>(editor);
    if(box != nullptr){
        int i=box->findData(data);
        box->setCurrentIndex(i);
    }
}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *box = qobject_cast<QComboBox*>(editor);
    if (box !=nullptr){
        QVariant data = box->itemData(box->findText(box->currentText()));
        model->setData(index,data,Qt::EditRole);
    }
}

void ComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
