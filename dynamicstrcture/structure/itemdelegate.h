#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QStandardItem>
#include <QComboBox>
#include <QMap>
#include <QDebug>

// custom combobox

class ComboDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ComboDelegate(QObject *parent=0);

    void setItems(QMap<int,QString> items);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const  QModelIndex &index) const;

private:
    QMap<int,QString> mapItems;//items
};

#endif // ITEMDELEGATE_H
