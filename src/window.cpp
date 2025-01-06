// window.cpp
#include "window.hpp"
#include <QVBoxLayout>
#include <iostream>

App::App(QWidget *parent) : QMainWindow(parent), mButtonPressedAmt(0) {
  // Create a central widget to hold layout-based controls
  auto centralWidget = new QWidget(this);

  this->resize(800, 600); // Example size

  this->mpLabel = new QLabel(centralWidget);
  this->mpLabel->setWordWrap(true); // Enable word wrapping
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
    // Use the stored mChapterText when updating the passage
    this->updatePassage(mChapterText);
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

void App::updatePassage(const QString &content) {
  // Define chunk size
  int chunkSize = 1000;

  // Calculate the starting character
  int startChar = mButtonPressedAmt * chunkSize;

  // Check if startChar exceeds content length
  if (startChar >= content.length()) {
    // Reset to loop back to the beginning
    mButtonPressedAmt = 0;
    startChar = 0;
  }

  // Extract the passage
  QString passage = content.mid(startChar, chunkSize);

  // Update the label with the passage
  this->mpLabel->setText(passage);

  // Emit signal for logging or tracking
  emit sigLabelTextUpdated(this->mpLabel->text().toStdString());
}

// Store and display the full chapter text initially
void App::setChapterContent(const QString &content) {
  // Save the content so it can be reused in updatePassage(...)
  mChapterText = content;

  // Optionally display the initial text (e.g., first 1000 characters)
  this->mpLabel->setText(mChapterText.left(1000));

  // Emit signal if you want to log or track changes
  emit sigLabelTextUpdated(this->mpLabel->text().toStdString());
}
