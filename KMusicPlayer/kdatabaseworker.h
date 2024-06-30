// -----------------------------------------------------------
// kdatabaseworker.h
// 创建者： 李康
// 创建时间： 2024/06/05
// 功能描述： 异步进行数据库的耗时操作
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
	// 异步查询所以歌曲信息
	void getAllMusic();

signals:
	void onAllMusicReady(QVector<KMusic> music);
};

#endif // _KMUSICPLAYER_KDATABASEWORKER_H_
