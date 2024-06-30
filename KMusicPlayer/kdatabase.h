// -----------------------------------------------------------
// kdatabase.h
// 创建者： 李康
// 创建时间： 2024/06/04
// 功能描述： 单例模式的数据库操作类
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
	// 数据库初始化
	bool initSqlite();
	// 关闭数据库连接
	void closeSqlite();
	// 添加音乐
	void addMusic(KMusic &music);
	// 获取全部音乐
	QVector<KMusic> getAllMusic();
	// 移除指定歌曲
	void removeMusic(QUrl url);
	// 移除所有歌曲
	void removeAllMusic();
	// 检查某歌曲是否存在
	bool existMusic(QString urlStr);

private:
	KDataBase() = default;
	KDataBase(const KDataBase & other) = delete;
	KDataBase& operator=(const KDataBase & other) = delete;
};

#endif // _KMUSICPLAYER_KDATABASE_H_
