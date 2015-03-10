#ifndef FEREADTAGSTHREAD_H
#define FEREADTAGSTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class FEDM_ISCReader;

class FEReadTagsThread : public QThread
{
    Q_OBJECT

public:
    FEReadTagsThread(FEDM_ISCReader* reader, QObject *parent = 0);
    ~FEReadTagsThread();
    void readTags();
    void pauseThread();
    bool running();
    
signals:
    void onNewData(int iStatus);
    void onError(int iStatus);    

protected:
    void run();

private:
    QMutex m_mutex;
    QWaitCondition m_condition;
    bool m_bRunning;
    bool m_bFinish;
    bool m_bWait;
    FEDM_ISCReader* m_iscReader;
};

#endif
