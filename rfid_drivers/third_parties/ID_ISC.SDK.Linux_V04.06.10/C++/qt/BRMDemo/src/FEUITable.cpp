/*-------------------------------------------------------
|                                                       |
|                  FEUITable.cpp                        |
|                                                       |
---------------------------------------------------------

Copyright  2007		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : info@feig.de
						Internet : http://www.feig.de
					
Author         		:	Benjamin Stadin
Begin        		:	13.12.2006

Version       		:	01.00.00 / 11.01.2007 / Benjamin Stadin

Operation Systems	:	Linux
*/

#include <qheaderview.h>
#include "FEUITable.h"

FEUITableDelegate::FEUITableDelegate(QObject *parent)
    : QItemDelegate(parent) {}

QWidget *FEUITableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&,
    const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setReadOnly(true);

    connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
    return editor;
}

void FEUITableDelegate::commitAndCloseEditor()
{
    QLineEdit *editor = qobject_cast<QLineEdit *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

void FEUITableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
    if (edit) 
    {
        edit->setText(index.model()->data(index, Qt::EditRole).toString());
    }
}

void FEUITableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
    const QModelIndex &index) const
{
    QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
    if (edit) 
    {
        model->setData(index, edit->text());
    }
}

FEUITableItem::FEUITableItem()
    : QTableWidgetItem() {}

FEUITableItem::FEUITableItem(const QString &text)
    : QTableWidgetItem(text) {}

QTableWidgetItem *FEUITableItem::clone() const
{
    FEUITableItem *item = new FEUITableItem();
    *item = *this;
    return item;
}

QVariant FEUITableItem::data(int role) const
{
    if (role == Qt::DisplayRole)
        return display();

    QString t = display().toString();
    bool isNumber = false;
    int number = t.toInt(&isNumber);

    if (role == Qt::TextColorRole) {
        if (!isNumber)
            return qVariantFromValue(QColor(Qt::black));
        else if (number < 0)
            return qVariantFromValue(QColor(Qt::red));
        return qVariantFromValue(QColor(Qt::blue));
    }

    if (role == Qt::TextAlignmentRole)
        if (!t.isEmpty() && (t.at(0).isNumber() || t.at(0) == '-'))
            return (int)(Qt::AlignRight | Qt::AlignVCenter);

    return QTableWidgetItem::data(role);
}

void FEUITableItem::setData(int role, const QVariant &value)
{
    QTableWidgetItem::setData(role, value);
    if (tableWidget())
        tableWidget()->viewport()->update();
}

QVariant FEUITableItem::display() const
{
    return QVariant();
}

void FEUITable::newRow()
{
	// add a row to the table
	int cnt = this->rowCount();
	cnt++;
	this->setRowCount(cnt);
}

int FEUITable::edit(int row, QString colByName, QString text)
{
	int col = -1;
	
	if (row > this->rowCount()-1)
		return -1;
	 
	// Find column by text
	for (int i = 0; i < m_colList.size(); i++) 
	{
		if (colByName == m_colList[i])
		{
			col = i;
			break;
		}
	}
	
	if (col >= 0)
	{
		// set text for the column
		QTableWidgetItem *item = this->item(row, col);
		if (!item)
			this->setItem(row, col, new QTableWidgetItem(text));
		else
			item->setText(text);
		return 0;
	}
	else
		return -1;
}


FEUITable::FEUITable(QStringList columns, QWidget *parent)
    : QTableWidget(parent)
{
	// internal list to map colum names and index
	m_colList = columns;
	int cols = columns.size();
	
	QFont fnt;
	fnt.setFixedPitch(true);
	fnt.setStyleHint(QFont::System);
	// fixed width font
	fnt.setFamily("Misc Fixed");
	setFont(fnt);

	for (int c = 0; c < cols; ++c) 
	{
		this->insertColumn(c);
		QString colName;
		colName = columns[c];
		this->setHorizontalHeaderItem(c, new QTableWidgetItem(colName));
	}
	//setItemPrototype(item(rows - 1, cols - 1));
	this->setItemPrototype(item(1, 1));
	this->setItemDelegate(new FEUITableDelegate());
	#if !defined(QT_NO_DBUS) && defined(Q_OS_UNIX)
	new FEUITableAdaptor(this);
	#endif
	// hide the column index on the left
	QHeaderView *h = this->verticalHeader();
	h->hide();
	// stretch the horizontal index header to fill the complete widget
	h = this->horizontalHeader();
	h->stretchLastSection();
	//ResizeToContents
	h->setResizeMode(QHeaderView::Stretch);
	createActions();
	setupContextMenu();
}

FEUITable::~FEUITable()
{
	//
}

void FEUITable::createActions()
{
    clearAction = new QAction(tr("Clear"), this);
    clearAction->setShortcut(Qt::Key_Delete);
    connect(clearAction, SIGNAL(triggered()), this, SLOT(clear()));
}

void FEUITable::clear()
{
	clearContents();
	setRowCount(0);
}

void FEUITable::setupContextMenu()
{
    addAction(clearAction);
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void FEUITable::resizeEvent(QResizeEvent *event)
{
	/*event->accept(); qt4.2.1
	resize(event->size());
	resize(event->size());*/
	QHeaderView *h = horizontalHeader();
	h->stretchLastSection();
}
