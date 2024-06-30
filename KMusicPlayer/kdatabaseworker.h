// -----------------------------------------------------------
// kdatabaseworker.h
// �����ߣ� �
// ����ʱ�䣺 2024/06/05
// ���������� �첽�������ݿ�ĺ�ʱ����
// -----------------------------------------------------------

#ifndef _KMUSICPLAYER_KDATABASEWORKER_H_
#define _KMUSICPLAYER_KDATABASEWORKER_H_

#include <QObject>
#include "kdatabase.h"

class KDatabaseWorker : public QObject
{
	Q_OBJECT

public:
	explicit KDatabaseWorker(QObject *parent = Q_NULLPTR);
	~KDatabaseWorker();
	// �첽��ѯ���Ը�����Ϣ
	void getAllMusic();

signals:
	void onAllMusicReady(QVector<KMusic> music);
};

#endif // _KMUSICPLAYER_KDATABASEWORKER_H_
