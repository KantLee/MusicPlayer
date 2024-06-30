#include "kdatabaseworker.h"

KDatabaseWorker::KDatabaseWorker(QObject *parent)
	: QObject(parent)
{
}

KDatabaseWorker::~KDatabaseWorker()
{
}

void KDatabaseWorker::getAllMusic()
{
	QVector<KMusic> music = KDataBase::getInstance().getAllMusic();
	emit onAllMusicReady(music);
}
