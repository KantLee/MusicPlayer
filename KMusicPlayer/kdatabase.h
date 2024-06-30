// -----------------------------------------------------------
// kdatabase.h
// �����ߣ� �
// ����ʱ�䣺 2024/06/04
// ���������� ����ģʽ�����ݿ������
// -----------------------------------------------------------

#ifndef _KMUSICPLAYER_KDATABASE_H_
#define _KMUSICPLAYER_KDATABASE_H_

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QCoreApplication>
#include <QMessageBox>
#include <QVariant>

#include "kmusic.h"

class KDataBase
{
public:
	static KDataBase& getInstance();
	// ���ݿ��ʼ��
	bool initSqlite();
	// �ر����ݿ�����
	void closeSqlite();
	// �������
	void addMusic(KMusic &music);
	// ��ȡȫ������
	QVector<KMusic> getAllMusic();
	// �Ƴ�ָ������
	void removeMusic(QUrl url);
	// �Ƴ����и���
	void removeAllMusic();
	// ���ĳ�����Ƿ����
	bool existMusic(QString urlStr);

private:
	KDataBase() = default;
	KDataBase(const KDataBase & other) = delete;
	KDataBase& operator=(const KDataBase & other) = delete;
};

#endif // _KMUSICPLAYER_KDATABASE_H_
