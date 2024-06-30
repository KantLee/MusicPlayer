#include "kdatabase.h"

KDataBase& KDataBase::getInstance()
{
	static KDataBase instance;
	return instance;
}

bool KDataBase::initSqlite()
{
	QSqlDatabase database;
	if (QSqlDatabase::contains("qt_sql_default_connection"))
	{
		database = QSqlDatabase::database("qt_sql_default_connection");
	}
	else
	{
		database = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
		database.setDatabaseName(QCoreApplication::applicationDirPath() + "/KMusicPlayer.db");
		database.setUserName("root");
		database.setPassword("root");
		if (!database.open())
			return false;
	}
	QSqlQuery query;
	// 设置WAL模式
	if (!query.exec("PRAGMA journal_mode=WAL;"))
		return false;
	// 检查表是否存在，不存在则创建
	query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg("MusicInfo"));
	if (query.next())
	{
		if (query.value(0).toInt() == 0)
		{
			QSqlQuery sql_query;
			// id，歌名，作者，歌曲位置，歌词位置，封面位置（没有找到内嵌封面时使用），时常，专辑名，比特率
			QString create_sql = "create table MusicInfo (ID INTEGER PRIMARY KEY AUTOINCREMENT"
				", MusicName TEXT NOT NULL"
				", Author TEXT"
				", MusicURL TEXT UNIQUE NOT NULL"
				", LyricURL TEXT"
				", ImgURL TEXT"
				", Duration BIGINT"
				", AlbumName TEXT"
				", AudioBitRate INTEGER)";
			sql_query.prepare(create_sql);
			sql_query.exec();
		}
		return true;
	}
	return false;
}

void KDataBase::closeSqlite()
{
	QSqlDatabase database = QSqlDatabase::database("qt_sql_default_connection", false);
	if (database.isOpen())
		database.close();
}

void KDataBase::addMusic(KMusic &music)
{
	QSqlQuery sql_query;
	QString insert_sql = "INSERT INTO MusicInfo (MusicName, Author, MusicURL, "
		"LyricURL, ImgURL, Duration, AlbumName, AudioBitRate) "
		"VALUES (:musciName, :author, :musicUrl, :lyricUrl, :imgUrl, :duration, :albumName, :audioBitRate)";
	sql_query.prepare(insert_sql);
	sql_query.bindValue(":musciName", music.getMusicName());
	sql_query.bindValue(":author", music.getAuthor());
	sql_query.bindValue(":musicUrl", music.getLocalUrl().toString());
	sql_query.bindValue(":lyricUrl", music.getLyricUrl().toString());
	sql_query.bindValue(":imgUrl", music.getImgUrl().toString());
	sql_query.bindValue(":duration", music.getDuration());
	sql_query.bindValue(":albumName", music.getAlbumTitle());
	sql_query.bindValue(":audioBitRate", music.getAudioBitRate());
	sql_query.exec();
}

QVector<KMusic> KDataBase::getAllMusic()
{
	QSqlQuery sql_query;
	QString select_sql = "SELECT MusicURL, Author, MusicName, Duration, "
		"AlbumName, AudioBitRate, LyricURL, ImgURL FROM MusicInfo";
	sql_query.prepare(select_sql);
	sql_query.exec();
	QVector<KMusic> music;
	while (sql_query.next())
	{
		KMusic tempMusic;
		tempMusic.setLocalUrl(QUrl(sql_query.value(0).toString()));
		tempMusic.setAuthor(sql_query.value(1).toString());
		tempMusic.setMusicName(sql_query.value(2).toString());
		tempMusic.setDuration(sql_query.value(3).toLongLong());
		tempMusic.setAlbumTitle(sql_query.value(4).toString());
		tempMusic.setAudioBitRate(sql_query.value(5).toInt());
		tempMusic.setLyricUrl(sql_query.value(6).toString());
		tempMusic.setImgUrl(sql_query.value(7).toString());
		music.push_back(tempMusic);
	}
	return music;
}

void KDataBase::removeMusic(QUrl url)
{
	QString query = "DELETE FROM MusicInfo WHERE MusicURL = :MusicURL";
	QSqlQuery sql_query;
	sql_query.prepare(query);
	sql_query.bindValue(":MusicURL", url);
	sql_query.exec();
}

void KDataBase::removeAllMusic()
{
	QSqlQuery sql_query;
	QString delete_sql = "delete from MusicInfo";
	sql_query.prepare(delete_sql);
	sql_query.exec();
}

bool KDataBase::existMusic(QString urlStr)
{
	QUrl url = QUrl::fromLocalFile(urlStr);
	QString query = "SELECT COUNT(*) FROM MusicInfo WHERE MusicURL = :MusicURL";
	QSqlQuery sql_query;
	sql_query.prepare(query);
	sql_query.bindValue(":MusicURL", url);
	sql_query.exec();
	if (sql_query.next())
		return sql_query.value(0).toInt() > 0;
}
