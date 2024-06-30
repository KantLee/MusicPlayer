#include "kdownloader.h"

KDownloader::KDownloader(QObject *parent)
	: QObject(parent)
	, m_reply(Q_NULLPTR)
	, m_downloadUrl(Q_NULLPTR)
	, m_file(Q_NULLPTR)
{
	(void)connect(&m_manager, &QNetworkAccessManager::finished, this, &KDownloader::onDownloadFinished);
}

KDownloader::~KDownloader()
{
	if (m_reply)
		m_reply->deleteLater();
	if (m_file) 
	{
		m_file->close();
		delete m_file;
		m_file = Q_NULLPTR;
	}
}

void KDownloader::startDownload(const QUrl &url)
{
	if (m_file)
	{
		m_file->close();
		delete m_file;
		m_file = Q_NULLPTR;
	}
	m_downloadUrl = url;
	m_reply = m_manager.get(QNetworkRequest(m_downloadUrl));
	(void)connect(m_reply, &QNetworkReply::readyRead, this, &KDownloader::onReadyRead);
	(void)connect(m_reply, &QNetworkReply::downloadProgress, this, &KDownloader::onDownloadProgress);
}

void KDownloader::startDownload(const QUrl& url, const QString& savePath)
{
	m_downloadUrl = url;
	this->m_savePath = savePath;
	if (m_file)
	{
		m_file->close();
		delete m_file;
		m_file = Q_NULLPTR;
	}
	m_file = new QFile(savePath);
	if (!m_file->open(QIODevice::WriteOnly)) 
	{
		emit downloadError(m_savePath);
		return;
	}
	m_downloadUrl = url;
	m_reply = m_manager.get(QNetworkRequest(m_downloadUrl));
	(void)connect(m_reply, &QNetworkReply::readyRead, this, &KDownloader::onReadyRead);
	(void)connect(m_reply, &QNetworkReply::downloadProgress, this, &KDownloader::onDownloadProgress);
}

void KDownloader::onDownloadFinished(QNetworkReply *reply)
{
	if (reply->error() != QNetworkReply::NoError)
	{
		// 在下载失败时删除已经创建但不完整的文件
		if (m_file)
			m_file->remove();
	}
	if (m_file)
		m_file->close();
	emit downloadFinished();
	m_reply->deleteLater();
	m_reply = Q_NULLPTR;
}

void KDownloader::onReadyRead()
{
	QByteArray data;
	while (!m_reply->atEnd())
	{
		// 一次读8kb
		data = m_reply->read(8192);
		if (m_file)
		{
			if (m_file->isOpen())
				m_file->write(data);
		}
		emit dataReady(data);
		//qDebug() << "Data received:" << data.size() << "bytes";
	}
}

void KDownloader::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	emit downloadProgress(bytesReceived, bytesTotal);
}
