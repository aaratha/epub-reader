#include "window.hpp"
#include <QVBoxLayout>
#include <iostream>

App::App(QWidget *parent) : QMainWindow(parent), mButtonPressedAmt(0) {
  // Create a central widget to hold layout-based controls
  auto centralWidget = new QWidget(this);

  this->mpLabel = new QLabel(centralWidget);
  this->mpPushButton = new QPushButton("Press Me!", centralWidget);

  // Initial text on the label
  this->updateLabelText();

  // Layout for the central widget
  auto mainLayout = new QVBoxLayout();
  mainLayout->addWidget(this->mpLabel);
  mainLayout->addSpacerItem(
      new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
  mainLayout->addWidget(this->mpPushButton);

  centralWidget->setLayout(mainLayout);
  this->setCentralWidget(centralWidget);

  // Connect the button's click to increment and update text
  QObject::connect(this->mpPushButton, &QPushButton::clicked, this, [this]() {
    this->mButtonPressedAmt++;
    this->updateLabelText();
  });

  // Print label text in the console whenever sigLabelTextUpdated is emitted
  QObject::connect(this, &App::sigLabelTextUpdated, this,
                   [](std::string_view val) { std::cout << val << std::endl; });
}

void App::updateLabelText() {
  // Update label with the click count
  this->mpLabel->setText("Button was clicked: " +
                         QString::number(this->mButtonPressedAmt) + " times.");

  // Emit our custom signal
  emit sigLabelTextUpdated(this->mpLabel->text().toStdString());
}

// This function can be called from anywhere (e.g., after loading an ePub
// chapter)
void App::setChapterContent(const QString &content) {
  // Update the label text with the chapter content
  this->mpLabel->setText(content);

  // Optionally emit the signal if you want to log or track changes
  emit sigLabelTextUpdated(this->mpLabel->text().toStdString());
}
