/*-------------------------------------------------------
|                                                       |
|                  FEUISimpleHexTable.cpp               |
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
Begin        		:	11.12.2006

Version       		:	01.00.00 / 04.01.2007 / Benjamin Stadin

Operation Systems	:	Linux

Known limitations: updateAscii() changes the data section for '.' from non-printable data entries to
ascii '.' data when block is changed by user.

*/

#include <qheaderview.h>
#include <qtextcursor.h>
#include "FEUISimpleHexTable.h"
#include <QMessageBox>

//------------------------------------------------------------------------------
// Name: FEHexItemEditor::FEHexItemEditor((QWidget *parent)
// Desc: constructor for the editor items within the table
//------------------------------------------------------------------------------
FEHexItemEditor::FEHexItemEditor(QWidget *parent)
	: QTextEdit(parent), m_bOnlyHex(false), rowIndex(0) {}
	
//------------------------------------------------------------------------------
// Name: FEHexItemEditor::keyPressEvent(QKeyEvent *event) 
// Desc: handles key events in hex editor
//------------------------------------------------------------------------------
void FEHexItemEditor::keyPressEvent(QKeyEvent *event) 
{
	QString s = toPlainText();
	QTextCursor cursor = textCursor();
	
	event->accept();
	
	// hex data field
	if (m_bOnlyHex && isHex(event->key()) && cursor.position() < s.length())
	{
		if (s[cursor.position()] == ' ')
			cursor.movePosition(QTextCursor::Right);
		setTextCursor(cursor);
		QTextEdit::keyPressEvent(event);
	}
	// ascii field
	else if (!m_bOnlyHex && cursor.position() < s.length() && !event->modifiers() 
			&& event->key() < 256 && isprint(event->key()))
	{
		QTextEdit::keyPressEvent(event);
	}
	else
	{
		if (!event->modifiers())
		{
			switch(event->key()) 
			{
			case Qt::Key_Home:
				QTextEdit::keyPressEvent(event);
				break;
			case Qt::Key_End:
				QTextEdit::keyPressEvent(event);
				break;
			case Qt::Key_Right:
				if (!m_bOnlyHex && s[cursor.position()+1] == ' ')
					cursor.movePosition(QTextCursor::Right);
				setTextCursor(cursor);
				QTextEdit::keyPressEvent(event);
				break;
			case Qt::Key_Left:
				if (!m_bOnlyHex && s[cursor.position()-1] == ' ')
					cursor.movePosition(QTextCursor::Left);
				setTextCursor(cursor);
				QTextEdit::keyPressEvent(event);
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: FEHexItemEditor::setAllowOnlyHexKeys(bool onlyHex)
// Desc: tell the edit item if it should only print hex characters (0..9, a..f)
//------------------------------------------------------------------------------
void FEHexItemEditor::setAllowOnlyHexKeys(bool onlyHex)
{
	m_bOnlyHex = onlyHex;
}

//------------------------------------------------------------------------------
// Name: FEHexItemEditor::isHex(int cSign)
// Desc: checks if a key is a printable hex character (0..9, a..f)
//------------------------------------------------------------------------------
bool FEHexItemEditor::isHex(int cSign)
{
    if ((cSign > 47) && (cSign < 58))
        return true;
    if ((cSign > 64) && (cSign < 71))
        return true;
    if ((cSign > 96) && (cSign < 104))
        return true;
    
    return false;
}

// we don't want't text to be inserted / deleted via drag 'n drop or text selection
void FEHexItemEditor::dropEvent(QDropEvent *e)
{
	e->accept();
}

void FEHexItemEditor::mousePressEvent(QMouseEvent *e)
{
	e->accept();
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTableDelegate::FEUISimpleHexTableDelegate(QObject *parent)
// Desc: constructor. The delegater handles the FEHexItemEditors of the
//	 currently visible table items
//------------------------------------------------------------------------------
FEUISimpleHexTableDelegate::FEUISimpleHexTableDelegate(QObject *parent)
    : QItemDelegate(parent), isResolving(false) {}

QWidget *FEUISimpleHexTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&,
    const QModelIndex &index) const
{
	FEHexItemEditor *editor = new FEHexItemEditor(parent);
	
	editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	if(index.column() == 1) // hex
	{
		editor->setReadOnly(false);
		editor->setOverwriteMode(true);
		editor->setAllowOnlyHexKeys(true);
	}
	else if (index.column() == 2) // ascii
	{
		editor->setReadOnly(false);
		editor->setOverwriteMode(true);
		editor->setAllowOnlyHexKeys(false);
	}
	else
	{
		editor->setReadOnly(true);	
	}

	return editor;
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
// Desc: sets the data (which is stored in the delegator) to the edited table item
//------------------------------------------------------------------------------
void FEUISimpleHexTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    FEHexItemEditor *edit = qobject_cast<FEHexItemEditor *>(editor);
    if (edit) 
    {
        edit->setHtml(index.model()->data(index, Qt::EditRole).toString());
    }
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTableDelegate::setModelData(...)
// Desc: stores data of a table item to the internal storage of the delegator
//------------------------------------------------------------------------------
void FEUISimpleHexTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
    const QModelIndex &index) const
{
    FEHexItemEditor *edit = qobject_cast<FEHexItemEditor *>(editor);
    if (edit) 
    {
    	// string contains ie "<font color = \"red\">foo..</font>
        model->setData(index, edit->toHtml());
    }
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTableItem::FEUISimpleHexTableItem()
// Desc: constructor
//------------------------------------------------------------------------------
FEUISimpleHexTableItem::FEUISimpleHexTableItem()
    : QTableWidgetItem(), m_bSecurityArea(false), isResolving(false) {}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTableItem::FEUISimpleHexTableItem()
// Desc: overloaded constructor, sets initial text to the item
//------------------------------------------------------------------------------
FEUISimpleHexTableItem::FEUISimpleHexTableItem(const QString &text)
    : QTableWidgetItem(text), m_bSecurityArea(false), isResolving(false) {}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTableItem::clone()
// Desc: creates a new item and copies it's content
//------------------------------------------------------------------------------
QTableWidgetItem *FEUISimpleHexTableItem::clone() const
{
    FEUISimpleHexTableItem *item = new FEUISimpleHexTableItem();
    *item = *this;
    return item;
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTableItem::display()
// Desc: returns the text of a table item
// NOTE: table items that are not currently edited are normal QTableWidgetItems. 
//	 QTableWidgetItems are *simply text* items. 
//	 When an item is edited an FEHexItemEditor, which is a html capapble editor,
//	 is created and the item role is EditRole. 
//	 ==> So internally html text is stored, 
//	 but only plain text is displayed in normal view mode. That's why the conversion is done here
//	 to show normal ascii text instead of html source code
//------------------------------------------------------------------------------
QVariant FEUISimpleHexTableItem::display() const
{
	QTextDocument textDoc;
	QString modelText;
	
	modelText = hexString();
	textDoc.setHtml(modelText);
	return textDoc.toPlainText();
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTableItem::role(int role)
// Desc: returns data for either the normal table item text (in view mode), or
//	 html data for the edit item in edit mode.
//	 In view mode the text color is also set, in edit mode the html data is used
//	 for text the color
//------------------------------------------------------------------------------
QVariant FEUISimpleHexTableItem::data(int role) const
{
    // normal hext data
    if (role == Qt::DisplayRole)
        return display();

    // hex data of security region
    //if (role == Qt::EditRole)
    if (role == Qt::TextColorRole) 
    {
    	if (m_bSecurityArea)
        	return qVariantFromValue(QColor(Qt::red));
    }
 
    if (role == Qt::EditRole)
    {
	return hexString();
    }

    return QTableWidgetItem::data(role);
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTableItem::setData(int role, const QVariant &value)
// Desc: creates a new item and copies it's content
//------------------------------------------------------------------------------
void FEUISimpleHexTableItem::setData(int role, const QVariant &value)
{
    QTableWidgetItem::setData(role, value);
    if (tableWidget())
        tableWidget()->viewport()->update();
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTableItem::setSecurityArea(bool isSecure)
// Desc: if set to true the color of the block will be red
//------------------------------------------------------------------------------
void FEUISimpleHexTableItem::setSecurityArea(bool isSecure)
{
	m_bSecurityArea = isSecure;
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::hexToChar(char *str)
// Desc: returns an ascii char for two hex chars (0..9, a..f)
//------------------------------------------------------------------------------
char FEUISimpleHexTable::hexToChar(char *str)
{
	int iValue	= 0;

	str[0] = (unsigned char) toupper((int) str[0]);
	str[1] = (unsigned char) toupper((int) str[1]);

	if ((str[0] >= '0') && (str[0] <= '9'))	{ iValue  = (str[0] - 48) << 4; }
	if ((str[0] >= 'A') && (str[0] <= 'F'))	{ iValue  = (str[0] - 55) << 4; }
	if ((str[1] >= '0') && (str[1] <= '9'))	{ iValue += (str[1] - 48);	}
	if ((str[1] >= 'A') && (str[1] <= 'F'))	{ iValue += (str[1] - 55);	}
	
	if (isprint(char(iValue)))
		return char(iValue);
	else
		return '.';
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::updateAscii(QTableWidgetItem* item)
// Desc: updates the ascii text if the hex text has changed and vice versa
// NOTE: Known limitation is that non-printable chars (which are displayed as dots '.') 
// 	 are interpreted as printable characters
//------------------------------------------------------------------------------
void FEUISimpleHexTable::updateAscii(QTableWidgetItem* item)
{
	QTableWidgetItem *editor = NULL;
	 if (item && item == currentItem()) 
	 {
		if (column(item) == 1)
		{
			editor = (QTableWidgetItem*)this->item(row(item),2);
			if (editor != NULL)
			{
				QString str = "";
				char* s = qstrdup(item->text().toLatin1());
				char outStr[64] = {0};
				removeBlanks(outStr, s, strlen(s));
				for (int i=0; i<strlen(outStr); i+=2)
				{
					str += hexToChar((char*)outStr+i);
				}
				editor->setText(str);
				delete s;
			}
		}
		if (column(item) == 2)
		{
			editor = (QTableWidgetItem*)this->item(row(item),1);
			if (editor != NULL)
			{
				char* s = qstrdup(item->text().toLatin1());
				char str[64] = {0};
				char* outIter = str;
				for (int i=0; i<strlen(s); i++) //?
				{
					char sHex[3] = {0};
					charToHex(s[i], sHex);
					*(outIter++) = sHex[0];
					*(outIter++) = sHex[1];
				}
				char outStr[128] = {0};
				addSpaces(outStr, str, strlen(str));
				editor->setText(outStr);
				delete s;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::charToHex(char a, char *str)
// Desc: converts an ascii character to two printable hex characters
//------------------------------------------------------------------------------
void FEUISimpleHexTable::charToHex(char a, char *str)	
{
	const char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
	                      '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	sprintf(str, "%c%c",hex[((unsigned) a/16) & 0x0F], hex[(unsigned) a & 0x0F]);

	str[2] = 0;
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::addSpaces(char* dest, char* src, int srcLen)
// Desc: adds spaces to the hex string, so that the string is splited at every second hex char
//------------------------------------------------------------------------------
void FEUISimpleHexTable::addSpaces(char* dest, char* src, int srcLen)
{
	// adds spaces to the hex string
	char* destIter;
	
	destIter = dest;
	for (int iCnt=0; iCnt < srcLen; iCnt++)
	{
		*(destIter++) = src[iCnt];
		if ((iCnt+1) % 2 == 0 && iCnt < srcLen -2) // don't blank firs/last
			*(destIter++) = ' '; 
	}
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::removeBlanks(char* dest, char* src, int srcLen)
// Desc: removes blanks in from hex string
//------------------------------------------------------------------------------
void FEUISimpleHexTable::removeBlanks(char* dest, char* src, int srcLen)
{
	// removes spaces to the hex string
	char* destIter;
	destIter = dest;
	for (int iCnt=0; iCnt < srcLen; iCnt++)
	{
		if (src[iCnt] != ' ')
 			*(destIter++) = src[iCnt];
	}
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::initTable(int size, int blockSize)
// Desc: initializes the table for a given size and block length and fills it with 0
//------------------------------------------------------------------------------
void FEUISimpleHexTable::initTable(int size, int blockSize)
{
	clearContents();
	m_iBlockSize = blockSize;
	setRowCount(size);
	unsigned char ucHex[blockSize];
	memset(ucHex, 0, blockSize);
	for (int i=0; i<size; i++)
		edit(i, ucHex, false);

}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::convUcharToAscii(char* dest, unsigned char* src, int srcLen)
// Desc: converts an uchar array to an array of printable chars
//------------------------------------------------------------------------------
void FEUISimpleHexTable::convUcharToAscii(char* dest, unsigned char* src, int srcLen)
{
	for (int i=0; i<srcLen; i++)
	{
		if (isprint(src[i]))
			dest[i] = src[i];
		else
			dest[i] = '.';
	}
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::edit(int record, unsigned char* data, bool securityArea)
// Desc: edit a data block. If securityArea is set to true the block is red coloured
//------------------------------------------------------------------------------
int FEUISimpleHexTable::edit(int record, unsigned char* data, bool securityArea)
{
	FEUISimpleHexTableItem* hexItem ;
	FEUISimpleHexTableItem*	dbItem;
	FEUISimpleHexTableItem*	asciiItem;
	
	if (record >= rowCount())
		return -1;
		
	//		db number field
	char sDbn[8] = {0};
	sprintf(sDbn, "%d", record);
	dbItem = (FEUISimpleHexTableItem*)item(record, 0);
	if (!dbItem)
		dbItem = new FEUISimpleHexTableItem(sDbn);
	else
	{	
		dbItem->setText(sDbn);
	}
	dbItem->setFont(font());
	setItem(record, 0, dbItem);
		
	//		hex data field
	// convert to char
	int iHexLen = 2 * m_iBlockSize + 2;
	char sHexData[iHexLen];
	memset(sHexData, 0, iHexLen);
	FEDM_ConvHexUCharToHexChar(data, m_iBlockSize, sHexData, iHexLen);
	// create array large enough for text with spaces
	int iDataLen = 2 * m_iBlockSize + m_iBlockSize / 2 + 2; // 2 because block size may not be even
	char sData[iDataLen];
	memset(sData, 0, iDataLen);
	// add spaces
	addSpaces(sData, sHexData, iDataLen);
	// set text for the hex data column
	hexItem = (FEUISimpleHexTableItem*)item(record, 1);
	if (!hexItem)
	{
		hexItem = new FEUISimpleHexTableItem();
		setItem(record, 1, hexItem);
	}
	hexItem->setSecurityArea(securityArea);
	hexItem->setFont(font());
	// red coloured 
	QString text;
	if (securityArea)
	{
		text = "<font color=\"red\">";
		text += sData;
		text += "</font>";
	}
	else
		text = sData;
	hexItem->setText(text);
	
	// 		ASCII field
	int iAsciiLen = m_iBlockSize + 1;
	char sAscii[iAsciiLen];
	memset(sAscii, 0, iAsciiLen);
	convUcharToAscii(sAscii, data, m_iBlockSize);
	asciiItem = (FEUISimpleHexTableItem*)item(record, 2);
	if (!asciiItem)
		asciiItem = new FEUISimpleHexTableItem(sAscii);
	else
		asciiItem->setText(sAscii);
	asciiItem->setFont(font());
	setItem(record, 2, asciiItem);
	
	return 0;	
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::FEUISimpleHexTable(QWidget *parent)
// Desc: constructor
//------------------------------------------------------------------------------
FEUISimpleHexTable::FEUISimpleHexTable(QWidget *parent)
    : QTableWidget(parent)
{	
	m_iBlockSize = 0;
	
	//table = new QTableWidget(0, 3, this );
	QFont fnt;
	fnt.setFixedPitch(true);
	fnt.setStyleHint(QFont::System);
	// fixed width font
	fnt.setFamily("Misc Fixed");
	setFont(fnt);
	
	this->insertColumn(0);
	this->insertColumn(1);
	this->insertColumn(2);
	
	QTableWidgetItem* dbItem = new QTableWidgetItem("DB");
	setHorizontalHeaderItem(0, dbItem);
	QTableWidgetItem* hexItem = new QTableWidgetItem("HEX DATA");
	setHorizontalHeaderItem(1, hexItem);
	QTableWidgetItem* asciiItem = new QTableWidgetItem("ASCII");
	setHorizontalHeaderItem(2, asciiItem);
	setItemPrototype(item(0, 1));
	setItemDelegate(new FEUISimpleHexTableDelegate());
	#if !defined(QT_NO_DBUS) && defined(Q_OS_UNIX)
	new FEUISimpleHexTableAdaptor(this);
	#endif
	// hide the column index on the left
	QHeaderView *h = verticalHeader();
	h->hide();
	// stretch the horizontal index header to fill the complete widget
	h = horizontalHeader();
	h->stretchLastSection();
	createActions();
	setupContextMenu();
	
    	connect(this, SIGNAL(itemChanged(QTableWidgetItem*)),
        this, SLOT(updateAscii(QTableWidgetItem*)));
	
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::~FEUISimpleHexTable(QWidget *parent)
// Desc: destructor
//------------------------------------------------------------------------------
FEUISimpleHexTable::~FEUISimpleHexTable()
{
//
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::getHexString(char* str, int row)
// Desc: get a hex string representation of a given table row (hex data block)
//------------------------------------------------------------------------------
void FEUISimpleHexTable::getHexString(char* str, int row)
{
	QTableWidgetItem *editor;
	QString hexStr = "";

	editor = item(row, 1);
	if (editor != NULL)
	{
		hexStr += editor->text();
	}
	char* s = qstrdup(hexStr.toLatin1());
	removeBlanks(str, s, strlen(s));
	delete s;
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::createActions()
// Desc: creates actions for the hex table ui
//------------------------------------------------------------------------------
void FEUISimpleHexTable::createActions()
{
    clearAction = new QAction(tr("Clear"), this);
    clearAction->setShortcut(Qt::Key_Delete);
    connect(clearAction, SIGNAL(triggered()), this, SLOT(clear()));
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::clear()
// Desc: clears the table
//------------------------------------------------------------------------------
void FEUISimpleHexTable::clear()
{
	clearContents();
	setRowCount(0);
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::setupContextMenu()
// Desc: sets the contex menu for the table (right mouse click)
//------------------------------------------------------------------------------
void FEUISimpleHexTable::setupContextMenu()
{
    addAction(clearAction);
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

//------------------------------------------------------------------------------
// Name: FEUISimpleHexTable::resizeEvent(QResizeEvent *event)
// Desc: triggered when the window is resized
//------------------------------------------------------------------------------
void FEUISimpleHexTable::resizeEvent(QResizeEvent *event)
{
	/*event->accept();
	
	resize(event->size());*/
	int blockSize;
	blockSize = m_iBlockSize > 0 ? m_iBlockSize : 12;
	QHeaderView *h = horizontalHeader();
	h->stretchLastSection();
	h->resizeSection(0, 40); 
	//h->resizeSection(1, blockSize * 22); 
	h->resizeSection(1, 188); 
	//h->resizeSection(2, blockSize * 8); 
	h->resizeSection(2, 110); 
}
