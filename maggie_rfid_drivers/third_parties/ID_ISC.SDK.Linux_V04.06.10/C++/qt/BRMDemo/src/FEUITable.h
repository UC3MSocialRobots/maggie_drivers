#include <QtGui>

class FEUITableDelegate : public QItemDelegate
{
    	Q_OBJECT
public:
	FEUITableDelegate(QObject *parent = 0);
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
		const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const;
private slots:
    	void commitAndCloseEditor();
};

class FEUITableItem : public QTableWidgetItem
{
public:
	FEUITableItem();
	FEUITableItem(const QString &text);
	
	QTableWidgetItem *clone() const;
	
	QVariant data(int role) const;
	void setData(int role, const QVariant &value);
	QVariant display() const;
};

#if !defined(QT_NO_DBUS) && defined(Q_OS_UNIX)
#include <QtDBus>

class FEUITableAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.feig.DBus.FEUITable")

public:
	FEUITableAdaptor(QTableWidget *table) : QDBusAbstractAdaptor(table), table(table)
	{ QDBusConnection::sessionBus().registerObject("/FEUITable", table); }

public slots:
private:
    QTableWidget *table;
};
#endif // QT_NO_DBUS

class FEUITable : public QTableWidget
{
   	Q_OBJECT 
public:
	FEUITable(QStringList columns, QWidget *parent = 0);
	~FEUITable();
	int edit(int row, QString colByName, QString text);
	void newRow();

public slots:
    	void clear();

protected:
	void setupContextMenu();
	void createActions();
	void resizeEvent(QResizeEvent *event);

private:
	QAction *clearAction;
	QStringList m_colList;
	QLabel *cellLabel;
};
