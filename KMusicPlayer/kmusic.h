// -----------------------------------------------------------
// kmusic.h
// 创建者： 李康
// 创建时间： 2024/05/31
// 功能描述： 歌曲元数据类
// -----------------------------------------------------------

#ifndef _KMUSICPLAYER_KMUSIC_H_
#define _KMUSICPLAYER_KMUSIC_H_

#include <QString>
#include <QUrl>
#include <QMediaPlayer>
#include <QCoreApplication>
#include <QMessageBox>
#include <QtSql>

class KMusic
{
public:
	KMusic() :m_duration(0), m_audioBitRate(0) {}
	KMusic(QUrl url);
	// 返回歌曲Url
	QUrl getUrl() const;
	// 返回歌曲的信息
	QString getInfo();
	// 弹窗显示歌曲信息
	void detail();
	// 存入数据库
	void insertSQL();
	// 根据文件名来获取歌词路径
	QString getLyricFile();
	// 根据歌曲url获得歌曲信息
	void refreshInfo();
	// 设置本地歌词地址(添加本地音乐使用)
	void setLyricUrl();
	// 设置本地歌词地址(添加在线音乐使用)
	void setLyricUrl(QString path);
	// 获取本地歌词地址
	QUrl getLyricUrl();
	// 设置本地封面地址(添加本地音乐使用)
	void setImgUrl();
	// 设置本地封面地址(添加在线音乐使用)
	void setImgUrl(QString path);
	// 获取本地封面地址
	QUrl getImgUrl();
	// 设置歌曲网络下载地址
	void setOnlineMusicUrl(QUrl url);
	// 设置歌词网络下载地址
	void setOnlineLyricUrl(QUrl url);
	// 设置封面网络下载地址
	void setOnlineImgUrl(QUrl url);
	// 获取歌曲网络下载地址
	QUrl getOnlineMusicUrl();
	// 获取歌词网络下载地址
	QUrl getOnlineLyricUrl();
	// 获取封面网络下载地址
	QUrl getOnlineImgUrl();
	// 获取歌曲名
	QString getMusicName();
	// 设置歌曲名
	void setMusicName(QString name);
	// 获取歌曲本地地址
	QUrl getLocalUrl();
	// 设置歌曲本地地址
	void setLocalUrl(QUrl url);
	// 获取歌手名
	QString getAuthor();
	// 设置歌手名
	void setAuthor(QString name);
	// 获取时常
	qint64 getDuration();
	// 设置时常
	void setDuration(qint64 duration);
	// 获取专辑名
	QString getAlbumTitle();
	// 设置专辑名
	void setAlbumTitle(QString name);
	// 获取比特率
	int getAudioBitRate();
	// 设置比特率
	void setAudioBitRate(int bitRate);

protected:
	// 格式化歌曲播放时间
	QString formatTime(qint64 timeMilliSeconds);

private:
	// 歌曲资源地址（本地）
	QUrl m_url;
	// 歌手
	QString m_author;
	// 歌曲名
	QString m_musicName;
	// 歌词地址
	QUrl m_lyricUrl;
	// 封面地址
	QUrl m_imgUrl;
	// 时长
	qint64 m_duration;
	// 唱片集
	QString m_albumTitle;
	// 比特率
	int m_audioBitRate;
	// 歌曲网络地址（下载使用）
	QUrl m_onlineMusicUrl;
	// 歌词网络地址（下载使用）
	QUrl m_onlineLyricUrl;
	// 封面网络地址（下载使用）
	QUrl m_onlineImgUrl;
};

#endif // _KMUSICPLAYER_KMUSIC_H_
