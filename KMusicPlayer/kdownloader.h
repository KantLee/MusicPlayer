// -----------------------------------------------------------
// kdownloader.h
// �����ߣ� �
// ����ʱ�䣺 2024/06/04
// ���������� ��������
// -----------------------------------------------------------

#ifndef _KMUSICPLAYER_KDOWNLOADER_H_
#define _KMUSICPLAYER_KDOWNLOADER_H_

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QBuffer>
#include <QFile>

class KDownloader : public QObject
{
	Q_OBJECT

public:
	explicit KDownloader(QObject *parent = Q_NULLPTR);
	~KDownloader();
	void startDownload(const QUrl &url);
	// ��ӱ���·������
	void startDownload(const QUrl &url, const QString &savePath);
	void onDownloadFinished(QNetworkReply *reply);
	void onReadyRead();
	void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);


signals:
	void dataReady(const QByteArray &data);
	void downloadFinished();
	void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void downloadError(QString path);

private:
	QNetworkAccessManager m_manager;
	QNetworkReply *m_reply;
	QUrl m_downloadUrl;
	QString m_savePath; // ����·��
	QFile *m_file; // ���ڱ����ļ�
};

#endif // _KMUSICPLAYER_KDOWNLOADER_H_
