#ifndef FEUISIMPLEHEX_H
#define FEUISIMPLEHEX_H

#include <QtGui>
#include "../../../../fedm-classlib/src/FedmIscCore.h"

class FEHexItemEditor : public QTextEdit
{
    Q_OBJECT
public:
	FEHexItemEditor(QWidget *parent = 0);
	void setAllowOnlyHexKeys(bool hexOnly);
protected:
	virtual void keyPressEvent(QKeyEvent * event);
	virtual void dropEvent(QDropEvent *e);
	virtual void mousePressEvent(QMouseEvent *e);
private:
	bool m_bOnlyHex;
	int  rowIndex;
	bool isHex(int cSign);
};

class FEUISimpleHexTableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
	FEUISimpleHexTableDelegate(QObject *parent = 0);
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
		const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const;

private:
	mutable bool isResolving;
};

class FEUISimpleHexTableItem : public QTableWidgetItem
{
public:
	FEUISimpleHexTableItem();
	FEUISimpleHexTableItem(const QString &text);
	
	QTableWidgetItem *clone() const;
	
	QVariant data(int role) const;
	void setData(int role, const QVariant &value);
	void setSecurityArea(bool isSecure);
	QVariant display() const;
	inline QString hexString() const
		{ return QTableWidgetItem::data(Qt::DisplayRole).toString(); }

private:
	bool m_bSecurityArea;
	mutable bool isResolving;
};

#if !defined(QT_NO_DBUS) && defined(Q_OS_UNIX)
#include <QtDBus>

class FEUISimpleHexTableAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.feig.DBus.FEUISimpleHexTable")

public:
	FEUISimpleHexTableAdaptor(QTableWidget *table) : QDBusAbstractAdaptor(table), table(table)
	{ QDBusConnection::sessionBus().registerObject("/FEUISimpleHexTable", table); }

public slots:
private:
    QTableWidget *table;

};
#endif // QT_NO_DBUS

class FEUISimpleHexTable : public QTableWidget
{
    Q_OBJECT 
public:
	FEUISimpleHexTable(QWidget *parent = 0);
	~FEUISimpleHexTable();
	int edit(int record, unsigned char* data, bool securityArea);
	void initTable(int size, int blockSize);
	void getHexString(char* str, int row);
	void setSize(int size);  

public slots:
	void clear();
	void updateAscii(QTableWidgetItem* item);

protected:
	void setupContextMenu();
	void createActions();
	void resizeEvent(QResizeEvent *event);
	void newRow();

private:
	QAction *clearAction;
	QLabel *cellLabel;
	QTableWidget *table;
	int m_iBlockSize;
	bool isHex(char cSign);
	void addSpaces(char* dest, char* src, int srcLen);
	void removeBlanks(char* dest, char* src, int srcLen);
	void convUcharToAscii(char* dest, unsigned char* src, int srcLen);
	char hexToChar(char *str);
	void charToHex(char a, char *str);
};

#endif
