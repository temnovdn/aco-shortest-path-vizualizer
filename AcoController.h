#ifndef ACOCONTROLLER_H
#define ACOCONTROLLER_H
#include <QObject>
#include <QString>

class QPushButton;
class QLabel;
class QHBoxLayout;
class QWidget;
class SelectionController;

class AcoController : public QObject
{
  Q_OBJECT
public:
    // selector: we read source/dest from it, and reset it on "Reset"
    explicit AcoController(SelectionController* selector, QWidget* parent = nullptr);
    void addToLayout(QHBoxLayout* layout);
    void sync();// after selection changes enable/disable Run

signals:
    // Step 4 will connect to this to actually run the algorithm
    void runRequested(const QString& source, const QString& destination);

private slots:
    void onRunClicked();
    void onResetClicked();

private:
    QString selectionText() const;
    SelectionController* m_selector = nullptr;
    QWidget* m_parentWidget = nullptr;
    QPushButton* m_runBtn = nullptr;
    QPushButton* m_resetBtn = nullptr;
    QLabel* m_infoLabel = nullptr;
};

#endif
