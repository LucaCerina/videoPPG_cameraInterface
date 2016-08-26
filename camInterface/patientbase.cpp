#include "patientbase.h"
#include "ui_patientbase.h"
#include "utilities.h"
#include "patientdialog.h"

patientBase::patientBase(QWidget *parent,QString callerObject) :
	QDialog(parent),
	ui(new Ui::patientBase)
{
	caller = callerObject;
	ui->setupUi(this);
	ui->tabWidget->setCurrentIndex(0);
	connect(this, SIGNAL(rejected()), this, SLOT(on_window_rejected()));

	// Populate patient list
	populatePtList();

	if(caller == "newRec")
	{
		QWidget::setWindowTitle("Select user");
		ui->ptDeleteButton->setEnabled(false);
		ui->ptEditButton->setEnabled(false);
		ui->tabWidget->setTabEnabled(1,false);
	}
	// Load video process
	videoProcess = new QProcess(this);
}

patientBase::~patientBase()
{
	delete ui;
}

void patientBase::populatePtList()
{
	QString ptPath = QDir::currentPath() + "/users/";
	QString ptFolderPath, tempPtData;
	QStringList ptStringList;
	QDirIterator iterDir(ptPath,QDir::NoDotAndDotDot|QDir::Dirs, QDirIterator::NoIteratorFlags);

	while(iterDir.hasNext())
	{
		// Current patient folder
		ptFolderPath = iterDir.next();

		// Open patient file in folder
		tempPtData = ptFolderPath.section("/", -1);
		QSettings ptFile(ptFolderPath + "/ptData.dat", QSettings::IniFormat);
		tempPtData += " " + ptFile.value("Patient name").toString() + " "
				+ ptFile.value("Patient surname").toString();
		ptStringList.append(tempPtData);

	}

	// Populate listview
	ptStringList.sort();
	ptListModel->setStringList(ptStringList);
	ui->ptListView->setModel(ptListModel);
}

void patientBase::populateExamList()
{
	QString recFolderPath;
	QStringList recStringList;

	// Get current patient folder
	QString ptFolder = ptListModel->data(ui->ptListView->currentIndex(), Qt::DisplayRole).toString();
	QString ptID = ptFolder.section(" ", 0, 0);
	ptFolder = QDir::currentPath() + "/users/" + ptID + "/records/";

	// Get subfolders
	QDirIterator iterDir(ptFolder, QDir::NoDotAndDotDot | QDir::Files, QDirIterator::NoIteratorFlags);

	while(iterDir.hasNext())
	{
		recFolderPath = iterDir.next().section(".avi", -2, -2).section("/", -1);
		if(recFolderPath.compare("")!=0)
			recStringList.append(recFolderPath);
	}

	// Populate listview
	recStringList.sort();
	recListModel->setStringList(recStringList);
	ui->ptExamListView->setModel(recListModel);
	connect(ui->ptExamListView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
			this, SLOT(on_videoSelectionChanged(QModelIndex)));
}

bool patientBase::getPtData()
{
	QString line, lineType, lineValue;
	// Current patient folder
	QString ptFolder = ptListModel->data(ui->ptListView->currentIndex(), Qt::DisplayRole).toString();
	if(ptFolder.compare("")!=0)
	{
		ptFolder = ptFolder.section(" ", 0, 0);
		ptIstance.ptId = ptFolder;
		ptFolder = QDir::currentPath() + "/users/" + ptFolder + "/ptData.dat";

		// Open patient file
		QSettings ptFile(ptFolder, QSettings::IniFormat);

		// Get general data
		ptIstance.ptName = ptFile.value("Patient Name").toString();
		ptIstance.ptSurName = ptFile.value("Patient surname").toString();
		ptIstance.ptSex = ptFile.value("Patient biological sex").toString();
		ptIstance.ptBDay = QDate::fromString(ptFile.value("Patient birthday").toString(), "dd-MM-yyyy");

		// Switch to last update group
		ptFile.beginGroup("Update " + ptFile.value("Last update").toString());

		// Get remaining data
		ptIstance.ptAFType = ptFile.value("Patient AF type").toString();
		ptIstance.ptRhyTrea = ptFile.value("Rhythm treatment").toString();
		ptIstance.ptRhyDose = ptFile.value("Rhythm dosage").toString();
		ptIstance.ptRhyTime = QDate::fromString(ptFile.value("Rhythm start").toString(), "dd-MM-yyyy");
		ptIstance.ptFreqTrea = ptFile.value("Frequency treatment").toString();
		ptIstance.ptFreqDose = ptFile.value("Frequency dosage").toString();
		ptIstance.ptFreqTime = QDate::fromString(ptFile.value("Frequency start").toString(), "dd-MM-yyyy");
		ptIstance.ptAnticTrea = ptFile.value("Anticoagulant treatment").toString();
		ptIstance.ptAnticDose = ptFile.value("Anticoagulant dosage").toString();
		ptIstance.ptAnticTime = QDate::fromString(ptFile.value("Anticoagulant start").toString(), "dd-MM-yyyy");
		ptIstance.ptNotes = ptFile.value("Notes").toString();
		return true;
	}
	else if(ptFolder.compare("")==0 && caller!="newRec")
	{
		QMessageBox::warning(this, "Error", "No patient selected or available for data access");
		this->show();
		return false;
	}
}

void patientBase::on_ptAccessButton_clicked()
{
	/* Update patient data visualization and populate records list */
	getPtData();
	if(ptIstance.ptId != "")
	{
		// Age
		ui->labelValueAge->setText(QString::number(calculateAge(ptIstance.ptBDay, QDate::currentDate())));
		// Sex
		ui->labelValueSex->setText(ptIstance.ptSex);
		// AF
		ui->labelValueAFType->setText(ptIstance.ptAFType);

		// Treatments
		// Rhythm
		if(!ptIstance.ptRhyTrea.compare("NA"))
		{
			ui->labelValueType_r->setText("No Treatment");
			ui->labelValueFrom_r->setText(ptIstance.ptRhyTime.toString("dd-MM-yyyy"));
			ui->tabTreatment->setTabEnabled(0, false);
		}
		else
		{
			ui->tabTreatment->setTabEnabled(0, true);
			ui->labelValueType_r->setText(ptIstance.ptRhyTrea);
		}
		ui->labelValueFrom_r->setText(ptIstance.ptRhyTime.toString("dd-MM-yyyy"));
		ui->labelValueDose_r->setText(ptIstance.ptRhyDose);

		// Frequency
		if(!ptIstance.ptFreqTrea.compare("NA"))
		{
			ui->labelValueType_f->setText("No Treatment");
			ui->labelValueFrom_f->setText(ptIstance.ptRhyTime.toString("dd-MM-yyyy"));
			ui->tabTreatment->setTabEnabled(1, false);
		}
		else
		{
			ui->tabTreatment->setTabEnabled(1, true);
			ui->labelValueType_f->setText(ptIstance.ptFreqTrea);
		}
		ui->labelValueFrom_f->setText(ptIstance.ptFreqTime.toString("dd-MM-yyyy"));
		ui->labelValueDose_f->setText(ptIstance.ptFreqDose);

		//Anticoagulant
		if(!ptIstance.ptAnticTrea.compare("NA"))
		{
			ui->labelValueType_a->setText("No Treatment");
			ui->labelValueFrom_a->setText(ptIstance.ptAnticTime.toString("dd-MM-yyyy"));
			ui->tabTreatment->setTabEnabled(2, false);
		}
		else
		{
			ui->tabTreatment->setTabEnabled(2, true);
			ui->labelValueType_a->setText(ptIstance.ptAnticTrea);
		}
		ui->labelValueFrom_a->setText(ptIstance.ptAnticTime.toString("dd-MM-yyyy"));
		ui->labelValueDose_a->setText(ptIstance.ptAnticDose);

		// Populate exams list
		populateExamList();
	}
	else if(caller =="newRec")
	{
		QMessageBox::warning(this, "Error", "No patient selected or available for data access");
		this->show();
	}
}

void patientBase::on_ptEditButton_clicked()
{
	/* Edit patient data */
	if(getPtData())
	{
		PatientDialog *ptDialog = new PatientDialog(this, "editData");
		connect(this, SIGNAL(sendPtData(PatientDialog::PatientData)),
				ptDialog, SLOT(getExternalPtData(PatientDialog::PatientData)));
		emit sendPtData(ptIstance);
		ptDialog->show();
	}
}

void patientBase::on_ptNewButton_clicked()
{
	PatientDialog * ptDialog = new PatientDialog(this);
	connect(ptDialog, SIGNAL(accepted()), this, SLOT(populatePtList()));
	ptDialog->show();
}

void patientBase::on_ptDeleteButton_clicked()
{
	if(getPtData())
	{
		QMessageBox::StandardButton msgBox;
		msgBox = QMessageBox::question(this, "Delete",
									   "Are you sure? This operation cannot be undone!",
									   QMessageBox::Yes | QMessageBox::No);

		if(msgBox==QMessageBox::Yes)
		{
			QDir dir;
			QString currentFolder = QDir::currentPath() + "/users/" + ptIstance.ptId;
			QString deleteFolder = QDir::currentPath() + "/del_users/" + ptIstance.ptId
					+ "_deleted_" + QDate::currentDate().toString("dd-MM-yyyy");
			dir.rename(currentFolder, deleteFolder);
			msgBox = QMessageBox::information(this, "Delete executed",
											  "Patient with ID " + ptIstance.ptId + " has been deleted");
		}
		populatePtList();
	}
}

void patientBase::on_buttonBox_accepted()
{
	if(caller=="newRec" && getPtData())
	{
		connect(this, SIGNAL(sendPtData(PatientDialog::PatientData)),
				this->parent(), SLOT(getExternalPtData(PatientDialog::PatientData)));
		emit sendPtData(ptIstance);
	}
}

//PROCESS VIDEOS
void patientBase::on_videoProcessButton_clicked()
{
	bool processFlag = true;

	// Get exam file
	QString tempData = recListModel->data(ui->ptExamListView->currentIndex(), Qt::DisplayRole).toString();
	qDebug() << tempData;
	QString examFile = QDir::currentPath() + "/users/" + tempData.section("_",1,1)
			+ "/records/" + tempData + ".dat";
	QSettings examData(examFile,QSettings::IniFormat);

	// Video filename
	QString videoFilename = QDir::currentPath() + "/users/" + tempData.section("_",1,1)
			+ "/records/" + tempData + ".avi";

	// Signals folder
	QString examDir = QDir::currentPath() + "/users/" + tempData.section("_",1,1)
			+ "/signals/";
	if(!QDir(examDir).exists())
		QDir().mkdir(examDir);
	QString outSigFilename = examDir + tempData;

	// Check if the video has been already processed
	if(examData.value("processed").toString() == "yes")
	{
		QMessageBox::StandardButton msgBox;
		msgBox = QMessageBox::question(this, "Continue processing",
									   "The exam has been already processed. Do you want to continue?",
									   QMessageBox::Yes | QMessageBox::No);
		if(msgBox == QMessageBox::No)
			processFlag = false;
	}

	// Start video processing
	if(processFlag && QFile("PhysioCAM.exe").exists())
	{
		// Set progress bar
		ui->executionProgress->setValue(0);

		// Set execution progress bar
		ui->executionProgress->setMaximum(examData.value("Framerate").toInt() * examData.value("Duration").toInt());
		connect(videoProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(printOutput()));
		connect(videoProcess, SIGNAL(finished(int)), this, SLOT(on_videoProcessEnded()));

		// Set video process functions
		videoProcess->setProcessChannelMode(QProcess::MergedChannels);
		videoProcess->setProgram("PhysioCAM.exe");
		QChar vidOutEnabled;
		if(ui->vidOutCheck->isChecked())
			vidOutEnabled = '1';
		else
			vidOutEnabled = '0';
		QStringList arguments;
		arguments << vidOutEnabled << QString::number(reloadSteps) << QString::number(frameSub) << videoFilename << outSigFilename << "0";
		videoProcess->setArguments(arguments);

		// Start video processing
		videoProcess->start();
		if(videoProcess->waitForStarted(-1))
		{
			ui->executionLabel->setText("Process started");
		}
	}

	// End processing
	QSettings recordData(outSigFilename + "_meta", QSettings::IniFormat);
	recordData.setValue("reloadSteps", QString::number(reloadSteps));
	recordData.setValue("frameSubs", QString::number(frameSub));
	examData.setValue("processed", "yes");
	examData.setValue("processDate", QDate::currentDate().toString("dd-MM-yyyy"));
}

void patientBase::on_sigButton_clicked()
{
	// Get exam folder
	QString tempData = recListModel->data(ui->ptExamListView->currentIndex(), Qt::DisplayRole).toString();
	QString examFile = QDir::currentPath() + "/users/" + tempData.section("_", 1, 1)
			+ "/records/" + tempData + ".dat";
	QSettings examData(examFile, QSettings::IniFormat);

	// Get signal file
	QString sigFile = QDir::currentPath() + "/users/" + tempData.section("_",1,1)
			+ "/signals/" + tempData + "_rgb.csv";

	// Set process
	QProcess signalProcess;
	signalProcess.setProgram("C:/Users/Bio-tec/Anaconda3/python.exe"); //ORRORE DA SISTEMARE
	qDebug() << "program " << signalProcess.program();
	QStringList arguments;
	arguments << "C:\\Users\\Bio-tec\\Documents\\DEV\\QT\\build-camInterface-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug\\signalProcess.py";
	arguments << sigFile;
	arguments << examData.value("realFps").toString();
	signalProcess.setArguments(arguments);

	// Start signal process
	signalProcess.start();
	if(signalProcess.waitForStarted(-1))
	{
		qDebug() << "Script started";
	}
	signalProcess.waitForFinished(-1);

	// Print output
	QString pDebug = signalProcess.readAllStandardOutput();
	qDebug() << pDebug;
}

void patientBase::printOutput()
{
	QByteArray strdata = videoProcess->readAllStandardOutput();
	if(strdata.contains("|"))
	{
		QString tempString = strdata.split(' ')[1];
		int currentFrame = tempString.split('|')[0].toInt();
		ui->executionProgress->setValue(currentFrame);
	}
	else if(strdata.contains("Select"))
	{
		ui->outputLabel->setText(strdata.data());
	}
	else if(strdata.contains("Currfps") && ui->executionProgress->value()%128==0)
	{
		double seconds = (double)(ui->executionProgress->maximum() - ui->executionProgress->value());
		seconds /= strdata.split(' ')[1].split('\r')[0].toDouble();
		QString eta = QDateTime::fromTime_t(floor(seconds)).toUTC().toString("hh:mm:ss");
		ui->etaLabel->setText(eta);
	}
}

void patientBase::on_videoSelectionChanged(QModelIndex current)
{
	// Load video meta data
	QString videoName = current.data().toString();
	QString subject = videoName.section("_", 1, 1);
	QSettings videoMetaData(QDir::currentPath() + "/users/" + subject
							+ "/records/" + videoName + ".dat", QSettings::IniFormat);

	// Get values
	videoFps = videoMetaData.value("Framerate").toDouble();
	videoDuration = videoMetaData.value("Duration").toInt();

	// Set reload label
	double reloadTime = videoDuration / double(ui->spinReload->value());
	ui->labelReloadTime->setText(QString::number(reloadTime) + "s");

	// Set tracker label
	double trackerFps = videoFps / double(ui->spinTracker->value());
	ui->labelTrackerFps->setText(QString::number(trackerFps) + "fps");
}

void patientBase::on_spinReload_valueChanged(int arg1)
{
	reloadSteps = arg1;
	double reloadTime = videoDuration / double(arg1);
	ui->labelReloadTime->setText(QString::number(reloadTime) + "s");
}

void patientBase::on_spinTracker_valueChanged(int arg1)
{
	frameSub = arg1;
	double trackerFps = videoFps / double(arg1);
	ui->labelTrackerFps->setText(QString::number(trackerFps) + "fps");
}

void patientBase::on_openVideoButton_clicked()
{
	QString tempData = recListModel->data(ui->ptExamListView->currentIndex(), Qt::DisplayRole).toString();
	QString videoFile = QDir::currentPath() + "/users/" + tempData.section("_",1,1)
			+ "/records/" + tempData + ".avi";
	QDesktopServices::openUrl(QUrl::fromLocalFile(videoFile));
}

void patientBase::on_window_rejected()
{
	// Handle if video analysis process is still running
	if(videoProcess != NULL)
		if(videoProcess->state() == QProcess::Running)
		{
			QMessageBox::StandardButton msgBox;
			msgBox = QMessageBox::question(this, "Video analysis running",
										   "Video Analysis running."
										   "If you press ok, the data will not be saved",
										   QMessageBox::Yes | QMessageBox::No);
			if(msgBox==QMessageBox::Yes)
				this->videoProcess->kill();
			else
				this->show();
		}
}

void patientBase::on_videoProcessEnded()
{
	ui->executionLabel->setText("Process completed");
}
