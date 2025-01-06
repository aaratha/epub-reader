#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QString>

class App : public QMainWindow {
  Q_OBJECT

public:
  explicit App(QWidget *parent = nullptr);

  // New function that can be called from elsewhere
  // (e.g., after parsing an ePub and getting chapter text)
  void setChapterContent(const QString &content);

signals:
  void sigLabelTextUpdated(std::string_view val);

private slots:
  void updateLabelText();

private:
  QLabel *mpLabel;
  QPushButton *mpPushButton;
  int mButtonPressedAmt;
};

#endif // WINDOW_HPP
