#include "mainwindow.h"
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QSpinBox>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QXmlStreamReader>
#include <QtCore/private/qzipreader_p.h>
#include <algorithm>

namespace {
QString clean(QString s) {
    return s.trimmed().replace("\r", " ").replace("\n", " ");
}
QString key(QString s) {
    return clean(s).remove(' ').remove('\t').toLower();
}
} // namespace

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle(QStringLiteral("题库训练器"));
    resize(1120, 760);
    buildUi();
    goHome();
}
QString MainWindow::typeName(int type) const {
    return type == Single     ? QStringLiteral("单选题")
           : type == Multiple ? QStringLiteral("多选题")
                              : QStringLiteral("判断题");
}
QString MainWindow::typeFolder(int type) const {
    return type == Single     ? QStringLiteral("single")
           : type == Multiple ? QStringLiteral("multiple")
                              : QStringLiteral("judge");
}
QString MainWindow::bankRoot(int type) const {
    const QString root = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                         QStringLiteral("/题库/") + typeFolder(type);
    QDir().mkpath(root);
    return root;
}

void MainWindow::buildUi() {
    central = new QWidget(this);
    setCentralWidget(central);
    auto *root = new QVBoxLayout(central);
    root->setContentsMargins(28, 24, 28, 24);
    root->setSpacing(18);
    auto *top = new QHBoxLayout;
    pageTitle = new QLabel;
    pageTitle->setObjectName("pageTitle");
    top->addWidget(pageTitle);
    top->addStretch();
    auto *home = new QPushButton(QStringLiteral("首页"));
    top->addWidget(home);
    // 首页按钮点击后执行 goHome()。
    connect(home, &QPushButton::clicked, this, &MainWindow::goHome);
    root->addLayout(top);
    pages = new QStackedWidget;
    root->addWidget(pages, 1);
    homePage = new QWidget;
    auto *hl = new QVBoxLayout(homePage);
    auto *intro = new QLabel(QStringLiteral(
        "导入 Excel "
        "题库，随机生成专属训练。\n每种题型可维护多个独立题库，训练题数会自动按题库容量校验。"));
    intro->setObjectName("intro");
    hl->addWidget(intro);
    auto *cards = new QHBoxLayout;
    const QStringList names = {QStringLiteral("单选题"), QStringLiteral("多选题"),
                               QStringLiteral("判断题")};
    for (int i = 0; i < names.size(); ++i) {
        auto *b = new QPushButton(names.at(i) + QStringLiteral("\n进入题库"));
        b->setMinimumHeight(130);
        b->setObjectName("typeCard");
        // 三个入口共用 openType()，i 分别对应单选、多选、判断。
        connect(b, &QPushButton::clicked, this, [this, i] { openType(i); });
        cards->addWidget(b);
    }
    hl->addLayout(cards);
    hl->addStretch();
    pages->addWidget(homePage);
    managePage = new QWidget;
    auto *ml = new QVBoxLayout(managePage);
    bankHint = new QLabel;
    bankHint->setObjectName("hint");
    ml->addWidget(bankHint);
    auto *acts = new QHBoxLayout;
    auto *imp = new QPushButton(QStringLiteral("导入 Excel"));
    auto *ren = new QPushButton(QStringLiteral("重命名"));
    auto *del = new QPushButton(QStringLiteral("删除题库"));
    auto *train = new QPushButton(QStringLiteral("开始训练"));
    acts->addWidget(imp);
    acts->addWidget(ren);
    acts->addWidget(del);
    acts->addStretch();
    acts->addWidget(train);
    ml->addLayout(acts);
    bankList = new QListWidget;
    ml->addWidget(bankList, 1);
    pages->addWidget(managePage);
    // 题库管理页的按钮事件集中列在这里，方便增加或替换行为。
    connect(imp, &QPushButton::clicked, this, &MainWindow::importBank);
    connect(ren, &QPushButton::clicked, this, &MainWindow::renameBank);
    connect(del, &QPushButton::clicked, this, &MainWindow::deleteBank);
    connect(train, &QPushButton::clicked, this, &MainWindow::showTrainingSetup);
    setupPage = new QWidget;
    auto *sl = new QVBoxLayout(setupPage);
    auto *form = new QFormLayout;
    bankCombo = new QComboBox;
    countSpin = new QSpinBox;
    countSpin->setRange(1, 1);
    form->addRow(QStringLiteral("选择题库"), bankCombo);
    form->addRow(QStringLiteral("训练题数"), countSpin);
    sl->addLayout(form);
    auto *note = new QLabel(QStringLiteral("题目将从所选题库中随机抽取且不重复。"));
    note->setObjectName("hint");
    sl->addWidget(note);
    auto *start = new QPushButton(QStringLiteral("开始训练"));
    sl->addWidget(start);
    connect(start, &QPushButton::clicked, this, &MainWindow::startTraining);
    connect(bankCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int) {
        if (bankCombo->currentIndex() < 0)
            return;
        QVector<Question> qs;
        QString e;
        if (readXlsx(bankCombo->currentData().toString(), currentType, qs, e))
            countSpin->setRange(1, qMax(1, qs.size()));
    });
    sl->addStretch();
    pages->addWidget(setupPage);
    quizPage = new QWidget;
    auto *ql = new QVBoxLayout(quizPage);
    progressLabel = new QLabel;
    progressLabel->setObjectName("progress");
    ql->addWidget(progressLabel);
    questionLabel = new QLabel;
    questionLabel->setWordWrap(true);
    questionLabel->setObjectName("question");
    ql->addWidget(questionLabel);
    auto *box = new QGroupBox(QStringLiteral("请选择答案"));
    box->setObjectName("answerBox");
    auto *answerLayout = new QVBoxLayout;
    answerLayout->setSpacing(12);
    box->setLayout(answerLayout);
    ql->addWidget(box);
    resultLabel = new QLabel;
    resultLabel->setWordWrap(true);
    resultLabel->setObjectName("result");
    ql->addWidget(resultLabel);
    nextButton = new QPushButton(QStringLiteral("提交答案"));
    nextButton->setObjectName("primaryButton");
    ql->addWidget(nextButton);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextQuestion);
    pages->addWidget(quizPage);
    answerGroup = new QButtonGroup(this);
    // UI 颜色、字体、圆角和间距集中在这里修改。
    // 例如：把 #356ae6 改成 #16a085，即可更换主按钮和选中状态的颜色。
    setStyleSheet(
        "QMainWindow{background:#f5f7fb;} "
        "QLabel#pageTitle{font-size:28px;font-weight:700;color:#172033;} "
        "QLabel#intro{font-size:18px;color:#536174;padding:18px 0;} "
        "QLabel#hint{color:#657184;font-size:14px;} "
        "QPushButton{padding:10px 18px;border:1px solid #d6dce6;border-radius:6px;"
        "background:#ffffff;color:#20304d;font-size:15px;} "
        "QPushButton:hover{background:#eaf1ff;color:#20304d;} "
        "QPushButton#primaryButton{background:#356ae6;color:white;border:0;font-size:18px;font-"
        "weight:600;min-height:48px;} "
        "QPushButton#primaryButton:hover{background:#2856c6;color:#ffffff;} "
        "QPushButton#primaryButton:pressed{background:#1f45a5;color:#ffffff;} "
        "QPushButton:disabled{background:#eef1f5;color:#8a94a3;border-color:#d9dee8;} "
        "QPushButton#typeCard{font-size:20px;font-weight:600;color:#20304d;background:white;border:"
        "1px solid #dbe2ef;} "
        "QLabel#question{font-size:22px;font-weight:600;color:#172033;background:#ffffff;padding:"
        "24px;"
        "border-radius:8px;min-height:90px;} "
        "QLabel#progress{color:#356ae6;font-size:16px;font-weight:600;} "
        "QLabel#result{font-size:18px;background:#fffaf0;border:1px solid "
        "#f0d9a6;border-radius:10px;padding:14px;color:#3d3426;} "
        "QListWidget{background:#ffffff;color:#20304d;border:1px solid "
        "#dbe2ef;border-radius:6px;padding:6px;} "
        "QListWidget::item:selected{background:#e7efff;color:#1747b3;} "
        "QComboBox,QSpinBox{padding:9px;border:1px solid "
        "#ccd5e3;border-radius:5px;background:#ffffff;color:#20304d;} "
        "QComboBox "
        "QAbstractItemView{background:#ffffff;color:#20304d;selection-background-color:#e7efff;"
        "selection-color:#1747b3;} "
        "QRadioButton#answerOption,QCheckBox#answerOption{font-size:20px;font-weight:600;color:#"
        "20304d;background:white;border:2px solid #d6dce6;border-radius:14px;padding:16px "
        "20px;min-height:30px;} "
        "QRadioButton#answerOption:hover,QCheckBox#answerOption:hover{background:#eaf1ff;border-"
        "color:#6e95ed;} "
        "QRadioButton#answerOption:checked,QCheckBox#answerOption:checked{background:#e7efff;"
        "border-color:#356ae6;color:#1747b3;} "
        "QRadioButton#answerOption:disabled,QCheckBox#answerOption:disabled{color:#20304d;"
        "background:#ffffff;border-color:#d6dce6;}");
}
void MainWindow::goHome() {
    pageTitle->setText(QStringLiteral("题库训练器"));
    pages->setCurrentWidget(homePage);
}
void MainWindow::openType(int type) {
    currentType = type;
    pageTitle->setText(typeName(type) + QStringLiteral("题库"));
    refreshBanks();
    pages->setCurrentWidget(managePage);
}
void MainWindow::refreshBanks() {
    bankList->clear();
    bankCombo->clear();
    QDir d(bankRoot(currentType));
    const auto files = d.entryList({"*.xlsx", "*.xlsm"}, QDir::Files, QDir::Name);
    for (const QString &f : files) {
        bankList->addItem(QFileInfo(f).completeBaseName());
        bankCombo->addItem(QFileInfo(f).completeBaseName(), d.filePath(f));
    }
    bankHint->setText(QStringLiteral("%1：已保存 %2 个题库。题库目录：%3")
                          .arg(typeName(currentType))
                          .arg(files.size())
                          .arg(d.absolutePath()));
    if (files.isEmpty())
        bankList->addItem(QStringLiteral("暂无题库，请先导入 Excel 文件"));
}
void MainWindow::importBank() {
    const QString src =
        QFileDialog::getOpenFileName(this, QStringLiteral("选择 Excel 题库"), QString(),
                                     QStringLiteral("Excel 文件 (*.xlsx *.xlsm)"));
    if (src.isEmpty())
        return;
    bool ok = false;
    const QString n =
        QInputDialog::getText(this, QStringLiteral("命名题库"), QStringLiteral("题库名称："),
                              QLineEdit::Normal, QFileInfo(src).completeBaseName(), &ok)
            .trimmed();
    if (!ok || n.isEmpty())
        return;
    QString e;
    if (!saveImportedBank(src, currentType, n, e)) {
        QMessageBox::warning(this, QStringLiteral("导入失败"), e);
        return;
    }
    refreshBanks();
    QMessageBox::information(
        this, QStringLiteral("导入成功"),
        QStringLiteral("题库“%1”已保存。共识别题目后可进入训练。\n文件已复制到应用题库目录。")
            .arg(n));
}
bool MainWindow::saveImportedBank(const QString &src, int type, const QString &name,
                                  QString &error) const {
    QVector<Question> qs;
    if (!readXlsx(src, type, qs, error))
        return false;
    if (qs.isEmpty()) {
        error = QStringLiteral("Excel 中没有可用题目，请检查表头和题目内容。");
        return false;
    }
    QString safe = name;
    safe.replace(QRegularExpression(R"([\\/:*?"<>|])"), "_");
    const QString dst = bankRoot(type) + "/" + safe + ".xlsx";
    if (QFile::exists(dst)) {
        error = QStringLiteral("同名题库已存在，请换一个名称。");
        return false;
    }
    if (!QFile::copy(src, dst)) {
        error = QStringLiteral("无法复制文件到题库目录，请检查权限。");
        return false;
    }
    return true;
}
void MainWindow::deleteBank() {
    const int r = bankList->currentRow();
    if (r < 0 || bankList->item(r)->text().startsWith(QStringLiteral("暂无")))
        return;
    const QString n = bankList->item(r)->text();
    if (QMessageBox::question(this, QStringLiteral("确认删除"),
                              QStringLiteral("确定删除题库“%1”吗？此操作不可撤销。").arg(n)) ==
        QMessageBox::Yes) {
        QDir(bankRoot(currentType)).remove(n + QStringLiteral(".xlsx"));
        refreshBanks();
    }
}
void MainWindow::renameBank() {
    const int r = bankList->currentRow();
    if (r < 0 || bankList->item(r)->text().startsWith(QStringLiteral("暂无")))
        return;
    const QString old = bankList->item(r)->text();
    bool ok = false;
    QString n = QInputDialog::getText(this, QStringLiteral("重命名题库"),
                                      QStringLiteral("新名称："), QLineEdit::Normal, old, &ok)
                    .trimmed();
    if (!ok || n.isEmpty() || n == old)
        return;
    n.replace(QRegularExpression(R"([\\/:*?"<>|])"), "_");
    if (!QDir(bankRoot(currentType))
             .rename(old + QStringLiteral(".xlsx"), n + QStringLiteral(".xlsx")))
        QMessageBox::warning(this, QStringLiteral("重命名失败"),
                             QStringLiteral("文件名无效或目标名称已存在。"));
    refreshBanks();
}
void MainWindow::showTrainingSetup() {
    refreshBanks();
    pageTitle->setText(QStringLiteral("训练设置 · ") + typeName(currentType));
    pages->setCurrentWidget(setupPage);
    if (bankCombo->count()) {
        QVector<Question> qs;
        QString e;
        if (readXlsx(bankCombo->currentData().toString(), currentType, qs, e))
            countSpin->setRange(1, qMax(1, qs.size()));
    }
}
void MainWindow::startTraining() {
    if (bankCombo->currentIndex() < 0) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("请先导入并选择题库。"));
        return;
    }
    QString e;
    if (!readXlsx(bankCombo->currentData().toString(), currentType, questions, e)) {
        QMessageBox::warning(this, QStringLiteral("读取失败"), e);
        return;
    }
    if (countSpin->value() > questions.size()) {
        QMessageBox::warning(
            this, QStringLiteral("题数不足"),
            QStringLiteral("当前题库只有 %1 题，训练数不能超过题库数量。").arg(questions.size()));
        return;
    }
    trainingIndices.clear();
    for (int i = 0; i < questions.size(); ++i)
        trainingIndices << i;
    std::shuffle(trainingIndices.begin(), trainingIndices.end(), *QRandomGenerator::global());
    trainingIndices.resize(countSpin->value());
    currentIndex = 0;
    score = 0;
    wrongQuestionSummaries.clear();
    trainingFinished = false;
    pageTitle->setText(QStringLiteral("正在训练 · ") + typeName(currentType));
    pages->setCurrentWidget(quizPage);
    showCurrentQuestion();
}
void MainWindow::clearQuestionWidgets() {
    auto *box = quizPage->findChild<QGroupBox *>("answerBox");
    if (box && box->layout()) {
        while (QLayoutItem *item = box->layout()->takeAt(0)) {
            if (QWidget *w = item->widget())
                w->deleteLater();
            delete item;
        }
    }
    answerChecks.clear();
    answerRadios.clear();
}
void MainWindow::showCurrentQuestion() {
    if (currentIndex < 0 || currentIndex >= trainingIndices.size()) {
        finishTraining();
        return;
    }
    const int questionIndex = trainingIndices.at(currentIndex);
    if (questionIndex < 0 || questionIndex >= questions.size()) {
        finishTraining();
        return;
    }
    clearQuestionWidgets();
    answerShown = false;
    resultLabel->clear();
    resultLabel->setStyleSheet(QString());
    if (nextButton)
        nextButton->setText(QStringLiteral("提交答案"));
    const Question &q = questions.at(questionIndex);
    progressLabel->setText(
        QStringLiteral("第 %1 / %2 题").arg(currentIndex + 1).arg(trainingIndices.size()));
    questionLabel->setText(q.text);
    auto *box = quizPage->findChild<QGroupBox *>("answerBox");
    if (!box || !box->layout())
        return;
    auto *lay = qobject_cast<QVBoxLayout *>(box->layout());
    if (!lay)
        return;
    for (const QString &opt : q.options) {
        if (currentType == Multiple) {
            auto *c = new QCheckBox(opt);
            c->setObjectName("answerOption");
            c->setMinimumHeight(58);
            lay->addWidget(c);
            answerChecks << c;
        } else {
            auto *r = new QRadioButton(opt);
            r->setObjectName("answerOption");
            r->setMinimumHeight(58);
            lay->addWidget(r);
            answerRadios << r;
        }
    }
}
QStringList MainWindow::selectedAnswers() const {
    QStringList a;
    for (int i = 0; i < answerChecks.size(); ++i)
        if (answerChecks.at(i) && answerChecks.at(i)->isChecked())
            a << QString(QChar('A' + i));
    for (int i = 0; i < answerRadios.size(); ++i)
        if (answerRadios.at(i) && answerRadios.at(i)->isChecked())
            a << QString(QChar('A' + i));
    return a;
}
void MainWindow::nextQuestion() {
    if (trainingFinished) {
        goHome();
        return;
    }
    if (currentIndex < 0 || currentIndex >= trainingIndices.size()) {
        finishTraining();
        return;
    }
    const int questionIndex = trainingIndices.at(currentIndex);
    if (questionIndex < 0 || questionIndex >= questions.size()) {
        finishTraining();
        return;
    }
    if (answerShown) {
        if (++currentIndex >= trainingIndices.size()) {
            finishTraining();
            return;
        }
        showCurrentQuestion();
        return;
    }
    QStringList got = selectedAnswers();
    if (got.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("请选择答案后再提交。"));
        return;
    }
    QStringList exp = questions.at(questionIndex)
                          .answer.split(QRegularExpression("[,，、\\s]+"), Qt::SkipEmptyParts);
    for (QString &s : exp) {
        s = s.trimmed().toUpper();
        if (currentType == Judge) {
            if (s == QStringLiteral("正确") || s == QStringLiteral("对") ||
                s == QStringLiteral("TRUE"))
                s = QStringLiteral("A");
            else if (s == QStringLiteral("错误") || s == QStringLiteral("错") ||
                     s == QStringLiteral("FALSE"))
                s = QStringLiteral("B");
        }
    }
    std::sort(exp.begin(), exp.end());
    std::sort(got.begin(), got.end());
    const bool correct = (got == exp);
    if (correct)
        ++score;
    answerShown = true;
    for (auto *w : answerChecks)
        if (w)
            w->setEnabled(false);
    for (auto *w : answerRadios)
        if (w)
            w->setEnabled(false);
    const auto formatAnswers = [this](const QStringList &answers) {
        if (currentType == Judge)
            return answers.contains(QStringLiteral("A")) ? QStringLiteral("正确（A）")
                                                         : QStringLiteral("错误（B）");
        return answers.join(QStringLiteral("、"));
    };
    const QString answerText = formatAnswers(exp);
    const QString selectedText = formatAnswers(got);
    const QString explanation = questions.at(questionIndex).explanation.trimmed();
    if (!correct) {
        wrongQuestionSummaries << QStringLiteral("第 %1 题：%2\n你的答案：%3\n正确答案：%4%5")
                                      .arg(currentIndex + 1)
                                      .arg(questions.at(questionIndex).text)
                                      .arg(selectedText)
                                      .arg(answerText)
                                      .arg(explanation.isEmpty()
                                               ? QString()
                                               : QStringLiteral("\n解析：") + explanation);
    }
    const auto markOptions = [&exp](auto &widgets) {
        for (int i = 0; i < widgets.size(); ++i) {
            auto *widget = widgets.at(i);
            if (!widget)
                continue;
            const QString letter(QChar('A' + i));
            const bool isCorrectOption = exp.contains(letter);
            const bool isSelectedOption = widget->isChecked();
            if (isSelectedOption && !isCorrectOption) {
                widget->setStyleSheet(
                    QStringLiteral("background:#fff1f1;color:#b42318;border:2px solid #e05252;"));
            } else if (isCorrectOption) {
                widget->setStyleSheet(
                    QStringLiteral("background:#effaf2;color:#167a3d;border:2px solid #42b883;"));
            }
        }
    };
    markOptions(answerChecks);
    markOptions(answerRadios);
    resultLabel->setStyleSheet(
        correct ? QStringLiteral("background:#effaf2;color:#167a3d;border:1px solid #42b883;")
                : QStringLiteral("background:#fff1f1;color:#b42318;border:1px solid #e05252;"));
    resultLabel->setText(
        QStringLiteral("%1\n正确答案：%2%3")
            .arg(correct ? QStringLiteral("回答正确") : QStringLiteral("回答错误"), answerText,
                 explanation.isEmpty() ? QString() : QStringLiteral("\n解析：") + explanation));
    if (nextButton)
        nextButton->setText(currentIndex + 1 >= trainingIndices.size() ? QStringLiteral("完成训练")
                                                                       : QStringLiteral("下一题"));
}
void MainWindow::finishTraining() {
    trainingFinished = true;
    pageTitle->setText(QStringLiteral("训练完成"));
    questionLabel->setText(QStringLiteral("本次训练已完成"));
    progressLabel->setText(QStringLiteral("得分：%1 / %2").arg(score).arg(trainingIndices.size()));
    clearQuestionWidgets();
    resultLabel->setStyleSheet(
        QStringLiteral("background:#ffffff;color:#20304d;border:1px solid #d6dce6;"));
    QString summary;
    if (wrongQuestionSummaries.isEmpty()) {
        summary = QStringLiteral("本次训练全部答对！");
    } else {
        summary = QStringLiteral("错题（共 %1 道）：\n\n%2")
                      .arg(wrongQuestionSummaries.size())
                      .arg(wrongQuestionSummaries.join(QStringLiteral("\n\n")));
    }
    resultLabel->setText(summary);
    if (nextButton)
        nextButton->setText(QStringLiteral("返回首页"));
}

bool MainWindow::readXlsx(const QString &path, int type, QVector<Question> &out,
                          QString &error) const {
    QZipReader zip(path);
    if (!zip.exists()) {
        error = QStringLiteral("无法打开 Excel 文件。");
        return false;
    }
    const QByteArray ss = zip.fileData(QStringLiteral("xl/sharedStrings.xml"));
    QStringList shared;
    if (!ss.isEmpty()) {
        QXmlStreamReader x(ss);
        bool in = false;
        QString cur;
        while (!x.atEnd()) {
            x.readNext();
            if (x.isStartElement() && x.name() == "si") {
                cur.clear();
                in = true;
            } else if (in && x.isCharacters())
                cur += x.text().toString();
            else if (x.isEndElement() && x.name() == "si") {
                shared << cur;
                in = false;
            }
        }
    }
    const QByteArray data = zip.fileData(QStringLiteral("xl/worksheets/sheet1.xml"));
    if (data.isEmpty()) {
        error = QStringLiteral("Excel 中找不到第一个工作表。");
        return false;
    }
    QXmlStreamReader x(data);
    QVector<QStringList> rows;
    QStringList row;
    int col = -1;
    QString val;
    int kind = 0;
    bool validCell = false;
    while (!x.atEnd()) {
        x.readNext();
        if (x.isStartElement() && x.name() == "row") {
            row.clear();
            col = -1;
        } else if (x.isStartElement() && x.name() == "c") {
            auto a = x.attributes();
            QString ref = a.value("r").toString();
            int p = 0;
            int parsed = -1;
            while (p < ref.size() && ref[p].isLetter()) {
                if (parsed < 0)
                    parsed = 0;
                parsed = parsed * 26 + ref[p].toUpper().unicode() - 'A' + 1;
                ++p;
            }
            col = parsed - 1;
            validCell = (col >= 0 && col < 10000);
            kind = a.value("t") == "s" ? 1 : a.value("t") == "inlineStr" ? 2 : 0;
            val.clear();
        } else if (x.isStartElement() && (x.name() == "v" || x.name() == "t"))
            val = x.readElementText();
        else if (x.isEndElement() && x.name() == "c") {
            if (validCell) {
                while (row.size() <= col)
                    row << QString();
                if (kind == 1) {
                    bool ok = false;
                    int n = val.toInt(&ok);
                    row[col] = ok && n >= 0 && n < shared.size() ? shared.at(n) : val;
                } else
                    row[col] = val;
            }
            validCell = false;
        } else if (x.isEndElement() && x.name() == "row")
            rows << row;
    }
    if (x.hasError() || rows.size() < 2) {
        error = QStringLiteral("Excel 内容格式无法识别。");
        return false;
    }
    const QStringList &header = rows.first();
    QHash<QString, int> h;
    for (int i = 0; i < header.size(); ++i)
        h.insert(key(header.at(i)), i);
    auto find = [&](const QStringList &ns) {
        for (const QString &n : ns)
            if (h.contains(n))
                return h.value(n);
        return -1;
    };
    int qi = find({"题目", "问题", "question", "title"}),
        ai = find({"答案", "正确答案", "answer", "correct"}),
        ei = find({"解析", "说明", "explanation"});
    if (qi < 0 || ai < 0) {
        error =
            QStringLiteral("需要包含“题目”和“答案”两列。选项列可使用 A、B、C、D 或 选项A 等表头。");
        return false;
    }
    QVector<int> opts;
    for (int i = 0; i < header.size(); ++i) {
        QString k = key(header.at(i));
        if (k == "a" || k == "b" || k == "c" || k == "d" || k == "e" || k.contains("选项a") ||
            k.contains("选项b") || k.contains("选项c") || k.contains("选项d") ||
            k.contains("选项e"))
            opts << i;
    }
    for (int r = 1; r < rows.size(); ++r) {
        const QStringList &cells = rows.at(r);
        if (qi >= cells.size() || clean(cells.at(qi)).isEmpty())
            continue;
        Question q;
        q.text = clean(cells.at(qi));
        q.answer = clean(ai < cells.size() ? cells.at(ai) : QString()).toUpper();
        q.explanation = ei >= 0 && ei < cells.size() ? clean(cells.at(ei)) : QString();
        for (int c : opts)
            if (c < cells.size() && !clean(cells.at(c)).isEmpty())
                q.options << clean(cells.at(c));
        if (type == Judge && q.options.isEmpty())
            q.options << QStringLiteral("正确") << QStringLiteral("错误");
        if (type == Judge || q.options.size() >= 2)
            out << q;
    }
    return true;
}
