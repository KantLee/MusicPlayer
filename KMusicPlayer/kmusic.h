// -----------------------------------------------------------
// kmusic.h
// �����ߣ� �
// ����ʱ�䣺 2024/05/31
// ���������� ����Ԫ������
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
	// ���ظ���Url
	QUrl getUrl() const;
	// ���ظ�������Ϣ
	QString getInfo();
	// ������ʾ������Ϣ
	void detail();
	// �������ݿ�
	void insertSQL();
	// �����ļ�������ȡ���·��
	QString getLyricFile();
	// ���ݸ���url��ø�����Ϣ
	void refreshInfo();
	// ���ñ��ظ�ʵ�ַ(��ӱ�������ʹ��)
	void setLyricUrl();
	// ���ñ��ظ�ʵ�ַ(�����������ʹ��)
	void setLyricUrl(QString path);
	// ��ȡ���ظ�ʵ�ַ
	QUrl getLyricUrl();
	// ���ñ��ط����ַ(��ӱ�������ʹ��)
	void setImgUrl();
	// ���ñ��ط����ַ(�����������ʹ��)
	void setImgUrl(QString path);
	// ��ȡ���ط����ַ
	QUrl getImgUrl();
	// ���ø����������ص�ַ
	void setOnlineMusicUrl(QUrl url);
	// ���ø���������ص�ַ
	void setOnlineLyricUrl(QUrl url);
	// ���÷����������ص�ַ
	void setOnlineImgUrl(QUrl url);
	// ��ȡ�����������ص�ַ
	QUrl getOnlineMusicUrl();
	// ��ȡ����������ص�ַ
	QUrl getOnlineLyricUrl();
	// ��ȡ�����������ص�ַ
	QUrl getOnlineImgUrl();
	// ��ȡ������
	QString getMusicName();
	// ���ø�����
	void setMusicName(QString name);
	// ��ȡ�������ص�ַ
	QUrl getLocalUrl();
	// ���ø������ص�ַ
	void setLocalUrl(QUrl url);
	// ��ȡ������
	QString getAuthor();
	// ���ø�����
	void setAuthor(QString name);
	// ��ȡʱ��
	qint64 getDuration();
	// ����ʱ��
	void setDuration(qint64 duration);
	// ��ȡר����
	QString getAlbumTitle();
	// ����ר����
	void setAlbumTitle(QString name);
	// ��ȡ������
	int getAudioBitRate();
	// ���ñ�����
	void setAudioBitRate(int bitRate);

protected:
	// ��ʽ����������ʱ��
	QString formatTime(qint64 timeMilliSeconds);

private:
	// ������Դ��ַ�����أ�
	QUrl m_url;
	// ����
	QString m_author;
	// ������
	QString m_musicName;
	// ��ʵ�ַ
	QUrl m_lyricUrl;
	// �����ַ
	QUrl m_imgUrl;
	// ʱ��
	qint64 m_duration;
	// ��Ƭ��
	QString m_albumTitle;
	// ������
	int m_audioBitRate;
	// ���������ַ������ʹ�ã�
	QUrl m_onlineMusicUrl;
	// ��������ַ������ʹ�ã�
	QUrl m_onlineLyricUrl;
	// ���������ַ������ʹ�ã�
	QUrl m_onlineImgUrl;
};

#endif // _KMUSICPLAYER_KMUSIC_H_
