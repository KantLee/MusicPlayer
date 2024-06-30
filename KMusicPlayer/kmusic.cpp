#include "kmusic.h"
#include "kdatabase.h"

KMusic::KMusic(QUrl url)
{
	m_url = url;
	refreshInfo();
	setLyricUrl();
	setImgUrl();
}

QUrl KMusic::getUrl() const
{
	return m_url;
}

QString KMusic::getInfo()
{
	return m_author + " - " + m_musicName + " [" + formatTime(m_duration) + "]";
}

void KMusic::detail()
{
	QString info(QStringLiteral("��������%1\n�����ң�%2\nʱ����%3\n��Ƭ����%4\n�����ʣ�%5\n�ļ�·����%6"));
	info = info.arg(m_musicName, m_author
		, formatTime(m_duration)
		, m_albumTitle
		, QString::number(m_audioBitRate) + "bps"
		, m_url.toString());
	QMessageBox::about(Q_NULLPTR, QStringLiteral("��������"), info);
}

void KMusic::insertSQL()
{
	KDataBase::getInstance().addMusic(*this);
}

QString KMusic::getLyricFile()
{
	if (m_lyricUrl.isEmpty())
		return "None";
	return m_lyricUrl.toString();
}

void KMusic::refreshInfo()
{
	QMediaPlayer tempPlayer;
	tempPlayer.setMedia(m_url);
	// Ԫ���ݵĽ�����Ҫʱ�䣬����������Ҫѭ���ȴ�����ͬʱ��Ҫ����Qt�¼�������������У�
	while (!tempPlayer.isMetaDataAvailable())
	{
		QCoreApplication::processEvents();
	}
	QStringList list = tempPlayer.availableMetaData();// ����ʱ�鿴����ЩԪ���ݿ���
	if (tempPlayer.isMetaDataAvailable()) 
	{
		// ������Ϣ
		m_author = tempPlayer.metaData(QStringLiteral("Author")).toStringList().join(",");
		m_musicName = tempPlayer.metaData(QStringLiteral("Title")).toString();
		m_albumTitle = tempPlayer.metaData(QStringLiteral("AlbumTitle")).toString();
		m_audioBitRate = tempPlayer.metaData(QStringLiteral("AudioBitRate")).toInt();
		m_duration = tempPlayer.duration();
	}
}

void KMusic::setLyricUrl()
{
	// Ѱ�������ͬĿ¼�µ�ͬ������ļ�
	QString musicFilePath = m_url.toLocalFile();
	musicFilePath.replace(".mp3", ".lrc");
	musicFilePath.replace(".flac", ".lrc");
	musicFilePath.replace(".mpga", ".lrc");
	if (QFile::exists(musicFilePath)) 
	{
		m_lyricUrl.setUrl(musicFilePath);
	}
	else 
	{
		musicFilePath.replace(".lrc", ".txt");
		if (QFile::exists(musicFilePath))
			m_lyricUrl.setUrl(musicFilePath);
	}
}

void KMusic::setLyricUrl(QString path)
{
	m_lyricUrl.setUrl(path);
}

QUrl KMusic::getLyricUrl()
{
	return m_lyricUrl;
}

void KMusic::setImgUrl()
{
	// Ѱ�������ͬĿ¼�µ�ͬ�������ļ�
	QString musicFilePath = m_url.toLocalFile();
	musicFilePath.replace(".mp3", ".jpg");
	musicFilePath.replace(".flac", ".jpg");
	musicFilePath.replace(".mpga", ".jpg");
	if (QFile::exists(musicFilePath))
	{
		m_imgUrl.setUrl(musicFilePath);
	}
	else
	{
		musicFilePath.replace(".jpg", ".png");
		if (QFile::exists(musicFilePath))
			m_imgUrl.setUrl(musicFilePath);
	}
}

void KMusic::setImgUrl(QString path)
{
	m_imgUrl.setUrl(path);
}

QUrl KMusic::getImgUrl()
{
	return m_imgUrl;
}

void KMusic::setOnlineMusicUrl(QUrl url)
{
	m_onlineMusicUrl = url;
}

void KMusic::setOnlineLyricUrl(QUrl url)
{
	m_onlineLyricUrl = url;
}

void KMusic::setOnlineImgUrl(QUrl url)
{
	m_onlineImgUrl = url;
}

QUrl KMusic::getOnlineMusicUrl()
{
	return m_onlineMusicUrl;
}

QUrl KMusic::getOnlineLyricUrl()
{
	return m_onlineLyricUrl;
}

QUrl KMusic::getOnlineImgUrl()
{
	return m_onlineImgUrl;
}

QString KMusic::getMusicName()
{
	return m_musicName;
}

void KMusic::setMusicName(QString name)
{
	m_musicName = name;
}

QUrl KMusic::getLocalUrl()
{
	return m_url;
}

void KMusic::setLocalUrl(QUrl url)
{
	m_url = url;
}

QString KMusic::getAuthor()
{
	return m_author;
}

void KMusic::setAuthor(QString name)
{
	m_author = name;
}

qint64 KMusic::getDuration()
{
	return m_duration;
}

void KMusic::setDuration(qint64 duration)
{
	m_duration = duration;
}

QString KMusic::getAlbumTitle()
{
	return m_albumTitle;
}

void KMusic::setAlbumTitle(QString name)
{
	m_albumTitle = name;
}

int KMusic::getAudioBitRate()
{
	return m_audioBitRate;
}

void KMusic::setAudioBitRate(int bitRate)
{
	m_audioBitRate = bitRate;
}

QString KMusic::formatTime(qint64 timeMilliSeconds)
{
	qint64 seconds = timeMilliSeconds / 1000;
	const qint64 minutes = seconds / 60;
	seconds -= minutes * 60;
	return QStringLiteral("%1:%2")
		.arg(minutes, 2, 10, QLatin1Char('0'))
		.arg(seconds, 2, 10, QLatin1Char('0'));
}
