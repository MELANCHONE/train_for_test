#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QVector>

struct Question {
    QString text;
    QStringList options;
    QString answer;
    QString explanation;
};
class QComboBox;
class QListWidget;
class QSpinBox;
class QLabel;
class QStackedWidget;
class QButtonGroup;
class QCheckBox;
class QRadioButton;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;
  private slots:
    void openType(int type);
    void importBank();
    void deleteBank();
    void renameBank();
    void startTraining();
    void nextQuestion();
    void finishTraining();
    void goHome();

  private:
    enum Type {
        Single = 0,
        Multiple = 1,
        Judge = 2
    };
    QString typeName(int type) const;
    QString typeFolder(int type) const;
    QString bankRoot(int type) const;
    void buildUi();
    void refreshBanks();
    void showTrainingSetup();
    void showCurrentQuestion();
    void clearQuestionWidgets();
    bool readXlsx(const QString &path, int type, QVector<Question> &out, QString &error) const;
    bool saveImportedBank(const QString &source, int type, const QString &name,
                          QString &error) const;
    QStringList selectedAnswers() const;
    QWidget *central = nullptr;
    QStackedWidget *pages = nullptr;
    QWidget *homePage = nullptr;
    QWidget *managePage = nullptr;
    QWidget *setupPage = nullptr;
    QWidget *quizPage = nullptr;
    QLabel *pageTitle = nullptr;
    QLabel *bankHint = nullptr;
    QLabel *questionLabel = nullptr;
    QLabel *progressLabel = nullptr;
    QLabel *resultLabel = nullptr;
    QComboBox *bankCombo = nullptr;
    QSpinBox *countSpin = nullptr;
    QListWidget *bankList = nullptr;
    QButtonGroup *answerGroup = nullptr;
    QVector<QCheckBox *> answerChecks;
    QVector<QRadioButton *> answerRadios;
    QVector<Question> questions;
    QVector<int> trainingIndices;
    QStringList wrongQuestionSummaries;
    QPushButton *nextButton = nullptr;
    bool answerShown = false;
    bool trainingFinished = false;
    int currentType = Single;
    int currentIndex = 0;
    int score = 0;
};
#endif
