#include "QULyricTaskFactory.h"
#include "QULyricTask.h"

#include <QDir>
#include <QLocale>
#include <QCoreApplication>

QULyricTaskFactory::QULyricTaskFactory(QObject *parent): QUSimpleTaskFactory(parent) {}

QString QULyricTaskFactory::name() const {
	return tr("Lyric Tasks");
}

QString QULyricTaskFactory::description() const {
	return tr("Modify a song's lyrics.");
}

QUTask* QULyricTaskFactory::createTask(int type) {
	return new QULyricTask((QULyricTask::TaskModes)type);
}

QList<int> QULyricTaskFactory::types() const {
	QList<int> result;
	result << QULyricTask::ConvertRelativeToAbsolute;
	result << QULyricTask::ConvertAbsoluteToRelative;
	result << QULyricTask::FixTimeStamps;
	result << QULyricTask::FixOverlappingNotes;
	result << QULyricTask::FixLineBreaks;
	result << QULyricTask::FixSpaces;
	result << QULyricTask::FixApostrophes;
	result << QULyricTask::FixLowBPM;
	result << QULyricTask::SetMedleyTags;
	result << QULyricTask::FixLineCapitalization;
	result << QULyricTask::NormalizePitches;
	result << QULyricTask::RemoveEmptySyllables;
	result << QULyricTask::ConvertSyllablePlaceholder1;
	result << QULyricTask::ConvertSyllablePlaceholder2;
	return result;
}

QMap<QString, QString> QULyricTaskFactory::translationLocations() const {
	QDir dir = QCoreApplication::applicationDirPath();
	QMap<QString, QString> locations;

	if(dir.cd("plugins") && dir.cd("languages")) {
		locations.insert(QLocale(QLocale::German, QLocale::Germany).name(), dir.filePath("lyric.de.qm"));
		locations.insert(QLocale(QLocale::Polish, QLocale::Poland).name(), dir.filePath("lyric.pl.qm"));
		locations.insert(QLocale(QLocale::French, QLocale::France).name(), dir.filePath("lyric.fr.qm"));
		locations.insert(QLocale(QLocale::Spanish, QLocale::Spain).name(), dir.filePath("lyric.es.qm"));
		locations.insert(QLocale(QLocale::Portuguese, QLocale::Portugal).name(), dir.filePath("lyric.pt.qm"));
	}

	return locations;
}
