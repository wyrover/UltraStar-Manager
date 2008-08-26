#include "QURenameTaskDialog.h"

#include <QIcon>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QDomCDATASection>

QURenameTaskDialog::QURenameTaskDialog(QURenameTask *task, QWidget *parent): QUTaskDialog(task, parent) {
	init(task);
}

QURenameTaskDialog::QURenameTaskDialog(QWidget *parent): QUTaskDialog(parent) {
	init();
	dataTable->fillData(QList<QUScriptData*>(), QU::renameTask); // for setting up custom delegates
}

void QURenameTaskDialog::init(QUScriptableTask *task) {
	targetCombo->addItems(QURenameTask::availableTargets());
	if(task)
		targetCombo->setCurrentIndex(targetCombo->findText(task->target(), Qt::MatchContains));

	infoLbl->setText(tr("Select a proper <b>target</b> and create a custom <b>schema</b> for the renaming operation."));
}

/*!
 * Collect the info which is present in this dialog, create a DOM document and save the
 * XML config file to disk.
 */
bool QURenameTaskDialog::saveTask(const QString &filePath) {
	_doc.clear();
	QDomElement task = _doc.createElement("task");
	_doc.appendChild(task);

	this->appendGeneral(task, QU::renameTask);

	QDomElement rename = _doc.createElement("rename");
	task.appendChild(rename);

	rename.setAttribute("target", targetCombo->currentText());
	rename.setAttribute("schema", schemaEdit->text());

	for(int row = 0; row < dataTable->rowCount(); row++) {
		QDomElement data = _doc.createElement("data");

		if(dataTable->item(row, 0)->text() != "true")
			data.setAttribute("if", dataTable->item(row, 0)->text());

		if(dataTable->item(row, 1)->text() == KEEP_SUFFIX_SOURCE)
			data.setAttribute("keepSuffix", "true");
		else if(dataTable->item(row, 1)->text() == TEXT_SOURCE)
			data.setAttribute("text", dataTable->item(row, 2)->text());
		else if(dataTable->item(row, 1)->text() == UNKNOWN_TAGS_SOURCE)
			data.setAttribute("keepUnknownTags", "true");
		else if(!dataTable->item(row, 1)->text().isEmpty()) {
			data.setAttribute("source", dataTable->item(row, 1)->text());
			if(dataTable->item(row, 2)->text() != N_A)
				data.setAttribute("default", dataTable->item(row, 2)->text());
		}

		rename.appendChild(data);
	}

	return this->saveDocument(filePath);
}
