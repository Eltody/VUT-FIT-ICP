#include <QtWidgets>
#include <QToolBar>
#include <QString>
#include <string>

#include "../core/blocks.h"
#include "blockeditor.h"
#include "ui_blockeditor.h"
#include "block_ui.h"
#include "graph_ui.h"


const int Style::NodeFieldOffset = 25;  //b
const int Style::NodeFieldWidth = 65;   //b
const int Style::NodeNamePadding = 8;   //b
const int Style::NodeNameHeight = 40;   //b
const int Style::NodeMinWidth = 60; //b
const int Style::NodeRoundSize = 9; //b
const QColor Style::NodeOutlineCol = QColor(0, 0, 0);   //b
const QColor Style::NodeOutlineHighlightCol = QColor(76, 76, 76);   //b
const QColor Style::NodeBackgroundCol = QColor(76, 76, 76); //b
const int Style::PortMarginV = 30;  //b
const int Style::PortNamePadding = 15;  //b p

bool calcIsOpen = false;
bool vectIsOpen = false;
bool matr2IsOpen = false;
bool matr3IsOpen = false;

BLOCKEDITOR::BLOCKEDITOR(GraphUI &g, QWidget *parent) :
	QMainWindow(parent),  ui(new Ui::BLOCKEDITOR), graph(g){
	ui->setupUi(this);

    QApplication::setApplicationDisplayName("(j)Elitný editor");
    setMinimumSize(600, 558);
    createActions();
	createToolBars();
	setCurrentFile("");
	setUnifiedTitleAndToolBarOnMac(true);
    setContextMenuPolicy(Qt::NoContextMenu);
	setCentralWidget(&graph);
	show();

    graph.onGraphChange([this](){this->setWindowModified(true);});
}

BLOCKEDITOR::~BLOCKEDITOR(){
	//avoid destruction of graph, which is destructed externally
	centralWidget()->setParent(nullptr);

    delete newBTN;
    delete openBTN;
    delete saveBTN;
    delete importBTN;
    delete computeAct;
    delete stepAct;
    delete resetAct;
    delete helpAct;
    delete calculatorBTN;
    delete vectorBTN;
    delete matrix2BTN;
    delete matrix3BTN;
	delete ui;
}

void BLOCKEDITOR::createActions(){
    newBTN = new QAction(QIcon(":/icons/new.png"), "&Nový...", this);
    newBTN->setShortcuts(QKeySequence::New);
    newBTN->setStatusTip("Vytvoriť novú plochu");
    connect(newBTN, SIGNAL(triggered()), this, SLOT(newFile()));

    openBTN = new QAction(QIcon(":/icons/open.png"), "&Otvoriť...", this);
    openBTN->setShortcuts(QKeySequence::Open);
    openBTN->setStatusTip("Otvoriť uložený súbor");
    connect(openBTN, SIGNAL(triggered()), this, SLOT(open()));

    importBTN = new QAction(QIcon(":/icons/import.png"), "&Vložiť", this);
    importBTN->setStatusTip("Vloží súbor do aktuálnej plochy");
    connect(importBTN, SIGNAL(triggered()), this, SLOT(merge()));

    saveBTN = new QAction(QIcon(":/icons/save.png"), "&Uložiť", this);
    saveBTN->setShortcuts(QKeySequence::Save);
    saveBTN->setStatusTip("Uloží plochu na disk");
    connect(saveBTN, SIGNAL(triggered()), this, SLOT(save()));

    computeAct = new QAction(QIcon(":/icons/compute.png"), "&Vypočítať (F3)", this);
	computeAct->setShortcut(QKeySequence::fromString("F3", QKeySequence::NativeText));
    computeAct->setStatusTip("Vypočíta všetky bloky");
	connect(computeAct, SIGNAL(triggered()), this, SLOT(compute()));

    stepAct = new QAction(QIcon(":/icons/step.png"), "&Krokovať (F4)", this);
	stepAct->setShortcut(QKeySequence::fromString("F4", QKeySequence::NativeText));
    stepAct->setStatusTip("Vypočíta jeden krok");
	connect(stepAct, SIGNAL(triggered()), this, SLOT(step()));

    resetAct = new QAction(QIcon(":/icons/reset.png"), "&Obnoviť (F5)", this);
	resetAct->setShortcut(QKeySequence::fromString("F5", QKeySequence::NativeText));
    resetAct->setStatusTip("Vynuluje výpočet");
	connect(resetAct, SIGNAL(triggered()), this, SLOT(reset()));

    helpAct = new QAction(QIcon(":/icons/help.png"), "&Nápoveda", this);
    helpAct->setStatusTip("Zobrazí nápovedu");
	helpAct->setShortcuts(QKeySequence::HelpContents);
	connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));
}

void BLOCKEDITOR::createToolBars(){
    connect(calcIn, SIGNAL(triggered()), this , SLOT(buildMathInput()));
    connect(calcOut, SIGNAL(triggered()), this , SLOT(buildMathOutput()));
    connect(calcAdd, SIGNAL(triggered()), this , SLOT(buildMathAdd()));
    connect(calcSub, SIGNAL(triggered()), this , SLOT(buildMathSub()));
    connect(calcMul, SIGNAL(triggered()), this , SLOT(buildMathMul()));
    connect(calcDiv, SIGNAL(triggered()), this , SLOT(buildMathDiv()));
    connect(calcSqrt, SIGNAL(triggered()), this , SLOT(buildMathSqrt()));
    connect(calcAbs, SIGNAL(triggered()), this , SLOT(buildMathAbs()));
    connect(calcGt, SIGNAL(triggered()), this , SLOT(buildMathGt()));
    connect(calcLt, SIGNAL(triggered()), this , SLOT(buildMathLt()));
    connect(calcSin, SIGNAL(triggered()), this , SLOT(buildMathSin()));
    connect(calcCos, SIGNAL(triggered()), this , SLOT(buildMathCos()));
    connect(calcInc, SIGNAL(triggered()), this , SLOT(buildMathInc()));
    connect(calcDec, SIGNAL(triggered()), this , SLOT(buildMathDec()));

    connect(vectorInp, SIGNAL(triggered()), this , SLOT(buildVectorInput()));
    connect(vectorInp3D, SIGNAL(triggered()), this , SLOT(buildVectorInput3D()));
    connect(vectorOut, SIGNAL(triggered()), this , SLOT(buildVectorOutput()));
    connect(vectorOut3D, SIGNAL(triggered()), this , SLOT(buildVectorOutput3D()));
    connect(vectorDotOut, SIGNAL(triggered()), this , SLOT(buildVectorDotOut()));
    connect(vectorAdd, SIGNAL(triggered()), this , SLOT(buildVectorAdd()));
    connect(vectorAdd3D, SIGNAL(triggered()), this , SLOT(buildVectorAdd3D()));
    connect(vectorSub, SIGNAL(triggered()), this , SLOT(buildVectorSub()));
    connect(vectorSub3D, SIGNAL(triggered()), this , SLOT(buildVectorSub3D()));
    connect(vectorMul, SIGNAL(triggered()), this , SLOT(buildVectorMul()));
    connect(vectorMul3D, SIGNAL(triggered()), this , SLOT(buildVectorMul3D()));
    connect(vectorDot, SIGNAL(triggered()), this , SLOT(buildVectorDot()));

    connect(matrixInp, SIGNAL(triggered()), this , SLOT(buildMatrixInput()));
    connect(matrixOut, SIGNAL(triggered()), this , SLOT(buildMatrixOutput()));
    connect(matrixDetIn, SIGNAL(triggered()), this , SLOT(buildMatrixDetInput()));
    connect(matrixDetOut, SIGNAL(triggered()), this , SLOT(buildMatrixDetOutput()));
    connect(matrixAdd, SIGNAL(triggered()), this , SLOT(buildMatrixAdd()));
    connect(matrixSub, SIGNAL(triggered()), this , SLOT(buildMatrixSub()));
    connect(matrixMul, SIGNAL(triggered()), this , SLOT(buildMatrixMul()));
    connect(matrixMulVec, SIGNAL(triggered()), this , SLOT(buildMatrixMulVec()));
    connect(matrixDet, SIGNAL(triggered()), this , SLOT(buildMatrixDet()));
    connect(matrix3In, SIGNAL(triggered()), this , SLOT(buildMatrix3In()));
    connect(matrix3Out, SIGNAL(triggered()), this , SLOT(buildMatrix3Out()));
    connect(matrix3Add, SIGNAL(triggered()), this , SLOT(buildMatrix3Add()));
    connect(matrix3Sub, SIGNAL(triggered()), this , SLOT(buildMatrix3Sub()));
    connect(matrix3Mul, SIGNAL(triggered()), this , SLOT(buildMatrix3Mul()));


    calculatorBTN = new QAction(QIcon(":/icons/expandCalc.png"), "", this);
    calculatorBTN->setStatusTip("Základné matematické operácie");
    connect(calculatorBTN, SIGNAL(triggered()), this, SLOT(calc()));

    vectorBTN = new QAction(QIcon(":/icons/expandVect.png"), "", this);
    vectorBTN->setStatusTip("Operácie nad vektormi");
    connect(vectorBTN, SIGNAL(triggered()), this, SLOT(vect()));

    matrix2BTN = new QAction(QIcon(":/icons/expandMatr2.png"), "", this);
    matrix2BTN->setStatusTip("Operácie nad 2x2 maticami");
    connect(matrix2BTN, SIGNAL(triggered()), this, SLOT(matr2()));

    matrix3BTN = new QAction(QIcon(":/icons/expandMatr3.png"), "", this);
    matrix3BTN->setStatusTip("Operácie nad 3x3 maticami");
    connect(matrix3BTN, SIGNAL(triggered()), this, SLOT(matr3()));

    blockMenu = new QToolBar();
    addToolBar(Qt::LeftToolBarArea, blockMenu);
    blockMenu->setIconSize(QSize(226, 45));
    blockMenu->setFixedWidth(226);
    blockMenu->setMovable(false);
    blockMenu->addAction(calculatorBTN);
    blockMenu->addAction(matrix2BTN);
    blockMenu->addAction(matrix3BTN);
    blockMenu->addAction(vectorBTN);

    fileToolBar = addToolBar("Súbor");
    fileToolBar->setMovable(false);
    fileToolBar->addAction(newBTN);
    fileToolBar->addAction(openBTN);
    fileToolBar->addAction(saveBTN);
    fileToolBar->addAction(importBTN);

    actionToolBar = addToolBar("Akcie");
    actionToolBar->setMovable(false);
	actionToolBar->addAction(computeAct);
	actionToolBar->addAction(stepAct);
    actionToolBar->addAction(resetAct);

    spacerWidget = new QWidget();
    spacerWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    helpToolBar = addToolBar("Nápoveda");
    helpToolBar->setMovable(false);
    helpToolBar->addWidget(spacerWidget);
	helpToolBar->addAction(helpAct);
}

void BLOCKEDITOR::buildMathInput(){
    graph.addBlock(MATH_INPUT);
}

void BLOCKEDITOR::buildMathOutput(){
    graph.addBlock(MATH_OUTPUT);
}

void BLOCKEDITOR::buildMathAdd(){
    graph.addBlock(MATH_ADD);
}

void BLOCKEDITOR::buildMathSub(){
    graph.addBlock(MATH_SUB);
}

void BLOCKEDITOR::buildMathMul(){
    graph.addBlock(MATH_MUL);
}

void BLOCKEDITOR::buildMathDiv(){
    graph.addBlock(MATH_DIV);
}

void BLOCKEDITOR::buildMathSqrt(){
    graph.addBlock(MATH_SQRT);
}

void BLOCKEDITOR::buildMathAbs(){
    graph.addBlock(MATH_ABS);
}

void BLOCKEDITOR::buildMathGt(){
    graph.addBlock(MATH_GT);
}

void BLOCKEDITOR::buildMathLt(){
    graph.addBlock(MATH_LT);
}

void BLOCKEDITOR::buildMathSin(){
    graph.addBlock(MATH_SIN);
}

void BLOCKEDITOR::buildMathCos(){
    graph.addBlock(MATH_COS);
}

void BLOCKEDITOR::buildMathInc(){
    graph.addBlock(MATH_INC);
}

void BLOCKEDITOR::buildMathDec(){
    graph.addBlock(MATH_DEC);
}



void BLOCKEDITOR::buildVectorInput(){
    graph.addBlock(VECTOR_INPUT);
}

void BLOCKEDITOR::buildVectorInput3D(){
    graph.addBlock(VECTOR3D_INPUT);
}

void BLOCKEDITOR::buildVectorOutput(){
    graph.addBlock(VECTOR_OUTPUT);
}

void BLOCKEDITOR::buildVectorOutput3D(){
    graph.addBlock(VECTOR3D_OUTPUT);
}

void BLOCKEDITOR::buildVectorDotOut(){
    graph.addBlock(VECTOR_DOT_OUTPUT);
}

void BLOCKEDITOR::buildVectorAdd(){
    graph.addBlock(VECTOR_ADD);
}

void BLOCKEDITOR::buildVectorAdd3D(){
    graph.addBlock(VECTOR_ADD3D);
}

void BLOCKEDITOR::buildVectorSub(){
    graph.addBlock(VECTOR_SUB);
}

void BLOCKEDITOR::buildVectorSub3D(){
    graph.addBlock(VECTOR_SUB3D);
}

void BLOCKEDITOR::buildVectorMul(){
    graph.addBlock(VECTOR_MUL_CONST);
}

void BLOCKEDITOR::buildVectorMul3D(){
    graph.addBlock(VECTOR_MUL_CONST3D);
}

void BLOCKEDITOR::buildVectorDot(){
    graph.addBlock(VECTOR_DOTPRODUCT);
}



void BLOCKEDITOR::buildMatrixInput(){
    graph.addBlock(MAT2_INPUT);
}

void BLOCKEDITOR::buildMatrixOutput(){
    graph.addBlock(MAT2_OUTPUT);
}

void BLOCKEDITOR::buildMatrixDetInput(){
    graph.addBlock(MAT_DETERMINANT_INPUT);
}

void BLOCKEDITOR::buildMatrixDetOutput(){
    graph.addBlock(MAT_DETERMINANT_OUTPUT);
}

void BLOCKEDITOR::buildMatrixAdd(){
    graph.addBlock(MAT_ADD);
}

void BLOCKEDITOR::buildMatrixSub(){
    graph.addBlock(MAT_SUB);
}

void BLOCKEDITOR::buildMatrixMul(){
    graph.addBlock(MAT_MUL);
}

void BLOCKEDITOR::buildMatrixMulVec(){
    graph.addBlock(MAT_MUL_VEC);
}

void BLOCKEDITOR::buildMatrixDet(){
    graph.addBlock(MAT_DETERMINANT);
}

void BLOCKEDITOR::buildMatrix3In(){
    graph.addBlock(MAT3_INPUT);
}

void BLOCKEDITOR::buildMatrix3Out(){
    graph.addBlock(MAT3_OUTPUT);
}

void BLOCKEDITOR::buildMatrix3Add(){
    graph.addBlock(MAT3_ADD);
}

void BLOCKEDITOR::buildMatrix3Sub(){
    graph.addBlock(MAT3_SUB);
}

void BLOCKEDITOR::buildMatrix3Mul(){
    graph.addBlock(MAT3_MUL);
}


void BLOCKEDITOR::closeEvent(QCloseEvent *event){
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void BLOCKEDITOR::calc(){
    if (vectIsOpen == true){
        vect();
    } else if (matr2IsOpen == true){
        matr2();
    } else if (matr3IsOpen == true){
        matr3();
    }
    if (calcIsOpen == false){
        calcIsOpen = true;
        calculatorBTN->setIcon(QIcon(":/icons/contractCalc.png"));
        matrix2BTN->setIcon(QIcon(":/icons/btmMatr2.png"));
        blockMenu->removeAction(matrix3BTN);
        blockMenu->removeAction(matrix2BTN);
        blockMenu->removeAction(vectorBTN);
        blockMenu->addAction(calcIn);
        blockMenu->addAction(calcOut);
        blockMenu->addAction(calcAdd);
        blockMenu->addAction(calcSub);
        blockMenu->addAction(calcMul);
        blockMenu->addAction(calcDiv);
        blockMenu->addAction(calcSqrt);
        blockMenu->addAction(calcAbs);
        blockMenu->addAction(calcGt);
        blockMenu->addAction(calcLt);
        blockMenu->addAction(calcSin);
        blockMenu->addAction(calcCos);
        blockMenu->addAction(calcInc);
        blockMenu->addAction(calcDec);
        blockMenu->addAction(matrix2BTN);
        blockMenu->addAction(matrix3BTN);
        blockMenu->addAction(vectorBTN);
    } else{
        calcIsOpen = false;
        calculatorBTN->setIcon(QIcon(":/icons/expandCalc.png"));
        matrix2BTN->setIcon(QIcon(":/icons/expandMatr2.png"));
        blockMenu->removeAction(calcIn);
        blockMenu->removeAction(calcOut);
        blockMenu->removeAction(calcAdd);
        blockMenu->removeAction(calcSub);
        blockMenu->removeAction(calcMul);
        blockMenu->removeAction(calcDiv);
        blockMenu->removeAction(calcSqrt);
        blockMenu->removeAction(calcAbs);
        blockMenu->removeAction(calcGt);
        blockMenu->removeAction(calcLt);
        blockMenu->removeAction(calcSin);
        blockMenu->removeAction(calcCos);
        blockMenu->removeAction(calcInc);
        blockMenu->removeAction(calcDec);
    }
}

void BLOCKEDITOR::matr2(){
    if (calcIsOpen == true){
        calc();
    } else if (vectIsOpen == true){
        vect();
    } else if (matr3IsOpen == true){
        matr3();
    }
    if (matr2IsOpen == false){
        matr2IsOpen = true;
        matrix2BTN->setIcon(QIcon(":/icons/contractMatr2.png"));
        matrix3BTN->setIcon(QIcon(":/icons/btmMatr3.png"));
        blockMenu->removeAction(matrix3BTN);
        blockMenu->removeAction(vectorBTN);
        blockMenu->addAction(matrixInp);
        blockMenu->addAction(matrixOut);
        blockMenu->addAction(matrixAdd);
        blockMenu->addAction(matrixSub);
        blockMenu->addAction(matrixMul);
        blockMenu->addAction(matrixMulVec);
        blockMenu->addAction(matrixDetIn);
        blockMenu->addAction(matrixDetOut);
        blockMenu->addAction(matrixDet);
        blockMenu->addAction(matrix3BTN);
        blockMenu->addAction(vectorBTN);
    } else{
        matr2IsOpen = false;
        matrix2BTN->setIcon(QIcon(":/icons/expandMatr2.png"));
        matrix3BTN->setIcon(QIcon(":/icons/expandMatr3.png"));
        blockMenu->removeAction(matrixInp);
        blockMenu->removeAction(matrixOut);
        blockMenu->removeAction(matrixAdd);
        blockMenu->removeAction(matrixSub);
        blockMenu->removeAction(matrixMul);
        blockMenu->removeAction(matrixMulVec);
        blockMenu->removeAction(matrixDetIn);
        blockMenu->removeAction(matrixDetOut);
        blockMenu->removeAction(matrixDet);
    }
}

void BLOCKEDITOR::matr3(){
    if (calcIsOpen == true){
        calc();
    } else if (vectIsOpen == true){
        vect();
    } else if (matr2IsOpen == true){
        matr2();
    }
    if (matr3IsOpen == false){
        matr3IsOpen = true;
        matrix3BTN->setIcon(QIcon(":/icons/contractMatr3.png"));
        vectorBTN->setIcon(QIcon(":/icons/btmVect.png"));
        blockMenu->removeAction(vectorBTN);
        blockMenu->addAction(matrix3In);
        blockMenu->addAction(matrix3Out);
        blockMenu->addAction(matrix3Add);
        blockMenu->addAction(matrix3Sub);
        blockMenu->addAction(matrix3Mul);
        blockMenu->addAction(vectorBTN);
    } else{
        matr3IsOpen = false;
        matrix3BTN->setIcon(QIcon(":/icons/expandMatr3.png"));
        vectorBTN->setIcon(QIcon(":/icons/expandVect.png"));
        blockMenu->removeAction(matrix3In);
        blockMenu->removeAction(matrix3Out);
        blockMenu->removeAction(matrix3Add);
        blockMenu->removeAction(matrix3Sub);
        blockMenu->removeAction(matrix3Mul);
    }
}

void BLOCKEDITOR::vect(){
    if (calcIsOpen == true){
        calc();
    } else if (matr2IsOpen == true){
        matr2();
    } else if (matr3IsOpen == true){
        matr3();
    }
    if (vectIsOpen == false){
        vectIsOpen = true;
        vectorBTN->setIcon(QIcon(":/icons/contractVect.png"));
        blockMenu->addAction(vectorInp);
        blockMenu->addAction(vectorOut);
        blockMenu->addAction(vectorAdd);
        blockMenu->addAction(vectorSub);
        blockMenu->addAction(vectorMul);
        blockMenu->addAction(vectorDot);
        blockMenu->addAction(vectorDotOut);
        blockMenu->addAction(vectorInp3D);
        blockMenu->addAction(vectorOut3D);
        blockMenu->addAction(vectorAdd3D);
        blockMenu->addAction(vectorSub3D);
        blockMenu->addAction(vectorMul3D);
    } else{
        vectIsOpen = false;
        vectorBTN->setIcon(QIcon(":/icons/expandVect.png"));
        blockMenu->removeAction(vectorInp);
        blockMenu->removeAction(vectorOut);
        blockMenu->removeAction(vectorAdd);
        blockMenu->removeAction(vectorSub);
        blockMenu->removeAction(vectorMul);
        blockMenu->removeAction(vectorDot);
        blockMenu->removeAction(vectorDotOut);
        blockMenu->removeAction(vectorInp3D);
        blockMenu->removeAction(vectorOut3D);
        blockMenu->removeAction(vectorAdd3D);
        blockMenu->removeAction(vectorSub3D);
        blockMenu->removeAction(vectorMul3D);
    }
}

void BLOCKEDITOR::newFile(){
    if(maybeSave()){
		graph.clearGraph();
		setCurrentFile(QString::fromStdString(""));
	}
}

void BLOCKEDITOR::open(){
    if (maybeSave()){
		QString fileName = QFileDialog::getOpenFileName(this,
            QString(), QString(), QString("Súbory dizajnéru (*.jee)"));
		if (!fileName.isEmpty())
			loadFile(fileName, false);
	}
}

void BLOCKEDITOR::merge(){
	QString fileName = QFileDialog::getOpenFileName(this,
        QString("Vložiť"), QString(), QString("Súbory dizajnéru (*.jee)"));
	if (!fileName.isEmpty())
		loadFile(fileName, true);
}

bool BLOCKEDITOR::save(){
    if (curFile.isEmpty()){
		return saveAs();
    } else{
		return saveFile(curFile);
	}
}

bool BLOCKEDITOR::saveAs(){
	QFileDialog dialog(this);
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter("Súbory dizajnéru (*.jee)");
	std::string saveName;
    if (curFile.isEmpty()){
		if (graph.GetName().empty()){
            saveName = "plocha1.jee";
        } else{
            saveName = graph.GetName() + ".jee";
		}
    } else{
		saveName = curFile.toStdString();
	}
	dialog.selectFile(saveName.c_str());
	QStringList files;
    if (dialog.exec()){
		files = dialog.selectedFiles();
    } else{
		return false;
    }
	return saveFile(files.at(0));
}

void BLOCKEDITOR::compute(){
	graph.computeAll();
}

void BLOCKEDITOR::step(){
	graph.computeStep();
}

void BLOCKEDITOR::reset(){
	graph.computeReset();
}

void BLOCKEDITOR::help(){
    QMessageBox::about(this, "Nápoveda",
                             "<h1>(j)Elitný editor - nápoveda</h1>"
                             "<h2>Bloky:</h2>"
                             "<p><b>Pre vytvorenie bloku klikni pravým tlačítkom na myši a vyber požadovaný blok.</p>"
                             "<p><b>Pre odstránenie bloku klikni na daný blok pravým tlačítkom na myši a vyber 'Odstrániť'.</p>"
                             "<h2>Prepojenia:</h2>"
                             "<p><b>Pre vytvorenie prepojenia medzi dvomi blokmi klikni ľavým tlačítkom na myši na výstup jedného a následne vstup druhého bloku.</p>"
                             "<p><b>Pre upravenie prepojenia klikni ľavým tlačítkom na myši na vstup bloku a následne klikni na požadovaný výstup.</p>"
                             "<p><b>Pre odstránenie prepojenia klikni ľavým tlačítkom na myši na vstup bloku a následne klikni do priestoru.</p>"
                             "<h2>Výpočty:</h2>"
                             "<p><b>Pre výpočet všetkých blokov naraz klikni na tlačidlo 'Vypočítať', alebo stlač F3 na klávesnici.</p>"
                             "<p><b>Pre výpočet jedného kroku klikni na tlačidlo 'Krokovať', alebo stlač F4 na klávesnici.</p>"
                             "<p><b>Pre vynulovanie výpočtov klikni na tlačidlo 'Obnoviť', alebo stlač F5 na klávesnici.</p>"
                             "<h1>O (j)Elitnom editore</h1>"
                             "<p><b>Autori:</b> Martin Rakús, Tomáš Zaťko</p>"
					   );
}

bool BLOCKEDITOR::maybeSave(){
    if(this->isWindowModified()){
        QMessageBox ret(QMessageBox::Question, tr("Uložiť?"), tr("Plocha bola upravená.\nPrajete si uložiť vykonané zmeny?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        ret.setButtonText(QMessageBox::Save, tr("Uložiť"));
        ret.setButtonText(QMessageBox::Discard, tr("Zahodiť"));
        ret.setButtonText(QMessageBox::Cancel, tr("Zrušiť"));
        switch(ret.exec()){
            case QMessageBox::Save:
                return save();
                break;
            case QMessageBox::Cancel:
                return false;
                break;
            case QMessageBox::Discard:
                break;
            default:
                break;
        }
	}
	return true;
}

void BLOCKEDITOR::loadFile(const QString &fileName, bool merge){
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "(j)Elitný editor",
                                   QString::fromStdString("Nemožno otvoriť súbor %1:\n%2.")
							 .arg(fileName)
							 .arg(file.errorString()));
		return;
	}

	QTextStream in(&file);
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	// FILE LOADING
	std::stringstream funcIn;
	while(!in.atEnd()){
		funcIn << in.read(1024).toStdString() << '\n';
	}

	if(!graph.loadGraph(funcIn, merge)) {
        QMessageBox::warning(this, "(j)Elitný editor",
                                   QString::fromStdString("Chyba čítania súboru."));
	}
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

    statusBar()->showMessage("Súbor bol otvorený", 2000);

	if(!merge){
		setCurrentFile(fileName);
		setWindowModified(false);
	}
}


bool BLOCKEDITOR::saveFile(const QString &fileName){
	QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "(j)Elitný editor",
                             QString::fromStdString("Nemožno uložiť súbor %1:\n%2.")
							 .arg(fileName)
							 .arg(file.errorString()));
		return false;
	}

	QTextStream out(&file);
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	// FILE SAVING
	std::stringstream funcOut;
	funcOut = graph.saveGraph();

    while(funcOut.good()){
		std::string str;
		std::getline(funcOut, str);
		out << QString::fromStdString(str) << '\n';
	}
    if(funcOut.fail()){
        QMessageBox::warning(this, "(j)Elitný editor",
                                   QString::fromStdString("Chyba zápisu súboru."));
	}
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	setCurrentFile(fileName);
    statusBar()->showMessage("Súbor bol uložený", 2000);
	return true;
}

void BLOCKEDITOR::setCurrentFile(const QString &fileName){
	curFile = fileName;
	setWindowModified(false);
	QString shownName = curFile;
    if (curFile.isEmpty()){
        shownName = "plocha.jee";
	}
	setWindowFilePath(shownName);
}
