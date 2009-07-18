#include "QUSongDatabase.h"
#include "QULogService.h"
#include "QUProgressDialog.h"
#include "QUSongSupport.h"
#include "QUMonty.h"

#include <QDir>
#include <QFileInfo>
#include <QString>

QUSongDatabase::QUSongDatabase(QObject *parent): QObject(parent) {}

QUSongDatabase* QUSongDatabase::_instance = 0;
QUSongDatabase* QUSongDatabase::instance() {
	if(_instance == 0)
		_instance = new QUSongDatabase(0);

	return _instance;
}

void QUSongDatabase::signalSongChanged() {
	emit songChanged(qobject_cast<QUSongFile*>(sender()));
}

void QUSongDatabase::addSong(QUSongFile *song) {
	_songs.append(song);
	connect(song, SIGNAL(dataChanged()), this, SLOT(signalSongChanged()));
	emit songAdded(song);
}

/*!
 * Deletes the given song (the whole directory). Song item has to be removed
 * from song tree before you call this function!
 */
void QUSongDatabase::deleteSong(QUSongFile *song) {
	QDir dir(song->songFileInfo().dir());
	QString artist = song->artist();
	QString title = song->title();

	QFileInfoList fiList = dir.entryInfoList(QStringList("*.*"), QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name);

	foreach(QFileInfo fi, fiList) {
		if(!QFile::remove(fi.filePath()))
			logSrv->add(QString(tr("Could NOT delete file: \"%1\"")).arg(fi.filePath()), QU::Warning);
		else
			logSrv->add(QString(tr("File was deleted successfully: \"%1\"")).arg(fi.filePath()), QU::Information);
	}

	QString dirName = dir.dirName();
	dir.cdUp();

	if(!dir.rmdir(dirName))
		logSrv->add(QString(tr("Could NOT delete directory: \"%1\". Maybe it is not empty.")).arg(song->songFileInfo().path()), QU::Warning);
	else
		logSrv->add(QString(tr("Directory was deleted successfully: \"%1\"")).arg(song->songFileInfo().path()), QU::Information);

	_songs.removeAll(song);
	delete song;
	emit songDeleted(song);

	logSrv->add(QString(tr("Song was deleted successfully: \"%1 - %2\"")).arg(artist).arg(title), QU::Information);
}

void QUSongDatabase::clear() {
	emit databaseCleared();
	qDeleteAll(_songs);
	_songs.clear();
}

/*!
 * Scan the given directory and create song file objects. Database will be cleared first.
 */
void QUSongDatabase::reload() {
	clear();

	QList<QDir> dirList;
	dirList.append(QDir(QU::BaseDir));

	// create a list of all sub-directories
	readSongDir(dirList);

	QUProgressDialog dlg(tr("Reading song files..."), dirList.size(), parentWidget());
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	foreach(QDir dir, dirList) {
		QFileInfoList songFiList = dir.entryInfoList(QUSongSupport::allowedSongFiles(), QDir::Files, QDir::Name);
		qStableSort(songFiList.begin(), songFiList.end(), QU::fileTypeLessThan);

		dlg.update(dir.dirName());
		if(dlg.cancelled()) break;

		QUSongFile *newSong = 0;

		foreach(QFileInfo fi, songFiList) {
			if( QUSongSupport::allowedLicenseFiles().contains(fi.fileName(), Qt::CaseInsensitive) )
				continue; // skip license files (txt)

			if(!newSong)
				newSong = new QUSongFile(fi.filePath());
			else { // found a friend!
				QUSongFile *friendSong = new QUSongFile(fi.filePath());
				newSong->addFriend(friendSong);
				// see "this->addSong(...)"
				connect(friendSong, SIGNAL(dataChanged()), this, SLOT(signalSongChanged()));
			}
		}

		if(newSong) // some song found (not friend)
			addSong(newSong);
	}

	emit databaseReloaded();
}

void QUSongDatabase::ignoreChangesForSong(QUSongFile *song) {
	disconnect(song, SIGNAL(dataChanged()), this, SLOT(signalSongChanged()));
}

void QUSongDatabase::processChangesForSong(QUSongFile *song) {
	connect(song, SIGNAL(dataChanged()), this, SLOT(signalSongChanged()));
}


/*!
 * Reads recursively all available directories and puts them in the dirList.
 */
void QUSongDatabase::readSongDir(QList<QDir> &dirList) {
	if(dirList.isEmpty())
		return;

	if(dirList.last().entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::NoSymLinks, QDir::Name).isEmpty()) {
		return;
	} else {
		QDir thisDir(dirList.last());
		QStringList subDirs = dirList.last().entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::NoSymLinks, QDir::Name);

		foreach(QString dir, subDirs) {
			QDir newDir(thisDir);
			newDir.cd(dir);

			dirList.append(newDir);
			this->readSongDir(dirList);
		}
	}
}